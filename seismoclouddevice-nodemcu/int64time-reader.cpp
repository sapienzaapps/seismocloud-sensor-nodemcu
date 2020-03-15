#include "common.h"
#include "int64time-reader.h"

int readNTPString(char* buf, NTPTimeSync* ts) {
  int i = 0;

  // This function assume that the number won't overflow...
  // TODO: make some checks if the number overflows or fields are missing

  ts->t0 = 0;
  for (; i < strlen(buf) && buf[i] != ';'; i++) {
    char c = buf[i];
    ts->t0 *= 10ULL;
    ts->t0 += (uint64)(c-0x30ULL);
  }

  i++;

  ts->t1 = 0;
  for (; i < strlen(buf) && buf[i] != ';'; i++) {
    char c = buf[i];
    ts->t1 *= 10ULL;
    ts->t1 += (uint64)(c-0x30ULL);
  }

  i++;

  ts->t2 = 0;
  for (; i < strlen(buf) && buf[i] != ';'; i++) {
    char c = buf[i];
    ts->t2 *= 10ULL;
    ts->t2 += (uint64)(c-0x30ULL);
  }

  Debug("t0: ");
  Debug64ln(ts->t0);
  Debug("t1: ");
  Debug64ln(ts->t1);
  Debug("t2: ");
  Debug64ln(ts->t2);
  Debug("t3: ");
  Debug64ln(ts->t3);

  return 0;
}

uint64 syncCurrentTime(uint64_t lastNTPTime, NTPTimeSync* ts) {
  if (lastNTPTime == 0 || ts->t0 == 0) {
    // NTP can't handle 34 or 68 years differences, so if we start from 0 then return t2
    return ts->t2;
  }
  int64_t a = (int64_t)ts->t1 - (int64_t)ts->t0;
  int64_t b = (int64_t)ts->t2 - (int64_t)ts->t3;
  int64_t theta = (a + b)/2LL;
  int64_t sigma = ((int64_t)ts->t3 - (int64_t)ts->t0) - ((int64_t)ts->t2 - (int64_t)ts->t1);

  Debug("lastNTPTime: ");
  Debug64ln(lastNTPTime);

  Debug("a: ");
  Debug64ln(a);
  Debug("b: ");
  Debug64ln(b);
  Debug("Theta: ");
  Debug64ln(theta);

  return (uint64_t)((int64_t)lastNTPTime + theta + sigma/2LL);
}
