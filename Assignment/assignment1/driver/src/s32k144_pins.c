#include "s32k144_pins.h"
#include "Driver_Common.h"

/*******************************************************************************
 *                                  Definitions
 ******************************************************************************/

/* Decode pin_id and get port value (e.g PORT_A, PORT_B,...) */
#define S32K144_DECODE_PORT(pin_id)		((pin_id >> 8) & 1U)
#define S32K144_DECODE_GPIO(pin_id)     ((pin_id >> 8) & 1U)

/* Decode pin_id and get pin value (e.g 0, 1, 2, ...) */
#define S32K144_DECODE_PIN(pin_id) 	    (pin_id & (0x000FU))

/*******************************************************************************
 *                                      Code
 ******************************************************************************/

PORT_Type *PORT_GetValue(PinName_t pin)
{
	PORT_Type *res = NULL;
	if (pin)
	{
		uint8_t port = S32K144_DECODE_PORT(pin);
		switch(port)
		{
		case PORT_A:
			res = s_port_base_ptr[PORT_A];
			break;
		case PORT_B:
			res = s_port_base_ptr[PORT_B];
			break;
		case PORT_C:
			res = s_port_base_ptr[PORT_C];
			break;
		case PORT_D:
			res = s_port_base_ptr[PORT_D];
			break;
		case PORT_E:
			res = s_port_base_ptr[PORT_E];
			break;
		default:
			printf("PORT is invalid !\n");
		}
	}
	else
	{
		printf("PIN is invalid !\nCheck the pin input\n");
	}
	return res;
}
GPIO_Type *GPIO_GetValue(PinName_t pin)
{
	GPIO_Type *res = NULL;
		if (pin)
		{
			uint8_t gpio = S32K144_DECODE_GPIO(pin);
			switch(gpio)
			{
			case PORT_A:
				res = s_gpio_base_ptr[PORT_A];
				break;
			case PORT_B:
				res = s_gpio_base_ptr[PORT_B];
				break;
			case PORT_C:
				res = s_gpio_base_ptr[PORT_C];
				break;
			case PORT_D:
				res = s_gpio_base_ptr[PORT_D];
				break;
			case PORT_E:
				res = s_gpio_base_ptr[PORT_E];
				break;
			default:
				printf("GPIO is invalid !\n");
			}
		}
		else
		{
			printf("PIN is invalid !\nCheck the pin input\n");
		}
		return res;
}

uint32_t Pin_GetValue(PinName_t pin)
{
	uint32_t res = 0;
	if (pin)
	{
		res = S32K144_DECODE_PIN(pin);
	}
	else
	{
		printf("PIN is invalid !\nCheck the pin input\n");
	}

	return res;
}
