#ifndef _MA_TRANSPORT_RTSP_H
#define _MA_TRANSPORT_RTSP_H

#include <sscma.h>

#include <thread>

#include "liveMedia.hh"
#include "H264LiveVideoServerMediaSubsession.hh"

#include "announceURL.hh"

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"

struct RTSP_SERVER_CTX_T {
    TaskScheduler* scheduler;
    UsageEnvironment* env;
    RTSPServer* rtspServer;
    char volatile status;
};

extern H264LiveVideoServerMediaSubsession* session;

void initRtspServer();

namespace ma {

class TransportRTSP final : public Transport {
public:
    TransportRTSP();
    ~TransportRTSP() override;

    operator bool() const;

    size_t available() const override;
    size_t send(const char* data, size_t length, int timeout = -1) override;
    size_t receive(char* data, size_t length, int timeout = 1) override;
    size_t receiveUtil(char* data, size_t length, char delimiter, int timeout = 1) override;

protected:
    // ma_transport_type_t m_type;
};

}

#endif
