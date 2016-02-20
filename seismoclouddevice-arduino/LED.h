//
// Created by enrico on 09/07/15.
//

#ifndef GALILEO_TERREMOTI_LED_H
#define GALILEO_TERREMOTI_LED_H

#include <stdint.h>

class LED {
public:
  /**
   * Init LED class with led pins
   * @param greenLedPin Green LED pin number
   * @param yellowLedPin Yellow LED pin number
   * @param redLedPin Red LED pin number
   */
  static void init(uint8_t greenLedPin, uint8_t yellowLedPin, uint8_t redLedPin);

  /**
   * On/Off GREEN LED
   * @param s True will set LED on, false will set LED off
   */
  static void green(bool s);

  /**
   * On/Off RED LED
   * @param s True will set LED on, false will set LED off
   */
  static void red(bool s);

  /**
   * On/Off YELLOW LED
   * @param s True will set LED on, false will set LED off
   */
  static void yellow(bool s);

  /**
   * Startup blink procedure
   */
  static void startupBlink();

  /**
   * LED tick (if threads are not available)
   */
  static void tick();

  /**
   * Check if LED animation is running
   * @return True if LED animation is running, false otherwise
   */
  static bool getLedAnimation();

  /**
   * Clear all resources
   */
  static void dispose();

  /**
   * Blink a LED
   * @param pin PIN number
   */
  static void setLedBlinking(uint8_t pin);

  /**
   * Clear LED blinking status
   */
  static void clearLedBlinking();

  /**
   * Get PIN status
   * @param pin LED PIN number
   * @return True if LED is ON, false if LED is OFF
   */
  static bool get(uint8_t pin);
private:
  static uint8_t greenLedPin;
  static uint8_t yellowLedPin;
  static uint8_t redLedPin;
  static bool ledAnimation;
  static uint8_t blinkingLed;
  static unsigned long lastBlink;
  static bool lastBlinkStatus;
};


#endif //GALILEO_TERREMOTI_LED_H

