#include "ma_transport_rtsp.h"

H264LiveVideoServerMediaSubsession* session = NULL;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName) {
    UsageEnvironment& env = rtspServer->envir();

    env << "\n\"" << streamName << "\" stream, from the file \""
        << inputFileName << "\"\n";
    announceURL(rtspServer, sms);
}

int createRtsp(RTSP_SERVER_CTX_T &rtspCtx, int port, int bufSize = 600000) {
    rtspCtx.scheduler = BasicTaskScheduler::createNew();
    rtspCtx.env = BasicUsageEnvironment::createNew(*rtspCtx.scheduler);
    rtspCtx.rtspServer = RTSPServer::createNew(*rtspCtx.env, port);

    if (rtspCtx.rtspServer == NULL) {
        *rtspCtx.env << "Failed to create RTSP server: " << rtspCtx.env->getResultMsg() << "\n";
        return 1;
    }

    OutPacketBuffer::maxSize = bufSize;
    rtspCtx.status = 0;

    return 0;
}

void createLiveSession(RTSP_SERVER_CTX_T &rtspCtx, const char* streamName, Boolean reuseFirstSource) {
    char const* descriptionString = "Session streamed by \"rtspServer\"";
    UsageEnvironment* env = rtspCtx.env;
    RTSPServer* rtspServer = rtspCtx.rtspServer;

    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    session = H264LiveVideoServerMediaSubsession::createNew(*env, reuseFirstSource);
    sms->addSubsession(session);
    rtspServer->addServerMediaSession(sms);

    rtspServer->envir() << "\n\"" << streamName << "\" stream, read frames from a file\n";
    announceURL(rtspServer, sms);
}

void createH264FileSession(RTSP_SERVER_CTX_T &rtspCtx, const char* streamName, const char* fileName) {
    char const* descriptionString = "Session streamed by \"rtspServer\"";
    UsageEnvironment* env = rtspCtx.env;
    RTSPServer* rtspServer = rtspCtx.rtspServer;

    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, fileName, False));
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName, fileName);
}

void initRtsp() {
    int ret = 0;
    RTSP_SERVER_CTX_T rtspCtx;

    ret = createRtsp(rtspCtx, 8555);
    if (ret) return;

    createLiveSession(rtspCtx, "live", True);
    createH264FileSession(rtspCtx, "h264", "test.264");

    rtspCtx.env->taskScheduler().doEventLoop(&(rtspCtx.status));
}


namespace ma {

TransportRTSP::TransportRTSP() : Transport(MA_TRANSPORT_MQTT) {
    std::thread th(initRtsp);
    th.detach();
}

TransportRTSP::~TransportRTSP() {
}

TransportRTSP::operator bool() const {
    return 1;
}

size_t TransportRTSP::available() const {
    return 0;
}

size_t TransportRTSP::send(const char* data, size_t length, int timeout) {
    session->writeData((uint8_t *) data, length);
    return 0;
}

size_t TransportRTSP::receive(char* data, size_t length, int timeout) {
    return 0;
}

size_t TransportRTSP::receiveUtil(char* data, size_t length, char delimiter, int timeout) {
    return 0;
}

} // namespace ma
