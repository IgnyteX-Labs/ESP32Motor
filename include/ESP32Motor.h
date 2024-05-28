#ifndef ESP32MOTOR_H_
#define ESP32MOTOR_H_

#include <Arduino.h>

/**
 * @brief A class to represent a Motor connected to a Motor driver, wraps the more complex ledc motor driver code required with easy access drive, etc. methods.
 * 
 */
class Motor {
private:
	const int IN1Pin, IN2Pin, PWMPin;
	const int pwmChannel;
	const int encoderPinA, encoderPinB, ticksPerRevolution;
	volatile long encoderCount = 0;
	int motorAngleZero = 0;
	void IRAM_ATTR encoderISR();

public:
	/**
	 * @brief Construct a new Motor object with encoder
	 *
	 * @param pwmChannel 0-15 Specifies the ledc channel used, has to be unique for every Motor object
	 * @param in1 GPIO Pin connected to IN1 on the Driver
	 * @param in2 GPIO Pin connected to IN2 on the Driver
	 * @param pwm GPIO Pin connected to PWM/EN/Enable on the Driver
	 * @param encoderA GPIO Pin connected to the A pin of the encoder on the motor
	 * @param encoderB GPIO Pin connected to the B pin of the encoder on the motor
	 * @param ticksPerRevolution Number of encoder ticks for one full revolution
	 */
	Motor(int pwmChannel, int in1, int in2, int pwm, int encoderA, int encoderB, int ticksPerRevolution);

	/**
	 * @brief Construct a new Motor object without encoder
	 *
	 * @param pwmChannel 0-15 Specifies the ledc channel used, has to be unique for every Motor object
	 * @param in1 GPIO Pin connected to IN1 on the Driver
	 * @param in2 GPIO Pin connected to IN2 on the Driver
	 * @param pwm GPIO Pin connected to PWM/EN/Enable on the Driver
	 */
	Motor(int pwmChannel, int in1, int in2, int pwm);

	/**
	 * @brief Sets up the motor, has to be called once in the setup!
	 *
	 * @return true if succeded
	 * @return false if failed
	 */
	bool begin();

	/**
	 * @brief Drive the motor with a certain speed
	 * 
	 * @param speed -255 to 255 (Goes clockwise for positive speed and counter-clockwise for negative speed)
	 */
	void drive(int speed);

	/**
	 * @brief Immediately stops the motor
	 * 
	 */
	void brake();

	/**
	 * @brief Get the current Angle of the Motor relative to the motor start position
	 * 
	 * @return float angle in -180 to 180
	 */
	float getMotorAngle();

	/**
	 * @brief Zeros the motor angle to the current motor angle
	 * 
	 */
	void setMotorAngleZero();

	/**
	 * @brief Get the Encoder Ticks
	 * 
	 * @return int total encoder ticks since the start of the programm
	 */
	int getEncoderTicks();


};

#endif