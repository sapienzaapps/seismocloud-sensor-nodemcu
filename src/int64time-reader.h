#ifndef __INT64_READER
#define __INT64_READER

typedef struct {
  uint64_t t0;
  uint64_t t1;
  uint64_t t2;
  uint64_t t3;
} NTPTimeSync;

int readNTPString(char* buf, NTPTimeSync* ts);

uint64 syncCurrentTime(uint64 lastNTPTime, NTPTimeSync* ts);

#endif
