#pragma once

#if defined(BOARD_ESP32S3_DEVKITC)

// TFT
#define TFT_MISO 38
#define TFT_MOSI 18
#define TFT_SCLK 21
#define TFT_CS 15
#define TFT_DC 17
#define TFT_RST 16
#define TFT_BL 46

// Sensors / actuators
#define SDA_PIN 8
#define SCL_PIN 9
#define SERVO_VALVE_PIN 4
#define PUMP_1_PIN 5
#define PUMP_2_PIN 6
#define GEAR_MOTOR_PIN 7
#define TRIG_1_PIN 10
#define ECHO_1_PIN 11
#define TRIG_2_PIN 12
#define ECHO_2_PIN 13
#define DQ_PIN 14

#elif defined(BOARD_ESP32_DEVKIT)

// TFT
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS 5
#define TFT_DC 2
#define TFT_RST 4
#define TFT_BL -1

// Sensors / actuators
#define SDA_PIN 21
#define SCL_PIN 22
#define SERVO_VALVE_PIN 16
#define PUMP_1_PIN 27
#define PUMP_2_PIN 14
#define GEAR_MOTOR_PIN 13
#define TRIG_1_PIN 25
#define ECHO_1_PIN 26
#define TRIG_2_PIN 32
#define ECHO_2_PIN 33
#define DQ_PIN 17

#else
#error "Unsupported board target"
#endif