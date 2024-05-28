#include <Arduino.h>
#include <ESP32Motor.h>

#define IN1 27
#define IN2 26
#define PWM 25

#define ENCODER_A 35
#define ENCODER_B 34
#define TICKS_PER_REV 1024

Motor motor(0, IN1, IN2, PWM, ENCODER_A, ENCODER_B, 1024);

void setup() {
	Serial.begin(115200);
	motor.begin();
}

void loop() {
	int i = 0;
	while(i >= -255) {
		i--;
		motor.drive(i);
		Serial.printf("Current encoder tick: %f, angle: %f°\n", motor.getEncoderTicks(), motor.getMotorAngle());
		delay(30);
	}
	while(i <= 255) {
		i++;
		motor.drive(i);
		Serial.printf("Current encoder tick: %f, angle: %f°\n", motor.getEncoderTicks(), motor.getMotorAngle());
		delay(30);
	}
}