#include "H264LiveVideoServerMediaSubsession.hh"
// #include "H264VideoStreamFramer.hh"

#include <stdio.h>

H264LiveVideoServerMediaSubsession* H264LiveVideoServerMediaSubsession::createNew(UsageEnvironment& env, Boolean reuseFirstSource) {
    return new H264LiveVideoServerMediaSubsession(env, reuseFirstSource);
}

H264LiveVideoServerMediaSubsession::H264LiveVideoServerMediaSubsession(UsageEnvironment& env, Boolean reuseFirstSource)
    : OnDemandServerMediaSubsession(env, reuseFirstSource) {
    printf("[chenl] (H264LiveVideoServerMediaSubsession::%s) init done here\n", __func__);
}

H264LiveVideoServerMediaSubsession::~H264LiveVideoServerMediaSubsession() {
}

FramedSource* H264LiveVideoServerMediaSubsession::createNewStreamSource(unsigned clientSessionId, unsigned& estBitrate) {
    printf("[chenl] (H264LiveVideoServerMediaSubsession::%s) init done here\n", __func__);
    H264FramedLiveSource* liveSource = H264FramedLiveSource::createNew(envir());
    if (liveSource == NULL) {
        return NULL;
    }

    // return H264VideoStreamFramer::createNew(envir(), liveSource);
    return H264VideoStreamDiscreteFramer::createNew(envir(), liveSource); // no HAL header
}

RTPSink* H264LiveVideoServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
        unsigned char rtpPayloadTypeIfDynamic, FramedSource* inputSource) {
    printf("[chenl] (H264LiveVideoServerMediaSubsession::%s) init done here\n", __func__);
    return H264VideoRTPSink::createNew(envir(), rtpGroupsock, rtpPayloadTypeIfDynamic);
}
