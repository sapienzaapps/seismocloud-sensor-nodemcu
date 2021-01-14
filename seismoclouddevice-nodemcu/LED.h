
#ifndef __LED_H
#define __LED_H

#ifdef MODEL_esp01
// Disable LED functions for ESP-01
#define LED_init()
#define LED(green, yellow, red)
#define LED_startup_blink()
#define LED_lost_api()
#define LED_restore_api()
#define LED_quake()
#define LED_quake_end()
#else

#define LED_init()            \
pinMode(LED_GREEN,  OUTPUT);  \
pinMode(LED_YELLOW, OUTPUT);  \
pinMode(LED_RED,    OUTPUT)

#define LED(green, yellow, red)    \
digitalWrite(LED_GREEN,  green);   \
digitalWrite(LED_YELLOW, yellow);  \
digitalWrite(LED_RED,    red)

#define LED_startup_blink()   \
for(int i=0; i < 10; i++) {   \
  LED(HIGH, HIGH, HIGH);      \
  delay(50);                  \
  LED(LOW , LOW , LOW );      \
  delay(50);                  \
}

// Statuses in normal working conditions
#define LED_lost_api()         digitalWrite(LED_YELLOW, HIGH)
#define LED_restore_api()      digitalWrite(LED_YELLOW, LOW)

#define LED_quake()            digitalWrite(LED_RED,    HIGH)
#define LED_quake_end()        digitalWrite(LED_RED,    LOW)

#endif

// Boot sequence LEDs (no need to be platform-dependent as these are aliases
// for LED define
#define LED_accel_calibr()     LED(LOW , LOW , HIGH)
#define LED_wait_net_cfg()     LED(LOW , HIGH, LOW )
#define LED_update()           LED(LOW , HIGH, HIGH)
#define LED_connection()       LED(HIGH, HIGH, HIGH)
#define LED_ready()            LED(HIGH, LOW , LOW )

#endif

