#include "alsaAudioPlugin.h"
#include <alsa/asoundlib.h>

#include "core/emulator.h"

#include "misc/log.h"

#include <pthread.h>

#include <cerrno>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t mutex_qte = PTHREAD_MUTEX_INITIALIZER;

void* thread_snd_pcm_writei(void* param)
{
    pthread_mutex_lock(&mutex);
    thread_param* param_thread = (thread_param*)param;
    uint8_t* sndBuffer = param_thread->sndBuffer;
    uint8_t** sndBufferPtr = param_thread->sndBufferPtr;
    snd_pcm_t *pcm_handle = param_thread->pcm_handle; 
    uint32_t* qte = param_thread->qte;
    pthread_mutex_unlock(&mutex);

    int pcm_return;
    while(1)
    {
        pthread_mutex_lock(&mutex_qte);
        int max_frames = 16384 >> 2;
        int frames = max_frames >> 2;
        if(*qte >= frames)
        {
//            DebugLogMessage("1 qte: %d, frames: %d, sndBuffer %d, sndBufferPtr %d", *qte, frames, sndBuffer, *sndBufferPtr);
            pcm_return = snd_pcm_writei(pcm_handle, sndBuffer, frames);
            if(pcm_return < frames)
            {
                snd_pcm_prepare(pcm_handle);
                DebugLogMessage("Underrun %d (%d)", pcm_return, frames);
            }
            (*qte) -= frames;
            (*sndBufferPtr) -= 1*frames*4;
            if((*sndBufferPtr) < sndBuffer) (*sndBufferPtr) = sndBuffer;
//            DebugLogMessage("2 qte: %d, frames: %d, sndBuffer %d, sndBufferPtr %d", *qte, frames, sndBuffer, *sndBufferPtr);
        }
        pthread_mutex_unlock(&mutex_qte);
    }
    /*
    while((pcm_return = snd_pcm_writei(pcm_handle, sndBuffer, 256)) < 0) {
        snd_pcm_prepare(pcm_handle);
        DebugLogMessage("Underrun %d", pcm_return);
    }
    */

    return NULL;
}

AlsaAudioPlugin::AlsaAudioPlugin() :
    thread(NULL),
    qte(0)
{
}

inline uint8_t* AlsaAudioPlugin::getBuffer()
{
    return sndBufferPtr;
}

inline void AlsaAudioPlugin::lock()
{
    pthread_mutex_lock(&mutex_qte);
}

inline void AlsaAudioPlugin::unlock()
{
    pthread_mutex_unlock(&mutex_qte);
}

inline int AlsaAudioPlugin::update()
{
#if 0
    if(thread)
    {
        pthread_join(*thread, NULL);
        free(thread);
        thread = NULL;
    }
#endif

#if 0
    if(!thread)
    {
        pthread_mutex_lock(&mutex);
        param_thread.sndBuffer = sndBuffer;
        param_thread.sndBufferPtr = &sndBufferPtr;
        param_thread.qte = &qte;
        param_thread.pcm_handle = pcm_handle;
        pthread_mutex_unlock(&mutex);

        DebugLogMessage("Create pthread");
        thread = (pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(thread, NULL, thread_snd_pcm_writei, (void*)&param_thread);
    }

    pthread_mutex_lock(&mutex_qte);

//    if(sndBufferPtr < (sndBuffer + periodsize + 4))
    {
        sndBufferPtr += 1*4;
        qte += 1;
    }

    pthread_mutex_unlock(&mutex_qte);

    return 0;
#else
	const int nb = 1;
    sndBufferPtr += (nb*4);
    qte += nb;

    int max_frames = periodsize; // >> 2;
    int frames = max_frames;

    while(qte >= max_frames)
    {
        //DebugLogMessage("1 qte: %d, frames: %d, sndBuffer %d, sndBufferPtr %d, periodsize %d", qte, frames, sndBuffer, sndBufferPtr, periodsize);
        int pcm_return;
        pcm_return = snd_pcm_writei(pcm_handle, sndBuffer, frames);
        if(pcm_return < frames)
        {
            if(pcm_return == -EPIPE)
            {
				int err;
                if(err = snd_pcm_prepare(pcm_handle) < 0)
				{
					DebugLogMessage("Cant recover underrun (%s)", snd_strerror(err));
				}
				else
				{
					DebugLogMessage("Underrun (%s) %d", snd_strerror(pcm_return), frames);
				}
            }
			else if(pcm_return < 0)
			{
				DebugLogMessage("Sound Error (%s)", snd_strerror(pcm_return));
			}
        }
        //DebugLogMessage("2 qte: %d, frames: %d, sndBuffer %d, sndBufferPtr %d, periodsize %d", qte, frames, sndBuffer, sndBufferPtr, periodsize);
        qte -= frames;
        sndBufferPtr -= 1*frames*4;
        //DebugLogMessage("2 qte: %d, frames: %d, sndBuffer %d, sndBufferPtr %d, periodsize %d", qte, frames, sndBuffer, sndBufferPtr, periodsize);
        if(sndBufferPtr < sndBuffer) sndBufferPtr = sndBuffer;
    }
    return 0;
#endif

    int pcm_return;
    while((pcm_return = snd_pcm_writei(pcm_handle, sndBuffer, 32)) < 0) {
        snd_pcm_prepare(pcm_handle);
        DebugLogMessage("Underrun %d", pcm_return);
        return 0;
    }
    return 0;
//    return 0;
//  while((pcmreturn = snd_pcm_writei(pcm_handle, sndBuffer, 1)) < 0) {
//      snd_pcm_prepare(pcm_handle);
//      ErrorLogMessage("Buffer Underrun");
//      return 0;
//  }
}

int AlsaAudioPlugin::init(shared_ptr<t_CPC> cpc, shared_ptr<t_PSG> psg)
{
	if(!cpc->snd_enabled)
	{
		InfoLogMessage("[ALSA Audio Plugin] Not opening audio because disabled in the config");
		return 0;
	}
    /*  TODO */
    unsigned int rate = cpc->snd_playback_rate;
    int channels = cpc->snd_stereo ? 2 : 1;

	snd_pcm_format_t format = SND_PCM_FORMAT_S16;
	switch(cpc->snd_bits)
	{
		case 8:
			format = SND_PCM_FORMAT_U8;
			break;
		case 16:
			format = SND_PCM_FORMAT_S16;
			break;
		case 24:
			format = SND_PCM_FORMAT_S24;
			break;
		case 32:
			format = SND_PCM_FORMAT_S32;
			break;
		default:
			WarningLogMessage("[ALSA Audio Plugin] Warning, %d bits format unknown, fallback to %s.", cpc->snd_bits, snd_pcm_format_name(format));
	}

    int periods = 2;
    periodsize = 4096;

    unsigned int exact_rate;
    int dir, err;

    /* Handle for the PCM device */ 
    //snd_pcm_t *pcm_handle;          

    /* Playback stream */
    snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;

    /* This structure contains information about    */
    /* the hardware and can be used to specify the  */      
    /* configuration to be used for the PCM stream. */ 
    snd_pcm_hw_params_t *hwparams;

    /* Name of the PCM device, like plughw:0,0          */
    /* The first number is the number of the soundcard, */
    /* the second number is the number of the device.   */
    char *pcm_name = strdup("plughw:0,0");
	WarningLogMessage("TODO [%s:%d]: pcm_name need to be configurable.", __FILE__, __LINE__);
  
    /* Allocate the snd_pcm_hw_params_t structure on the stack. */
    snd_pcm_hw_params_alloca(&hwparams);

    /* Open PCM. The last parameter of this function is the mode. */
    /* If this is set to 0, the standard mode is used. Possible   */
    /* other values are SND_PCM_NONBLOCK and SND_PCM_ASYNC.       */ 
    /* If SND_PCM_NONBLOCK is used, read / write access to the    */
    /* PCM device will return immediately. If SND_PCM_ASYNC is    */
    /* specified, SIGIO will be emitted whenever a period has     */
    /* been completely processed by the soundcard.                */
    if(err = snd_pcm_open(&pcm_handle, pcm_name, stream, 0) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error opening PCM device %s (Error: %s)", pcm_name, snd_strerror(err));
        return 1;
    }

    /* Init hwparams with full configuration space */
    if(err = snd_pcm_hw_params_any(pcm_handle, hwparams) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Can not configure PCM device %s. (Error: %s)", pcm_name, snd_strerror(err));
        return 1;
    }

    /* Set access type. This can be either    */
    /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
    /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
    /* There are also access types for MMAPed */
    /* access, but this is beyond the scope   */
    /* of this introduction.                  */
    if(err = snd_pcm_hw_params_set_access(pcm_handle, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting access on device %s. (Error: %s)", pcm_name, snd_strerror(err));
        return 1;
    }
  
    /* Set sample format */ 
    if(err = snd_pcm_hw_params_set_format(pcm_handle, hwparams, format) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting format %s on device %s. (Error: %s)", snd_pcm_format_name(format), pcm_name, snd_strerror(err));
        return 1;
    }
    
    /* Set sample rate. If the exact rate is not supported */
    /* by the hardware, use nearest possible rate.         */ 
#if 0 // Segfault... Humf... TODO
    exact_rate = rate;
    if(snd_pcm_hw_params_set_rate_near(pcm_handle, hwparams, &exact_rate, &dir) < 0)
    {
        ErrorLogMessage("Error setting rate (%d) on device %s.", rate, pcm_name);
    }
    if(dir != 0) {
        WarningLogMessage("The rate %d Hz is not supported by your hardware on device %s.\n" \
                "==> Using %d Hz instead.", pcm_name, rate, exact_rate);
    }
#else
	exact_rate = rate;
    if(err = snd_pcm_hw_params_set_rate(pcm_handle, hwparams, exact_rate, 0) < 0)
    {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting rate (%d) on device %s. (Error: %s)", rate, pcm_name, snd_strerror(err));
    }
#endif
    
    /* Set number of channels */
    if(err = snd_pcm_hw_params_set_channels(pcm_handle, hwparams, channels) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting channels (%d) on device %s. (Error: %s)", channels, pcm_name, snd_strerror(err));
        return 1;
    }

    /* Set number of periods. Periods used to be called fragments. */ 
    if(err = snd_pcm_hw_params_set_periods(pcm_handle, hwparams, periods, 0) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting periods (%d) on device %s. (Error: %s)", periods, pcm_name, snd_strerror(err));
        return 1;
    }

    /* Set buffer size (in frames). The resulting latency is given by */
    /* latency = periodsize * periods / (rate * bytes_per_frame)     */
    int buffer_size = (periodsize * periods) >> 2;
    if(err = snd_pcm_hw_params_set_buffer_size(pcm_handle, hwparams, buffer_size) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting buffersize (%d) on device %s. (Error: %s)", buffer_size, pcm_name, snd_strerror(err));
        return 1;
    }
    
    /* Apply HW parameter settings to */
    /* PCM device and prepare device  */
    if(err = snd_pcm_hw_params(pcm_handle, hwparams) < 0) {
        ErrorLogMessage("[ALSA Audio Plugin] Error setting HW params on device %s. (Error: %s)", pcm_name, snd_strerror(err));
        return 1;
    }

	sndBuffer = new uint8_t[periodsize*4*2];
	if(!sndBuffer)
	{
		ErrorLogMessage("[ALSA Audio Plugin] sndBuffer allocation error.");
		return 1;
	}
    sndBufferPtr = sndBuffer;

	InfoLogMessage("[ALSA Audio Plugin] PCM device %s open at %d Hz %s, %d channels.", pcm_name, rate, snd_pcm_format_name(format), channels);

    return 0;
}
