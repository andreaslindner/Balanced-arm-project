#include <stdint.h>
#include <chip.h>
#include <pinint.h>
#include <uart.h>
#include <i2c.h>
#include <imu.h>
#include <fonction.h>
#include <math.h>
#include <motor.h>

extern const uint8_t imu_address;
extern volatile int16_t values[7];
extern volatile uint8_t function;
extern volatile uint8_t ask_for_new_value;
extern volatile uint8_t read_available;

const float sampleTime = 0.0098;
volatile float previousAngle = 0;
volatile float errorSum = 0;
const float targetAngle = 0;

/*DEBUG*/
/*
volatile float listAngle[500];
volatile int i = 0;
volatile int j = 0;
*/

void Init_PININT()
{
	//Ser pin dir as input
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT);
	//Configure pin as GPIO with pullup
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIN_ID, (IOCON_FUNC0 | IOCON_MODE_PULLUP | IOCON_DIGMODE_EN));
	// Configure channel interrupt as edge sensitive and rising edge interrupt
	Chip_GPIO_SetupPinInt(LPC_GPIO, GPIO_PININT_PORT, GPIO_PININT, GPIO_INT_RISING_EDGE);

	// Enable GPIO pin intrerrupt
	Chip_GPIO_EnableInt(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));

	//Set register as 0
	IMU_Read_Values();

	// Enable interrupt in the NVIC
	NVIC_ClearPendingIRQ(PININT_NVIC_NAME);
	NVIC_SetPriority(PININT_NVIC_NAME,1);
	NVIC_EnableIRQ(PININT_NVIC_NAME);
}

static void compute_new_angle(float *currentAngle)
{
	float accAngle = atan2(values[0],values[2]) * RAD_TO_DEG;
	float gyroSpeed = translate(values[5], -32768, 32767, -250, 250);
	float gyroAngle = (float) gyroSpeed * sampleTime;
	*currentAngle = ALPHA * (previousAngle + gyroAngle) + (1-ALPHA) * accAngle;
}

void PININT_IRQ_HANDLER(void)
{
	float currentAngle, error, motorPower;

	/* Clear Int */
	Chip_GPIO_ClearInts(LPC_GPIO, GPIO_PININT_PORT, (1 << GPIO_PININT));

	/* Ask for read a new value of the IMU */
	if (read_available == 1) {
		IMU_Read_Values();
	} else { // read_available == 0
		ask_for_new_value = 1;
	}

	/* Calculate the angle */
	compute_new_angle(&currentAngle);

	/* Calculate the errors */
	error = currentAngle - targetAngle;
	errorSum += error;
	errorSum = errorSum <= -300 ? -300 : (errorSum >= 300 ? 300 : errorSum);			// constrain errorSum to be between -300 and 300

	/* Calculate the output from PID algorithm */
	motorPower =	  KP * error
					+ KI * errorSum * sampleTime
					- KD * (currentAngle - previousAngle) / sampleTime;

	/*Prepare next loop */
	previousAngle = currentAngle;

	/*
	if (i < 500) {
		if (j == 5) {
			listAngle[i] = currentAngle;
			j = 0;
			++i;
		} else {
			j++;
		}
	} else {
		++i;
	}
	*/
	/* Set the power of the motors */
	Motor_setPower(motorPower);

}
