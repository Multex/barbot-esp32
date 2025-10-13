#include <Arduino.h>
#include <unity.h>
#include <ArduinoJson.h>

void setUp() {}
void tearDown() {}

void test_parses_simple_recipe() {
  const char* json = R"({
    "VodkaCola": [
      { "disp": 2, "oz": 1 },
      { "disp": 1, "oz": 2 }
    ]
  })";

  JsonDocument doc;
  auto err = deserializeJson(doc, json);
  TEST_ASSERT_FALSE_MESSAGE(err, "JSON should parse without error");

  int first_disp = doc["VodkaCola"][0]["disp"].as<int>();
  int second_oz = doc["VodkaCola"][1]["oz"].as<int>();
  TEST_ASSERT_EQUAL_INT(2, first_disp);
  TEST_ASSERT_EQUAL_INT(2, second_oz);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_parses_simple_recipe);
  UNITY_END();
}

void loop() {}

