
#ifndef __LED_H
#define __LED_H

#ifdef NO_LEDS
// Disable LED functions
#define LED_init()
#define LED(green, yellow, red)
#define LED_startup_blink()
#else

#define LED_RED     D7
#define LED_YELLOW  D6
#define LED_GREEN   D5

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

#endif

// Boot sequence LEDs
#define LED_accel_calibr()     LED(LOW , LOW , HIGH)
#define LED_wait_net_cfg()     LED(LOW , HIGH, LOW )
#define LED_update()           LED(LOW , HIGH, HIGH)
#define LED_connection()       LED(HIGH, HIGH, HIGH)
#define LED_ready()            LED(HIGH, LOW , LOW )
#define LED_lost_api()         LED(HIGH, HIGH, LOW)

#endif // __LED_H