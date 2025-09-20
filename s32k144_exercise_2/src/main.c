/**
 * @file s32k144_exercise_2
 * @author tatrandinhtien (tatrandinhtien@gmail.com)
 * @brief  Blink 3 LED: Blue - Red - Green
 * @version 0.1
 * @date 2025-09-20
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "S32K144.h"

#include <stdio.h>

void delay(volatile uint32_t time);

void delay(volatile uint32_t time)
{
	while(time--);
}

int main()
{
	/* Clock Port D*/
	IP_PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC_MASK;

	/* GPIO mode */
	IP_PORTD->PCR[0] |= (0x1U << 8);
	IP_PORTD->PCR[15] |= (0x1U << 8);
	IP_PORTD->PCR[16] |= (0x1U << 8);

	/* Output */
	IP_PTD->PDDR |= (0x1U << 0) | (0x1U << 15) | (0x1U << 16);
	IP_PTD->PSOR |= (0x1U << 0);
	IP_PTD->PSOR |= (0x1U << 15);
	IP_PTD->PSOR |= (0x1U << 16);


	while(1)
	{
        /* Turn on LED Blue - Turn off LED Red and Green */
		IP_PTD->PCOR |= (0x1U << 0);
		IP_PTD->PSOR |= (0x1U << 15);
		IP_PTD->PSOR |= (0x1U << 16);

		delay(10000000);

        /* Turn on LED Red - Turn off LED Blue and Green */
		IP_PTD->PCOR |= (0x1U << 15);
		IP_PTD->PSOR |= (0x1U << 0);
		IP_PTD->PSOR |= (0x1U << 16);

		delay(10000000);

        /* Turn on LED Green - Turn off LED Blue and Red */
		IP_PTD->PCOR |= (0x1U << 16);
		IP_PTD->PSOR |= (0x1U << 0);
		IP_PTD->PSOR |= (0x1U << 15);

		delay(10000000);

	}

	return 0;
}
