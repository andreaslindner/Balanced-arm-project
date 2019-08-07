#include <stdint.h>
#include <chip.h>
#include <pinint.h>
#include <uart.h>
#include <i2c.h>
#include <imu.h>
#include <fonction.h>
#include <math.h>
#include <motor2.h>
#include <timer.h>

extern const uint8_t imu_address;
extern volatile int16_t values[7];
extern volatile uint8_t function;
extern volatile uint8_t ask_for_new_value;
extern volatile uint8_t read_available;

const float sampleTime = 0.005;
volatile float previousAngle = 0;
volatile float errorSum = 0;
float targetAngle = -10;

extern volatile float kp;
extern volatile float ki;
extern volatile float kd;
extern volatile float alpha;
extern volatile uint8_t first;


void Init_PININT()
{
	//Set pin dir as input
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT);
	//Configure pin as GPIO with pull-up
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIN_ID, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
	// Configure channel interrupt as edge sensitive and rising edge interrupt
	Chip_GPIO_SetupPinInt(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT, GPIO_INT_RISING_EDGE);

	// Enable GPIO pin interrupt
	Chip_GPIO_EnableInt(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));

	//Set register as 0
	IMU_Read_Values();

	// Enable interrupt in the NVIC
	NVIC_ClearPendingIRQ(PININT_NVIC_NAME);
	NVIC_SetPriority(PININT_NVIC_NAME,1);
	NVIC_EnableIRQ(PININT_NVIC_NAME);
}

/* Calculate the new angle from the values retrieved from the IMU */
static void compute_new_angle(float *currentAngle)
{
	float accAngle =  (values[2] == 0) ? 0 : (atan2(values[0],values[2]) * RAD_TO_DEG);
	float gyroSpeed = translate(values[5], -32768, 32767, -250, 250);
	float gyroAngle = (float) gyroSpeed * sampleTime;

	*currentAngle = alpha * (previousAngle + gyroAngle) + (1-alpha) * accAngle;
}

void PININT_IRQ_HANDLER(void)
{
	float currentAngle, error, motorPower;

	/* Clear GPIO Int */
	Chip_GPIO_ClearInts(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));

	/* Calculate the angle */
	if (first == 0)  {
		compute_new_angle(&currentAngle);
	} else {
		first = 0;
		currentAngle = 0;
	}

	/* Calculate the errors */
	error = currentAngle - targetAngle;
	errorSum += error;
	errorSum = errorSum <= -1000 ? -1000 : (errorSum >= 1000 ? 1000 : errorSum);			// constrain errorSum to be between -300 and 300

	/* Calculate the output from PID algorithm */
	motorPower =	  kp * error
					+ ki * errorSum * sampleTime
					- kd * (currentAngle - previousAngle) / sampleTime;

	/* Prepare next loop */
	previousAngle = currentAngle;

	/* Set the power of the motors */
	Motors_Move(motorPower);

	/* Ask for read a new value of the IMU, in order to clean the interrupt triggered from the IMU you need to read any register from IMU, so this call will clean the last interrupt */
	if (read_available == 1) {
		IMU_Read_Values();
	} else { // read_available == 0
		ask_for_new_value = 1;
	}

}
