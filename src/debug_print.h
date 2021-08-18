#ifndef _DEBUGPRINT_H
#define _DEBUGPRINT_H

#ifdef DEBUG
#define Debug(x) Serial.print(x)
#define Debugln(x) Serial.println(x)
#define Debug64(x) Serial.print(uint32_t(x / 100000)); Serial.print(' '); Serial.print(uint32_t(x % 100000));
#define Debug64ln(x) Serial.print(uint32_t(x / 100000)); Serial.print(' ');Serial.println(uint32_t(x % 100000));
#else
#define Debug(x)
#define Debugln(x)
#define Debug64(x)
#define Debug64ln(x)
#endif

#endif //_DEBUGPRINT_H
