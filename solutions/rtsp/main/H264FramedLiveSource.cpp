#include "H264FramedLiveSource.hh"

H264FramedLiveSource* H264FramedLiveSource::createNew(UsageEnvironment& env) {
    return new H264FramedLiveSource(env);
}

H264FramedLiveSource::H264FramedLiveSource(UsageEnvironment& env)
    : FramedSource(env) {
    _eventTriggerId = envir().taskScheduler().createEventTrigger(deliverFrame0);
}

H264FramedLiveSource::~H264FramedLiveSource() {
    envir().taskScheduler().deleteEventTrigger(_eventTriggerId);
    _eventTriggerId = 0;
}

void H264FramedLiveSource::doGetNextFrame() {

    printf("[chenl] (H264FramedLiveSource::%s)::%d - do to\n", __func__, __LINE__);

    return;
}

void H264FramedLiveSource::deliverFrame0(void* clientData) {
    ((H264FramedLiveSource *) clientData)->deliverFrame();
}

void H264FramedLiveSource::deliverFrame() {
    if (!isCurrentlyAwaitingData()) {
        return;
    }

    u_int8_t* newFrameDataStart = _buf;
    unsigned newFrameSize = _bufSize;

    if (newFrameSize > fMaxSize) {
        fFrameSize = fMaxSize;
        fNumTruncatedBytes = newFrameSize - fMaxSize;
    } else {
        fFrameSize = newFrameSize;
    }

    gettimeofday(&fPresentationTime, NULL);

    memmove(fTo, newFrameDataStart, fFrameSize);

    if (fNumTruncatedBytes) {
        _buf = _buf + fFrameSize;
        _bufSize -= fFrameSize;
        fNumTruncatedBytes = 0;
    } else {
        _bufSize = 0;
    }
    
    FramedSource::afterGetting(this);
}

EventTriggerId H264FramedLiveSource::getEventTriggerId() const {
    return _eventTriggerId;
}

void H264FramedLiveSource::setBuf(u_int8_t* buf, unsigned size) {
    if (size > 4 && buf[0] == 0 && buf[1] == 0 && buf[2] == 0 && buf[3] == 1) {
        _bufSize = size - 4;
        _buf = buf + 4;
    } else {
        _bufSize = size;
        _buf = buf;
    }
}

unsigned H264FramedLiveSource::getBufSize() {
    return _bufSize;
}

void SignalNewFrameData(TaskScheduler* task, H264FramedLiveSource* src) {
    if (task != NULL && src != NULL) {
        task->triggerEvent(src->getEventTriggerId(), src);
    }
}
