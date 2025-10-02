/**
 * @file s32k144_pins.h
 * @author Ta Tran Dinh Tien (tatrandinhtien@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-02
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef S32K144_PINS_H_
#define S32K144_PINS_H_

#include <stdint.h>
#include "../include/S32K144.h"

/*******************************************************************************
 *                                  Definitions
 ******************************************************************************/

/* Total number of available PORT instances used in this abstraction */
#define PORT_NUMS       5U
/* Logical port indices matching the device's PORT instance ordering */
#define PORT_A          0
#define PORT_B          1
#define PORT_C          2
#define PORT_D          3
#define PORT_E          4
/* Encode a (port, pin) pair into a single 16-bit value: high byte = port, low byte = pin */
#define PIN_ID(port, pin)         ((port << 8) | (pin))

/**
 * @brief Enumeration of all PORT A..E pins using the encoded (port,pin) scheme.
 *
 * Each value is produced by PIN_ID(PORT_X, n) where:
 *  - PORT_X is one of PORT_A..PORT_E (0..4)
 *  - n is the pin index within that port (0..16 / 17 depending on port).
 *
 * The enumeration makes it convenient to pass a single PinName_t to helper functions.
 */
typedef enum
{
	/* Pin of PORT A */
    PTA0  = PIN_ID(PORT_A, 0),
    PTA1  = PIN_ID(PORT_A, 1),
    PTA2  = PIN_ID(PORT_A, 2),
    PTA3  = PIN_ID(PORT_A, 3),
    PTA4  = PIN_ID(PORT_A, 4),
    PTA5  = PIN_ID(PORT_A, 5),
    PTA6  = PIN_ID(PORT_A, 6),
    PTA7  = PIN_ID(PORT_A, 7),
    PTA8  = PIN_ID(PORT_A, 8),
    PTA9  = PIN_ID(PORT_A, 9),
    PTA10 = PIN_ID(PORT_A, 10),
    PTA11 = PIN_ID(PORT_A, 11),
    PTA12 = PIN_ID(PORT_A, 12),
    PTA13 = PIN_ID(PORT_A, 13),
    PTA14 = PIN_ID(PORT_A, 14),
    PTA15 = PIN_ID(PORT_A, 15),
    PTA16 = PIN_ID(PORT_A, 16),
    PTA17 = PIN_ID(PORT_A, 17),

	/* Pin of PORT B */
    PTB0  = PIN_ID(PORT_B, 0),
    PTB1  = PIN_ID(PORT_B, 1),
    PTB2  = PIN_ID(PORT_B, 2),
    PTB3  = PIN_ID(PORT_B, 3),
    PTB4  = PIN_ID(PORT_B, 4),
    PTB5  = PIN_ID(PORT_B, 5),
    PTB6  = PIN_ID(PORT_B, 6),
    PTB7  = PIN_ID(PORT_B, 7),
    PTB8  = PIN_ID(PORT_B, 8),
    PTB9  = PIN_ID(PORT_B, 9),
    PTB10 = PIN_ID(PORT_B, 10),
    PTB11 = PIN_ID(PORT_B, 11),
    PTB12 = PIN_ID(PORT_B, 12),
    PTB13 = PIN_ID(PORT_B, 13),
    PTB14 = PIN_ID(PORT_B, 14),
    PTB15 = PIN_ID(PORT_B, 15),
    PTB16 = PIN_ID(PORT_B, 16),
    PTB17 = PIN_ID(PORT_B, 17),

	/* Pin of PORT C */
    PTC0  = PIN_ID(PORT_C, 0),
    PTC1  = PIN_ID(PORT_C, 1),
    PTC2  = PIN_ID(PORT_C, 2),
    PTC3  = PIN_ID(PORT_C, 3),
    PTC4  = PIN_ID(PORT_C, 4),
    PTC5  = PIN_ID(PORT_C, 5),
    PTC6  = PIN_ID(PORT_C, 6),
    PTC7  = PIN_ID(PORT_C, 7),
    PTC8  = PIN_ID(PORT_C, 8),
    PTC9  = PIN_ID(PORT_C, 9),
    PTC10 = PIN_ID(PORT_C, 10),
    PTC11 = PIN_ID(PORT_C, 11),
    PTC12 = PIN_ID(PORT_C, 12),
    PTC13 = PIN_ID(PORT_C, 13),
    PTC14 = PIN_ID(PORT_C, 14),
    PTC15 = PIN_ID(PORT_C, 15),
    PTC16 = PIN_ID(PORT_C, 16),
    PTC17 = PIN_ID(PORT_C, 17),

	/* Pin of PORT D */
    PTD0  = PIN_ID(PORT_D, 0),
    PTD1  = PIN_ID(PORT_D, 1),
    PTD2  = PIN_ID(PORT_D, 2),
    PTD3  = PIN_ID(PORT_D, 3),
    PTD4  = PIN_ID(PORT_D, 4),
    PTD5  = PIN_ID(PORT_D, 5),
    PTD6  = PIN_ID(PORT_D, 6),
    PTD7  = PIN_ID(PORT_D, 7),
    PTD8  = PIN_ID(PORT_D, 8),
    PTD9  = PIN_ID(PORT_D, 9),
    PTD10 = PIN_ID(PORT_D, 10),
    PTD11 = PIN_ID(PORT_D, 11),
    PTD12 = PIN_ID(PORT_D, 12),
    PTD13 = PIN_ID(PORT_D, 13),
    PTD14 = PIN_ID(PORT_D, 14),
    PTD15 = PIN_ID(PORT_D, 15),
    PTD16 = PIN_ID(PORT_D, 16),
    PTD17 = PIN_ID(PORT_D, 17),

	/* Pin of PORT E */
    PTE0  = PIN_ID(PORT_E, 0),
    PTE1  = PIN_ID(PORT_E, 1),
    PTE2  = PIN_ID(PORT_E, 2),
    PTE3  = PIN_ID(PORT_E, 3),
    PTE4  = PIN_ID(PORT_E, 4),
    PTE5  = PIN_ID(PORT_E, 5),
    PTE6  = PIN_ID(PORT_E, 6),
    PTE7  = PIN_ID(PORT_E, 7),
    PTE8  = PIN_ID(PORT_E, 8),
    PTE9  = PIN_ID(PORT_E, 9),
    PTE10 = PIN_ID(PORT_E, 10),
    PTE11 = PIN_ID(PORT_E, 11),
    PTE12 = PIN_ID(PORT_E, 12),
    PTE13 = PIN_ID(PORT_E, 13),
    PTE14 = PIN_ID(PORT_E, 14),
    PTE15 = PIN_ID(PORT_E, 15),
    PTE16 = PIN_ID(PORT_E, 16),

} PinName_t;

/* Base address lookup tables (s32k144 CMSIS style) for PORT and GPIO instances.
 * IP_PORT_BASE_PTRS / IP_GPIO_BASE_PTRS come from the device header and contain
 * the memory-mapped base addresses for PORTA..PORTE and PTA..PTE respectively.
 */
static PORT_Type *const s_port_base_ptr[PORT_NUMS] = IP_PORT_BASE_PTRS;
static GPIO_Type *const s_gpio_base_ptr[PORT_NUMS] = IP_GPIO_BASE_PTRS;

/*******************************************************************************
 *                                      API
 ******************************************************************************/

/**
 * @brief Retrieve the PORT register block pointer for an encoded pin.
 *
 * Decodes the (port) portion of the PinName_t and returns the matching PORT_Type*.
 *
 * @param pin Encoded pin value (see PinName_t). A zero value (PTA0) is currently
 *            treated as invalid inside implementation.
 * @return PORT_Type* Pointer to PORT instance; NULL if the pin (or decoded port) is invalid.
 */
PORT_Type *PORT_GetValue(PinName_t pin);

/**
 * @brief Retrieve the GPIO register block pointer for an encoded pin.
 *
 * Decodes the (port) portion of the PinName_t and returns the matching GPIO_Type*.
 *
 * @param pin Encoded pin value (see PinName_t). A zero value (PTA0) is currently
 *            treated as invalid inside implementation.
 * @return GPIO_Type* Pointer to GPIO instance; NULL if the pin (or decoded port) is invalid.
 */
GPIO_Type *GPIO_GetValue(PinName_t pin);

/**
 * @brief Decode and return the pin number (bit position) from an encoded pin.
 *
 * @param pin Encoded pin value.
 * @return int32_t Pin (0..n). Returns 0 if input is 0 (PTA0) or invalid (no distinction made).
 */
uint32_t Pin_GetValue(PinName_t pin);


#endif /* S32K144_PINS_H_ */
