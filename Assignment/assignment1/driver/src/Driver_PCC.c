/**
 * @file Driver_SCG.c
 * @author Ta Tran Dinh Tien (tatrandinhtien@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../driver/inc/Driver_Common.h"
#include "../driver/inc/Driver_PCC.h"
#include "../include/S32K144.h"

/*******************************************************************************
 * 										Code
 ******************************************************************************/

 /**
 * @brief Configure clock settings for a specific peripheral via PCC.
 *
 * Intended to (in future) set selector/divider/PCS options for the given peripheral's
 * PCC register. Currently not implemented (stub) and performs no register writes.
 *
 * @param peripheral Peripheral identifier (offset added to IP_PCC_BASE).
 * @param config Clock configuration structure describing desired source/dividers.
 */
void PCC_SetClockConfiguration(PCC_PERIPHERALS_t peripheral, pcc_clock_config_t config)
{

}

/**
 * @brief Enable (gate on) clock for a given peripheral.
 *
 * Validates the peripheral register (PR bit must be set) before setting the CGC bit
 * to enable the clock. If the peripheral is not present (PR==0) returns PCC_STATUS_ERROR.
 *
 * @param peripheral Peripheral identifier (offset from IP_PCC_BASE).
 * @return PCC_STATUS_t PCC_STATUS_SUCCESS on success, PCC_STATUS_ERROR if invalid.
 */
PCC_STATUS_t PCC_EnableClock(PCC_PERIPHERALS_t peripheral)
{
    PCC_STATUS_t result = PCC_STATUS_SUCCESS;

    /*Store address of the peripheral reg*/
    volatile uint32_t * const pReg = (volatile uint32_t *)(IP_PCC_BASE + peripheral);

    /*Check the peripheral is valid*/
    uint8_t checkBit = (((*pReg & PCC_PCCn_PR_MASK) >> PCC_PCCn_PR_SHIFT) & 1U);

    if (0 == checkBit)
    {
        return PCC_STATUS_ERROR;
    }

    /*Set CGC bit*/
    *pReg |= (1U << PCC_PCCn_CGC_SHIFT);

    return result;
}

/**
 * @brief Disable (gate off) clock for a given peripheral.
 *
 * Validates the peripheral (PR bit) then clears the CGC bit to stop its clock. Returns
 * error status if the peripheral is not present.
 *
 * @param peripheral Peripheral identifier (offset from IP_PCC_BASE).
 * @return PCC_STATUS_t PCC_STATUS_SUCCESS on success, PCC_STATUS_ERROR if invalid.
 */
PCC_STATUS_t PCC_DisableClock(PCC_PERIPHERALS_t peripheral)
{
    PCC_STATUS_t result = PCC_STATUS_SUCCESS;

     /*Store address of the peripheral reg*/
    volatile uint32_t * const pReg = (volatile uint32_t *)(IP_PCC_BASE + peripheral);

    /*Check the peripheral is valid*/
    uint8_t checkBit = (((*pReg & PCC_PCCn_PR_MASK) >> PCC_PCCn_PR_SHIFT) & 1U);

    if (0 == checkBit)
    {
        return PCC_STATUS_ERROR;
    }

    /*Clear CGC bit*/
    *pReg &= ~PCC_PCCn_CGC_MASK;

    return result;
}