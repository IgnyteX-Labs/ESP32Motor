#include <Arduino.h>
#include <ESP32Motor.h>

#define IN1 27
#define IN2 26
#define PWM 25

#define ENCODER_A 35
#define ENCODER_B 34
#define TICKS_PER_REV 720

Motor motor(0, IN1, IN2, PWM, ENCODER_A, ENCODER_B, TICKS_PER_REV);

void setup()
{
	Serial.begin(115200);
	motor.begin();
}

int i = 0;
void loop()
{
	while (i > -255)
	{
		i--;
		motor.drive(i);
		Serial.printf("Current speed: %i, encoder tick: %i, angle: %f°\n", i, motor.getEncoderTicks(), motor.getMotorAngle());
		delay(300);
	}
	while (i < 255)
	{
		i++;
		motor.drive(i);
		Serial.printf("Current speed: %i, encoder tick: %i, angle: %f°\n", i, motor.getEncoderTicks(), motor.getMotorAngle());
		delay(300);
	}
}
