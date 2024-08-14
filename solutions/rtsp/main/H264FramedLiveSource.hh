#ifndef _H264_FRAMED_LIVE_SOURCE_HH
#define _H264_FRAMED_LIVE_SOURCE_HH

#include "UsageEnvironment.hh"
#include "FramedSource.hh"

class H264FramedLiveSource: public FramedSource {
public:
    static H264FramedLiveSource* createNew(UsageEnvironment& env);
    virtual ~H264FramedLiveSource();
    EventTriggerId getEventTriggerId() const;
    void setBuf(u_int8_t* buf, unsigned size);
    unsigned getBufSize();

protected:
    H264FramedLiveSource(UsageEnvironment& env);

private:
    virtual void doGetNextFrame();

private:
    static void deliverFrame0(void* clientData);
    void deliverFrame();

private:
    EventTriggerId _eventTriggerId;
    u_int8_t* _buf;
    unsigned _bufSize;
};

#endif
