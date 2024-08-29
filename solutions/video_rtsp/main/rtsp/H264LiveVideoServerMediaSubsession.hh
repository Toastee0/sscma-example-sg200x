#ifndef _H264_LIVE_VIDEO_SERVER_MEDIA_SUBSESSION_HH
#define _H264_LIVE_VIDEO_SERVER_MEDIA_SUBSESSION_HH

#include <mutex>
#include <atomic>

#include "OnDemandServerMediaSubsession.hh"
#include "H264FramedLiveSource.hh"
#include "H264VideoStreamDiscreteFramer.hh"
#include "H264VideoRTPSink.hh"

class H264LiveVideoServerMediaSubsession: public OnDemandServerMediaSubsession {
public:
    static H264LiveVideoServerMediaSubsession* createNew(UsageEnvironment& env, Boolean reuseFirstSource);
    bool isStart();
    int writeData(uint8_t* data, uint32_t size);

protected:
    H264LiveVideoServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource);
    ~H264LiveVideoServerMediaSubsession();
    virtual void startStream(unsigned clientSessionId, void* streamToken,
            TaskFunc* rtcpRRHandler,
            void* rtcpRRHandlerClientData,
            unsigned short& rtpSeqNum,
            unsigned& rtpTimestamp,
            ServerRequestAlternativeByteHandler* serverRequestAlternativeByteHandler,
            void* serverRequestAlternativeByteHandlerClientData);

protected:
    FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
    RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);

private:
    H264FramedLiveSource* _liveSource;
    std::atomic<bool> _start;
    std::mutex _mutex;
};


#endif
