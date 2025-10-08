/**
 * @file main.c
 * @author Dinh Tien (tien.tak21@hcmut.edu.vn)
 * @brief S32K144 microcontroller demo program:
 *        - Initializes System Oscillator (SOSC), PORTs/GPIO and ADC.
 *        - Reads potentiometer value via ADC and controls 3 LEDs
 *          (red, green, blue) based on the voltage level.
 * @version 0.1
 * @date 2025-10-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "S32K144.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void SOSC_Init(void);
void PORT_Init(void);
void GPIO_Init(void);
void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

/*******************************************************************************
 * Code
 ******************************************************************************/

 /**
  * @brief Initialize the System Oscillator (SOSC).
  *
  * This function configures the external oscillator by:
  * - Disabling SOSC before configuration
  * - Setting up EREFS, HGO, and RANGE bits for external crystal
  * - Configuring SOSC clock divider
  * - Enabling SOSC and waiting for it to stabilize
  */
void SOSC_Init()
{
    /*Disable the SOSC*/
    IP_SCG->SOSCCSR &= ~SCG_SOSCCSR_SOSCEN_MASK;

    /*Config EREFS bit: 1, HGO bit: 1 and RANGE bit: 0b11 in SOSCCFG register*/
    IP_SCG->SOSCCFG = SCG_SOSCCFG_EREFS(1)| SCG_SOSCCFG_HGO(1)| SCG_SOSCCFG_RANGE(3);

    /*Config SOSC Divide 2 by 1*/
    IP_SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV2(1);

    /*Enable the SOSC*/
    IP_SCG->SOSCCSR |= (1U << SCG_SOSCCSR_SOSCEN_SHIFT);

    /*Wait until SOSC is stable*/
    while ((IP_SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) == 0);
}

/**
 * @brief Initialize PORTs used in the application.
 *
 * This function:
 * - Enables clock for PORTD (3 LEDs) and PORTA (potentiometer input)
 * - Configures LED pins to GPIO mode via multiplexing
 * - Sets PTA6 to analog mode (clears MUX) for ADC usage
 */
void PORT_Init()
{
    /*Enable clock for PORTD(3 LEDS) and PORTA(Potentiometer)*/
    IP_PCC->PCCn[PCC_PORTD_INDEX] = PCC_PCCn_CGC(1);
    IP_PCC->PCCn[PCC_PORTA_INDEX] = PCC_PCCn_CGC(1);

    /*Choose GPIO mode for PTD0 - Blue LED*/
    IP_PORTD->PCR[0] &= ~PORT_PCR_MUX_MASK;
    IP_PORTD->PCR[0]  = (1U << PORT_PCR_MUX_SHIFT);

    /*Choose GPIO mode for PTD16 - Green LED*/
    IP_PORTD->PCR[16] &= ~PORT_PCR_MUX_MASK;
    IP_PORTD->PCR[16] = (1U << PORT_PCR_MUX_SHIFT);

    /*Choose GPIO mode for PTD15 - Red LED*/
    IP_PORTD->PCR[15] &= ~PORT_PCR_MUX_MASK;
    IP_PORTD->PCR[15] = (1U << PORT_PCR_MUX_SHIFT);

    /*Choose Analog mode for PTA6 - Potentiometer*/
    IP_PORTA->PCR[6] &= ~PORT_PCR_MUX_MASK;

}

/**
 * @brief Configure GPIO direction for LED pins.
 *
 * Sets pins PTD0 (Blue), PTD15 (Red), and PTD16 (Green) as outputs
 * for controlling the LEDs.
 */
void GPIO_Init()
{
    /*Choose OUTPUT direction for PTD0, PTD16, PTD15*/
    IP_PTD->PDDR |= (1U << 0) | (1U << 16) | (1U << 15);

}

/**
 * @brief Initialize ADC0 for reading analog signals from potentiometer.
 *
 * Configuration steps:
 * - Disable/restart ADC0 clock to ensure clean configuration
 * - Select ALTCLK1 (SOSCDIV2_CLK) as ADC clock source
 * - Configure ADC for 12-bit mode with appropriate clock settings
 * - Initialize CFG2, SC2, and SC3 registers
 */
void ADC_Init()
{
    /*Disable clock for ADC0 peripheral*/
    IP_PCC->PCCn[PCC_ADC0_INDEX] &= ~PCC_PCCn_CGC_MASK;

    /*Choose ALTCLK1: SOSCDIV2_CLK*/
    IP_PCC->PCCn[PCC_ADC0_INDEX] = PCC_PCCn_PCS(1);

    /*Enable clock for ADC0 peripheral*/
    IP_PCC->PCCn[PCC_ADC0_INDEX] = PCC_PCCn_CGC(1);

    /*Choose Mode bit : 12-bit, Input clock select: ALTCLK1, Clock divide select: 2*/
    IP_ADC0->CFG1 = ADC_CFG1_MODE(1) | ADC_CFG1_ADICLK(0) | ADC_CFG1_ADIV(1);

    /*Set value 0 for SC2, SC3, CFG2*/
    IP_ADC0->CFG2 = ADC_CFG2_SMPLTS(0);
    IP_ADC0->SC2 = 0x00;
    IP_ADC0->SC3 = 0x00;
}

/**
 * @brief Read ADC value from specified channel.
 *
 * This function initiates ADC conversion by writing the channel number
 * to SC1[0], waits for conversion complete flag (COCO), then returns
 * the converted value from R[0] register.
 *
 * @param channel ADC channel number to read (e.g., channel 2 for PTA6)
 * @return uint16_t 12-bit ADC result (0..4095)
 */
uint16_t ADC_Read(uint8_t channel)
{
    IP_ADC0->SC1[0] = ADC_SC1_ADCH(channel);
    while (!(IP_ADC0->SC1[0] & ADC_SC1_COCO_MASK));
    return (uint16_t)IP_ADC0->R[0];
}

int main(void)
{
    PORT_Init();
    GPIO_Init();
    SOSC_Init();
    ADC_Init();

    uint16_t adc_value, voltage;

    while (1)
    {
        adc_value = ADC_Read(2);
        voltage = (adc_value * 5000U) / 4095U;

        IP_PTD->PSOR |= (1U << 0) | (1U << 15) | (1U << 16);

        if (voltage >= 3750)
        {
            /*Red LED (PTD15)*/
            IP_PTD->PCOR = (1U << 15);
        }
        else if (voltage >= 2500)
        {
            /*Green LED*/
            IP_PTD->PCOR = (1U << 16);
        }
        else if (voltage >= 1250)
        {
            /*Blue LED*/
            IP_PTD->PCOR = (1U << 0);
        }
        else
        {
            /*Do nothing*/
        }
    }

    return 0;
}
