
#ifndef GALILEO_TERREMOTI_LED_H
#define GALILEO_TERREMOTI_LED_H

#define LED_init()            \
pinMode(LED_GREEN,  OUTPUT);  \
pinMode(LED_YELLOW, OUTPUT);  \
pinMode(LED_RED,    OUTPUT)

#define LED(green, yellow, red)    \
digitalWrite(LED_GREEN,  green);   \
digitalWrite(LED_YELLOW, yellow);  \
digitalWrite(LED_RED,    red)

// Boot sequence LEDs
#define LED_accel_calibr()     LED(LOW , LOW , HIGH)
#define LED_wait_net_cfg()     LED(LOW , HIGH, LOW )
#define LED_update()           LED(LOW , HIGH, HIGH)
#define LED_connection()       LED(HIGH, HIGH, HIGH)
#define LED_ready()            LED(HIGH, LOW , LOW )

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

