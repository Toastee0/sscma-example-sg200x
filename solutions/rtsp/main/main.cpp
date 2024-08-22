#include "ma_transport_rtsp.h"

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

    ma::TransportRTSP rtsp;

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
                rtsp.send((const char *) buf, tmp);
                // session->writeData(buf, tmp);
            }
        } else {
            curSize = 0;
            fseek(fp, 0L, SEEK_SET);
            printf("restart to read file[%d - %d]\n", curSize, fileSize);
        }
    }

    return 0;
}
