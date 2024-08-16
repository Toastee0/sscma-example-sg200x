#include <thread>

#include "liveMedia.hh"
#include "H264LiveVideoServerMediaSubsession.hh"

#include "announceURL.hh"

#include "defs.hh"

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

int getFileSize(FILE* fp, const char* fileName) {
    int fileSize = 0;

    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    return fileSize;
}

int readFrame(FILE* fp, uint8_t* ptr) {
    int size = 0;
    static uint8_t hd[4] = { 0 };
    
    if (fread(ptr, 1, 4, fp) < 4) {
        printf("read start error\n");
        return size;
    }

    if (ptr[0] == 0x00 && ptr[1] == 0x00 && ptr[2] == 0x00 && ptr[3] == 0x01) {
        size = 4;
        while (1) {
            if (fread(ptr + size, 1, 1, fp)) {
                hd[0] = hd[1];
                hd[1] = hd[2];
                hd[2] = hd[3];
                hd[3] = ptr[size];
                size++;
                if (hd[0] == 0x00 && hd[1] == 0x00 && hd[2] == 0x00 && hd[3] == 0x01) {
                    size -= 4;
                    fseek(fp, -4, SEEK_CUR);
                    break;
                }
            } else {
                break;
            }
         }
    }

    return size;
}

int main(int argc, char** argv) {
    const int MAX_SIZE = 150000;
    const char* fileName = "test.264";
    FILE* fp = NULL;
    int fileSize = 0, curSize = 0;

    std::thread th(initRtsp);

    fp = fopen(fileName, "rb");
    if (fp == NULL) {
        printf("open %s file failed here\n", fileName);
        return 1;
    }
    fileSize = getFileSize(fp, fileName);
    printf("%s file size: %d\n", fileName, fileSize);

    uint8_t* buf = (uint8_t* ) malloc(MAX_SIZE);
    while (1) {
        if (session == NULL || !session->isStart()) {
            printf("not start now\n");
            sleep(1);
            continue;
        }

        if (curSize < fileSize) {
            int tmp = readFrame(fp, buf);
            usleep(30 * 1000);
            if (tmp) {
                curSize += tmp;
                session->writeData(buf, tmp);
            }
        } else {
            curSize = 0;
            fseek(fp, 0L, SEEK_SET);
            printf("restart to read file[%d - %d]\n", curSize, fileSize);
        }
    }

    return 0;
}
