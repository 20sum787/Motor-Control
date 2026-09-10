#include "CytronMotorDriver.h"
#include <ESP32Encoder.h>

// Create an encoder instance
ESP32Encoder encoder;
// set desired motor speed from 20-170
const int MOTOR_SPEED = 20;
// set target position (max 720 normally, as 2 full turns maps to open/closed)
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


void setup() {
  Serial.begin(115200);
  // Enable internal weak pull-up resistors for the encoder channels
  ESP32Encoder::useInternalWeakPullResistors = puType::up;
  // Attach the pins for full quadrature (4X counting)
  encoder.attachFullQuad(ENCODER_A_PIN, ENCODER_B_PIN);
  // Clear the hardware counter buffer to start at 0
  encoder.clearCount();

  motor.setSpeed(0);

}

void loop() {

  if (Serial.available() > 0) {
    char incomingByte = Serial.read();

    if (incomingByte == 's') { // 's' for Start
      systemRunning = true;
      motor.setSpeed(MOTOR_SPEED); 
    }

    else if (incomingByte == ' ') {
      
      motor.setSpeed(0);
      
      Serial.println("STOPPED"); // alert that motor is stopped
      while(1); // Freezes the ESP32 safety loop until manual reset 
      // this is quite a non-rigorous method, but since stops would only be carried out in case of emergency, I deemed this acceptable
    }

    else {
      motor.setSpeed(0);
    }
  }


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

    // Save current states for the next calculation 
    lastPosition = currentPosition;
    lastTime = currentTime;

    float upper_bound = TARGET_POSITION + 0.5;
    float lower_bound = TARGET_POSITION - 0.5;

    Serial.print(degrees, 5);      
    Serial.print(",");             
    Serial.println(currentRPM, 1);   


    if (systemRunning == true) {

      if (degrees >= upper_bound) {
        motor.setSpeed(0);
        delay(500);
        motor.setSpeed(-1*MOTOR_SPEED);
    }
      else if (degrees <= lower_bound) {
        motor.setSpeed(MOTOR_SPEED);
        
    }

    }

    else {
      motor.setSpeed(0);
    }

   
  }

}
