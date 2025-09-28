#include "Driver_PCC.h"
#include "Driver_GPIO.h"
#include "Driver_Common.h"


/* delay prototype */
static void delay(volatile uint32_t time);

static void delay(volatile uint32_t time)
{
	while(time--);
}
int main(void)
{


	/* Clock configuration for buttons: PTC12, PTC13*/
	if (Driver_PCC0.Initialize(PTC) == ARM_DRIVER_OK)
	{
		Driver_PCC0.SetOutputMode(48, ARM_GPIO_PUSH_PULL);
		Driver_PCC0.SetDirection(48, ARM_GPIO_INPUT);
		Driver_PCC0.SetPullResistor(48, ARM_GPIO_PULL_DOWN);

		Driver_PCC0.SetOutputMode(49, ARM_GPIO_PUSH_PULL);
		Driver_PCC0.SetDirection(49, ARM_GPIO_INPUT);
		Driver_PCC0.SetPullResistor(49, ARM_GPIO_PULL_DOWN);
	}
	else
	{
		printf("Clock configuration for btn0 and btn1 is invalid !\n");
	}

	/* Clock configuration for RED and GREEN led: PTD15, PTD16*/
	if (Driver_PCC0.Initialize(PTD) == ARM_DRIVER_OK)
	{
		Driver_PCC0.SetOutputMode(69, ARM_GPIO_PUSH_PULL);
		Driver_PCC0.SetDirection(69, ARM_GPIO_OUTPUT);

		Driver_PCC0.SetOutputMode(70, ARM_GPIO_PUSH_PULL);
		Driver_PCC0.SetDirection(70, ARM_GPIO_OUTPUT);
	}
	else
	{
		printf("Clock configuration for RED and GREEN led is invalid !\n");
	}

	/* Turn off RED and GREEN led */
	Driver_PCC0.SetOutput(69, 1);
	Driver_PCC0.SetOutput(70, 1);

	while (1)
	{
		if (Driver_PCC0.GetInput(48) == 1)
		{
			delay(100);
			if (Driver_PCC0.GetInput(48) == 1)
			{

			}
		}
	}




}
