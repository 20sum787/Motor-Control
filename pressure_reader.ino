// Heavily based off code from my project lead (to read pressure sensor data), and my own motor code
// This file is a work-in-progress for the final PID control script

#include "CytronMotorDriver.h"
#include <ESP32Encoder.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>
// Create an encoder instance
ESP32Encoder encoder;
// set desired motor speed from 20-170
const int MOTOR_SPEED = 20;
// set target position (in degrees)
int TARGET_POSITION = 540;

// Define encoder pins
const int ENCODER_A_PIN = 6;
const int ENCODER_B_PIN = 7;

// Cytron specifications (encoder resolution)
const float ENCODER_PPR = 3895.9; 

// Timing variables for RPM calculation
unsigned long lastTime = 0;
long lastPosition = 0;

CytronMD motor(PWM_DIR, 46, 8);  // PWM = Pin 46, DIR = Pin 8.

bool systemRunning = false; // Starts safely turned OFF

#define I2C_SDA_PIN 1
#define I2C_SCL_PIN 2
#define ADS1115_ADDR 0x48

// ADS1115 analogue input channels in use.
#define ADC_CH_PRESSURE_A3 3   // ADS1115 A3
#define ADC_CH_PRESSURE_A1 1   // ADS1115 A1

// Measured resistor values.
// Circuit: sensor signal -> R_TOP -> ADS input node -> R_BOTTOM -> GND
const float R_TOP_OHMS_A3    = 4300.0f;
const float R_BOTTOM_OHMS_A3 = 8160.0f;

const float R_TOP_OHMS_A1    = 4310.0f;
const float R_BOTTOM_OHMS_A1 = 8300.0f;

const float DIVIDER_RATIO_A3 = R_BOTTOM_OHMS_A3 / (R_TOP_OHMS_A3 + R_BOTTOM_OHMS_A3);
const float DIVIDER_RATIO_A1 = R_BOTTOM_OHMS_A1 / (R_TOP_OHMS_A1 + R_BOTTOM_OHMS_A1);

// Typical 0.5-4.5 V pressure transducer mapping.
// Change these if your sensor is different.
const float SENSOR_MIN_V = 0.5f;
const float SENSOR_MAX_V = 4.5f;
const float SENSOR_FULL_SCALE_PSI = 150.0f;

Adafruit_ADS1115 ads;
bool ads_ok = false;
unsigned long last_retry_ms = 0;

bool initialiseADS1115() {
  Serial.println("Initialising ADS1115...");

  // GAIN_ONE gives +/-4.096 V full-scale.
  // With ADS1115 powered from 3.3 V, keep the physical input below 3.3 V.
  ads.setGain(GAIN_ONE);

  if (!ads.begin(ADS1115_ADDR, &Wire)) {
    Serial.println("ADS1115 not found at 0x48. Check VDD, GND, SDA, SCL, ADDR.");
    return false;
  }

  Serial.println("ADS1115 found at 0x48.");
  return true;
}

float readAdcVoltage(uint8_t channel) {
  int16_t raw = ads.readADC_SingleEnded(channel);
  return ads.computeVolts(raw);
}

float dividerToSensorVoltage(float adc_voltage, float divider_ratio) {
  return adc_voltage / divider_ratio;
}

float sensorVoltageToPsi(float sensor_voltage) {
  float psi = (sensor_voltage - SENSOR_MIN_V) *
              SENSOR_FULL_SCALE_PSI /
              (SENSOR_MAX_V - SENSOR_MIN_V);

  // Small negative values are normal near zero because of sensor offset/noise.
  if (psi < 0.0f) psi = 0.0f;
  return psi;
}


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
