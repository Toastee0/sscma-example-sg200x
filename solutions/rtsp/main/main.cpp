#include <thread>

#include "BasicUsageEnvironment.hh"
#include "liveMedia.hh"
#include "H264LiveVideoServerMediaSubsession.hh"

#include "announceURL.hh"

UsageEnvironment* env;

// To make the second and subsequent client for each stream reuse the same
// input stream as the first client (rather than playing the file from the
// start for each client), change the following "False" to "True":
Boolean reuseFirstSource = False;

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName); // forward

int main(int argc, char** argv) {
  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  RTSPServer* rtspServer = RTSPServer::createNew(*env, 8555);
  if (rtspServer == NULL) {
    *env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
    exit(1);
  }

  char const* descriptionString = "Session streamed by \"rtspServer\"";

  OutPacketBuffer::maxSize = 300000;
  {
    char const* streamName = "live";
    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    sms->addSubsession(H264LiveVideoServerMediaSubsession::createNew(*env, reuseFirstSource));
    rtspServer->addServerMediaSession(sms);

    rtspServer->envir() << "\n" << streamName << " start here\n";
    announceURL(rtspServer, sms);
  }

  {
    char const* streamName = "h264";
    char const* inputFileName = "test.264";
    ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName, descriptionString);
    sms->addSubsession(H264VideoFileServerMediaSubsession::createNew(*env, inputFileName, reuseFirstSource));
    rtspServer->addServerMediaSession(sms);

    announceStream(rtspServer, sms, streamName, inputFileName);
  }

  env->taskScheduler().doEventLoop(); // does not return

  return 0;
}

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,
			   char const* streamName, char const* inputFileName) {
  UsageEnvironment& env = rtspServer->envir();

  env << "\n\"" << streamName << "\" stream, from the file \""
      << inputFileName << "\"\n";
  announceURL(rtspServer, sms);
}
