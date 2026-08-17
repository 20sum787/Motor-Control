
#include "CytronMotorDriver.h"
#include <ESP32Encoder.h>

// Create an encoder instance
ESP32Encoder encoder;
// set desired motor speed from 20-170
int MOTOR_SPEED = 20;
// set target position (in degrees from 0-90)
int TARGET_POSITION = 900;

// Define encoder pins
const int ENCODER_A_PIN = 6;
const int ENCODER_B_PIN = 7;

// Cytron specifications (encoder resolution)
const float ENCODER_PPR = 3895.9; 

// Timing variables for RPM calculation
unsigned long lastTime = 0;
long lastPosition = 0;

CytronMD motor(PWM_DIR, 46, 8);  // PWM = Pin 46, DIR = Pin 8.

void setup() {
  
  Serial.begin(115200);


  // Enable internal weak pull-up resistors for the encoder channels
  ESP32Encoder::useInternalWeakPullResistors = puType::up;

  // Attach the pins for full quadrature (4X counting)
  encoder.attachFullQuad(ENCODER_A_PIN, ENCODER_B_PIN);
  
  // Clear the hardware counter buffer to start at 0
  encoder.clearCount();

}


void loop() {
  // put your main code here, to run repeatedly:

motor.setSpeed(MOTOR_SPEED);

unsigned long currentTime = millis();

unsigned long TimeInterval = currentTime - lastTime;
  
  // Calculate updates every 50 milliseconds
  if (TimeInterval >= 20) {
    long currentPosition = encoder.getCount();

    // calculate degrees rotated
    float degrees = (float)currentPosition*360 / ENCODER_PPR;
  
    // Calculate rotational speed (RPM)
    long deltaTicks = currentPosition - lastPosition;
    float timeElapsedMinutes = (float)(currentTime - lastTime) / 60000.0;
    float currentRPM = ((float)deltaTicks / ENCODER_PPR) / timeElapsedMinutes;

    // Display output data
    Serial.print(" Time interval: ");
    Serial.print(TimeInterval, 4);
    Serial.print(" | Degrees rotated: ");
    Serial.print(degrees, 5);
    Serial.print(" | Speed: ");
    Serial.print(currentRPM, 1);
    Serial.println(" RPM");

    // Save current states for the next calculation block
    lastPosition = currentPosition;
    lastTime = currentTime;

    float upper_bound = TARGET_POSITION + 0.1;
    float lower_bound = TARGET_POSITION - 0.1;

    if (degrees >= upper_bound) {
      MOTOR_SPEED = -20;
    }
    else if (degrees <= lower_bound) {
      MOTOR_SPEED = 20;

    }

    else {
      MOTOR_SPEED = 0;
    }
  }
  
}
