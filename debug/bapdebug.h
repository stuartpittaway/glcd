
//#define BAPDEBUG_WRITE
//#define BAPDEBUG_WRITECMD
//#define BAPDEBUG_READ
//#define BAPDEBUG_BUSY
//#define BAPDEBUG_GOTO
//#define BAPDEBUG_SELECT

#define BAPDEBUG_BADVALUES



extern "C" {
#include <inttypes.h>
#include <stdio.h>      // BAPDEBUG
}

#include "HardwareSerial.h"     // BAPDEBUG
static char buf[64];
#define printf(...)                                             \
        do {                                                    \
                sprintf(buf, __VA_ARGS__); Serial.write(buf);   \
        } while (0)


#define kbwait() \
        do {                                                    \
                printf("Press any key to contiue....");         \
                Serial.flush();                                 \
                while(Serial.available() == 0)                  \
					;											\
                Serial.flush();                                 \
                printf("\n");                                   \
        } while (0)

