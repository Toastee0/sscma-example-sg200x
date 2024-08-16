#ifndef _DEFS_H_
#define _DEFS_H_

#include "BasicUsageEnvironment.hh"
#include "RTSPServer.hh"

struct RTSP_SERVER_CTX_T {
    TaskScheduler* scheduler;
    UsageEnvironment* env;
    RTSPServer* rtspServer;
    char volatile status;
};

#endif
