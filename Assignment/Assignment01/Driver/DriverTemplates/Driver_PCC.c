#include "Driver_PCC.h"
#include "S32K144.h"


/**
 * @brief Initialize (enable clock) for the selected port using Peripheral Clock Control (PCC) of S32K144.
 *
 * This function enables the clock for the selected port (PORTA, PORTB, PORTC, PORTD, PORTE)
 * by writing to the PCC->PCCn[] register of S32K144.
 * If the port is invalid, the function returns an error code.
 *
 * @param port Enum specifying the port (PA, PB, PC, PD, PE)
 * @return int32_t ARM_DRIVER_OK if successful, ARM_DRIVER_ERROR_PARAMETER if error
 */
int32_t PCC_Initialize (ARM_PCC_PORT port)
{
    int32_t result = ARM_DRIVER_OK;
    if (port >= PA && port <= PE)
    {
        switch (port)
        {
            case PA:
                IP_PCC->PCCn[PCC_PORTA_INDEX] &= ~PCC_PCCn_CGC_MASK;
                IP_PCC->PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC(1);
                break;
            case PB:
            	IP_PCC->PCCn[PCC_PORTB_INDEX] &= ~PCC_PCCn_CGC_MASK;
            	IP_PCC->PCCn[PCC_PORTB_INDEX] |= PCC_PCCn_CGC(1);
                break;
            case PC:
            	IP_PCC->PCCn[PCC_PORTC_INDEX] &= ~PCC_PCCn_CGC_MASK;
            	IP_PCC->PCCn[PCC_PORTC_INDEX] |= PCC_PCCn_CGC(1);
                break;
            case PD:
            	IP_PCC->PCCn[PCC_PORTD_INDEX] &= ~PCC_PCCn_CGC_MASK;
            	IP_PCC->PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC(1);
                break;
            case PE:
            	IP_PCC->PCCn[PCC_PORTE_INDEX] &= ~PCC_PCCn_CGC_MASK;
            	IP_PCC->PCCn[PCC_PORTE_INDEX] |= PCC_PCCn_CGC(1);
                break;
        }
    }
    else
    {
        result = ARM_DRIVER_ERROR_PARAMETER;
    }
    return result;
}

ARM_DRIVER_PCC Driver_PCC0 = { PCC_Initialize };

