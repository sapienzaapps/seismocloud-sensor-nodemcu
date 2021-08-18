#include <Arduino.h>
#include <unity.h>
#include "api.h"

void test_config_parse(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "rpiport=123\nrpiaddress=pippo";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(123, rpiPort);
  TEST_ASSERT_EQUAL_STRING("pippo", rpiHostName.c_str());
}

void test_config_parse_ignore_bad_port(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "rpiport=1111123\nrpiaddress=pippo";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("pippo", rpiHostName.c_str());
}

void test_config_parse_ignore_line(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "rpiport123\nrpiaddress=pippo";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("pippo", rpiHostName.c_str());
}

void test_config_parse_ignore_all(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "rpiport123rpiaddresspippo";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());
}

void test_config_parse_malformed(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "rpipor=t123rpiadd=resspippo";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());

  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  testConfig = "rpipor\nt123rpiadd\nresspippo";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());

  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  testConfig = "rpiport=asd\n";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());
}

void test_config_parse_malformed_empty(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());

  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  testConfig = "\n";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());

  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  testConfig = "\n\n\n\n";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());
}

void test_config_parse_malformed2(void) {
  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  String testConfig = "=\n";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());

  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  testConfig = "=\n=";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());

  // Restore default
  rpiPort = 80;
  rpiHostName = "";

  testConfig = "=\n=\n=\n\n";
  parseConfig(testConfig.c_str());
  TEST_ASSERT_EQUAL_UINT16(80, rpiPort);
  TEST_ASSERT_EQUAL_STRING("", rpiHostName.c_str());
}

void setup() {
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);

  UNITY_BEGIN();    // IMPORTANT LINE!
}

uint16_t i = 0;

void loop() {
  RUN_TEST(test_config_parse);
  RUN_TEST(test_config_parse_ignore_bad_port);
  RUN_TEST(test_config_parse_ignore_line);
  RUN_TEST(test_config_parse_ignore_all);
  RUN_TEST(test_config_parse_malformed);
  RUN_TEST(test_config_parse_malformed_empty);
  RUN_TEST(test_config_parse_malformed2);

  i++;

  if (i > 10000) {
    UNITY_END(); // stop unit testing
    while(true);
  }
}
