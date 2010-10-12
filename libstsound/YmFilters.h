#ifndef __FILTER_H_
#define __FILTER_H_

#include "YmTypes.h"

static const ymint DC_ADJUST_BUFFERLEN = 512;

class Filter
{
    public:
        Filter();
        virtual void AddSample(ymint sample) = 0;
        virtual ymint GetResult() = 0;
        virtual void Reset() = 0;
};

class SimpleLowPassFilter: public Filter
{
    public:
        SimpleLowPassFilter();
        void AddSample(ymint sample);
        ymint GetResult();
        void Reset();

    private:
        ymint delay_line[3]; // Delay lines for low pass filtering. 3 samples.
};

class DCRemover: public Filter
{
    public:
        DCRemover();
        void AddSample(ymint sample);
        ymint GetResult();
        void Reset();

    private:
        ymint buffer[DC_ADJUST_BUFFERLEN];
        ymint pos;
        ymint sum;
#ifndef YM_SIMPLIFIED_FILTER
        bool full;
#endif
};

#if 0
class FIRFilter: public Filter
{
    public:
        FIRFilter();
        FIRFilter(ymint N = 512, ymint delay = 0, ymint *coef);
        ~FIRFilter();
        void AddSample(ymint sample);
        ymint GetResult();
        void Reset();

    privte:
        ymint N; // N-tap 
        ymint delay;
        ymint pos;
        ymint *buffer;
        ymint *delay_line;
        ymfloat *coef;
        ymfloat coef_sum;
}
#endif // Unfinished

#endif /* #ifndef __FILTER_H_ */
