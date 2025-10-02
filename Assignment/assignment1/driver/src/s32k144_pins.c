/**
 * @file s32k144_pins.c
 * @author Ta Tran Dinh Tien (tatrandinhtien@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "s32k144_pins.h"
#include "Driver_Common.h"

/*******************************************************************************
 *                                  Definitions
 ******************************************************************************/

#define S32K144_DECODE_PORT(pin_id)      ((pin_id >> 8) & 0xFFU)
#define S32K144_DECODE_GPIO(pin_id)      ((pin_id >> 8) & 0xFFU)

/* Extract the pin index (lower nibble). If higher pin numbers (>15) are needed,
 * the mask might need to expand, but remains unchanged here.
 */
#define S32K144_DECODE_PIN(pin_id)       (pin_id & (0x007FU))

/*******************************************************************************
 *                                      Code
 ******************************************************************************/

/**
 * @brief Get PORT register base address for an encoded pin.
 *
 * Decodes the port portion of the provided PinName_t (using S32K144_DECODE_PORT) and
 * returns the corresponding PORT_Type* from the static lookup table.
 *
 * @param pin Encoded pin value (created via PIN_ID()).
 * @return PORT_Type* Pointer to PORTx registers, or NULL if pin is 0 or the decoded
 *         port does not match any handled case.
 */
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

/**
 * @brief Get GPIO register base address for an encoded pin.
 *
 * Uses S32K144_DECODE_GPIO (same implementation as port decode) and returns the
 * pointer to the GPIO_Type instance (PTx) from the lookup table.
 *
 * @param pin Encoded pin value.
 * @return GPIO_Type* Pointer to GPIO register block for the decoded port; NULL on error.
 */
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

/**
 * @brief Extract the pin number (bit position) from an encoded pin identifier.
 *
 * Returns the lower nibble of the encoded value via S32K144_DECODE_PIN().
 *
 * @param pin Encoded pin (PIN_ID()). If 0, function prints an error and returns 0.
 * @return uint32_t Pin index (0..15 currently, due to 0xF mask).
 */
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
