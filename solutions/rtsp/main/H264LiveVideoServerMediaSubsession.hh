#ifndef _H264_LIVE_VIDEO_SERVER_MEDIA_SUBSESSION_HH
#define _H264_LIVE_VIDEO_SERVER_MEDIA_SUBSESSION_HH

#include "OnDemandServerMediaSubsession.hh"
#include "H264FramedLiveSource.hh"
#include "H264VideoStreamDiscreteFramer.hh"
#include "H264VideoRTPSink.hh"

class H264LiveVideoServerMediaSubsession: public OnDemandServerMediaSubsession {
public:
    static H264LiveVideoServerMediaSubsession* createNew(UsageEnvironment& env, Boolean reuseFirstSource);

protected:
    H264LiveVideoServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource);
    ~H264LiveVideoServerMediaSubsession();

protected:
    FramedSource* createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate);
    RTPSink* createNewRTPSink(Groupsock* rtpGroupsock, unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource);
};


#endif
