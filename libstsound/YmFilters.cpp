#include "YmFilters.h"
#include <cstring>

Filter::Filter()
{
}

DCRemover::DCRemover()
{
    Reset();
}

void DCRemover::Reset()
{
    memset(buffer, 0, DC_ADJUST_BUFFERLEN*sizeof(ymint));
    pos = DC_ADJUST_BUFFERLEN - 1;
    sum = 0;
    full = 0; // Calculate DC Remove only on the number of sample in buffer, can limits start clicks
}

void DCRemover::AddSample(ymint sample)
{
    pos = (pos+1)&(DC_ADJUST_BUFFERLEN-1);
#ifndef YM_SIMPLIFIED_FILTER
    if(!full && (pos == (DC_ADJUST_BUFFERLEN - 1)))
    {
        full = 1;
    }
#endif
    sum -= buffer[pos];
    sum += sample;

    buffer[pos] = sample;
}

ymint DCRemover::GetResult()
{
#ifdef YM_SIMPLIFIED_FILTER
    return buffer[pos] - (sum / DC_ADJUST_BUFFERLEN);
#else
    if(full)
    {
        return buffer[pos] - (sum / DC_ADJUST_BUFFERLEN);
    }
    else
    {
        return buffer[pos] - (sum / (pos + 1));
    }
#endif
}

SimpleLowPassFilter::SimpleLowPassFilter()
{
    Reset();
}

void SimpleLowPassFilter::Reset()
{
    delay_line[0] = 0;
    delay_line[1] = 0;
    delay_line[2] = 0;
}

void SimpleLowPassFilter::AddSample(ymint sample)
{
    delay_line[0] = delay_line[1];
    delay_line[1] = delay_line[2];
    delay_line[2] = sample;
}

ymint SimpleLowPassFilter::GetResult()
{
    return (delay_line[0] >> 2)
        + (delay_line[1] >> 1)
        + (delay_line[2] >> 2);
}

#if 0
FIRFilter::FIRFilter()
    : N(512), delay(0), buffer(NULL), delay_line(NULL)
{
    Reset();
}

FIRFilter::FIRFilter(ymint N, ymint delay)
    : N(N), delay(delay), buffer(NULL), delay_line(NULL)
{
    Reset();
}

FIRFilter::~FIRFilter()
{
    free(buffer);
    buffer = NULL;
    free(delay_line);
    delay_line = NULL
}

void FIRFilter::Reset()
{
    free(buffer);
    buffer = (ymint*)malloc(N*size(ymint));
    if(buffer == NULL)
    {
        // TODO
    }
    memset(buffer, 0, N*sizeof(ymint));
    pos = N - 1;

    free(delay_line);
    delay_line = (ymint*)malloc(delay*size(ymint));
    if(buffer == NULL)
    {
        // TODO
    }
    memset(delay_line, 0, N*sizeof(ymint));
    delay_pos = delay - 1;
}

void FIRFilter::AddSample(ymint sample)
{
    delay_pos = (++delay_pos) % delay;
    pos = (++pos) % N;
    buffer[pos] = sample;
}

ymint FIRFilter::GetResult()
{
    double sum = 0;

    for(ymint i = 0; i < N; i++)
    {
        const ymint j = (pos + N - i) % N
        sum += coef[i] * buffer[j];
    }

    delay_line[delay_pos] = sum/coef_sum;

    return delay[(delay_pos + 1) % delay];
}
#endif // Unfinish
