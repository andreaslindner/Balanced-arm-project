# Project Board LCP11C24

## Program design
![program activity](architecture.png)

## Bugs

* [] Sometimes the program doesn't succeed in initalizing the IMU, problem for write into the IMU, the i2c bus stays on the state I2C_BUSY ( solution may be to create a non blocking write function dealing with time out issue)
* [] In order to clean the interrupt triggered by the IMU, we need to clear gpio interrupt and to clear the interrupt register of the IMU. Clearing the gpio interrupt is not a problem but to clear the IMU interrupt register we need to read any register of the IMU. This might become an issue if we ask for reading IMU while we still didn't finish reading it from the last time (a situation like we're receiving the values and we won't ask for any more register to read), the IMU interrupt register won't be cleared, and IMU won't be anymore able to trigger the interrupt. Consequences : IMU handler won't be executed again and the control loop won't execute anymore.