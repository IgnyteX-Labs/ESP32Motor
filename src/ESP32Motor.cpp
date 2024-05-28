#include "ESP32Motor.h"
#include <FunctionalInterrupt.h>

Motor::Motor(int pwmChannel, int in1, int in2, int pwm, int encoderA, int encoderB, int ticksPerRevolution) : pwmChannel(pwmChannel), IN1Pin(in1), IN2Pin(in2), PWMPin(pwm), encoderPinA(encoderA), encoderPinB(encoderB), ticksPerRevolution(ticksPerRevolution) {}

Motor::Motor(int pwmChannel, int in1, int in2, int pwm) : pwmChannel(pwmChannel), IN1Pin(in1), IN2Pin(in2), PWMPin(pwm), encoderPinA(-1), encoderPinB(-1), ticksPerRevolution(-1) {}

Motor::Motor(int pwmChannel, int in1, int in2, int pwm, int minPwmValue, int encoderA, int encoderB, int ticksPerRevolution) : pwmChannel(pwmChannel), IN1Pin(in1), IN2Pin(in2), PWMPin(pwm), minPwmValue(minPwmValue), encoderPinA(encoderA), encoderPinB(encoderB), ticksPerRevolution(ticksPerRevolution) {}

Motor::Motor(int pwmChannel, int in1, int in2, int pwm, int minPwmValue) : pwmChannel(pwmChannel), IN1Pin(in1), IN2Pin(in2), PWMPin(pwm), minPwmValue(minPwmValue), encoderPinA(-1), encoderPinB(-1), ticksPerRevolution(-1) {}

void IRAM_ATTR Motor::encoderISR()
{
	if (digitalRead(encoderPinA) > digitalRead(encoderPinB))
	{
		encoderCount++;
	}
	else
	{
		encoderCount--;
	}
}

bool Motor::begin()
{
	// Setup motor driver pins
	pinMode(IN1Pin, OUTPUT);
	pinMode(IN2Pin, OUTPUT);
	pinMode(PWMPin, OUTPUT);

	// Configure PWM signal generation
	if (ledcSetup(pwmChannel, 100000, 8) == 0)
	{
		return false;
	}
	ledcAttachPin(PWMPin, pwmChannel);

	// Setup encoder pins and interrupt
	if (encoderPinA != -1 && encoderPinB != -1 && ticksPerRevolution != -1)
	{
		pinMode(encoderPinA, INPUT);
		pinMode(encoderPinB, INPUT);
		attachInterrupt(digitalPinToInterrupt(encoderPinA), std::bind(&Motor::encoderISR, this), RISING);
	}

	return true;
}

void Motor::drive(int speed)
{
	if (speed < -255)
	{
		speed = 255;
	}
	else if (speed > 255)
	{
		speed = 255;
	}

	if (speed == 0)
	{
		brake();
	}
	else if (speed > 0)
	{
		digitalWrite(IN1Pin, HIGH);
		digitalWrite(IN2Pin, LOW);
		ledcWrite(pwmChannel, speed);
	}
	else if (speed < 0)
	{
		digitalWrite(IN1Pin, LOW);
		digitalWrite(IN2Pin, HIGH);
		ledcWrite(pwmChannel, -speed);
	}
}

void Motor::brake()
{
	digitalWrite(IN1Pin, HIGH);
	digitalWrite(IN2Pin, HIGH);
}

int Motor::getEncoderTicks()
{
	return encoderCount;
}

float Motor::getMotorAngle()
{
	float unbalancedAngle = ((float)((encoderCount - motorAngleZero) % ticksPerRevolution)) / (float)ticksPerRevolution * 360; // Gives motor angle in -360 to 360
	float angle = 0;
	if(unbalancedAngle > 180) {
		angle = -((float)360 - unbalancedAngle);
	}else if(unbalancedAngle <= -180) {
		angle = (float)360 + unbalancedAngle;
	}else {
		angle = unbalancedAngle;
	}
	return angle;
}

void Motor::setMotorAngleZero()
{
	motorAngleZero = encoderCount;
}