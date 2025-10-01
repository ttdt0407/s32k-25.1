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
#include "../driver/inc/Driver_SCG.h"
#include "../include/S32K144.h"

/*******************************************************************************
 * 									Prototypes
 ******************************************************************************/
static uint32_t scg_get_mode_reg(SCG_CLOCK_MODE_t mode);
static SCG_STATUS_t scg_check_source_valid(SCG_CLOCK_SOURCE_t src);
static void scg_config_system_source(SCG_CLOCK_SOURCE_t src, uint32_t *pReg);
static void scg_config_system_div_core(SCG_DIV_CORE_t div_core, uint32_t *pReg);
static void scg_config_system_div_bus(SCG_DIV_CORE_t div_bus, uint32_t *pReg);
static void scg_config_system_div_slow(SCG_DIV_CORE_t div_slow,uint32_t *pReg);
static SCG_STATUS_t scg_system_clock_status(SCG_CLOCK_SOURCE_t src);

/*******************************************************************************
 * 										Code
 ******************************************************************************/

/**
 * @brief 
 * 
 * @param mode 
 * @return uint32_t 
 */
static uint32_t scg_get_mode_reg(SCG_CLOCK_MODE_t mode)
{
    uint32_t result = 0;
    switch (mode)
    {
        case SCG_HSRUN_MODE:
            result = IP_SCG->HCCR;
            break;
        case SCG_RUN_MODE:
            result = IP_SCG->RCCR;
            break;
         case SCG_VLPR_MODE:
            result = IP_SCG->VCCR;
            break;
        default:
            printf("No pointer to mode register !\n");
            break;
    }
    return result;
}

/**
 * @brief 
 * 
 * @param src 
 * @return SCG_STATUS_t 
 */
static SCG_STATUS_t scg_check_source_valid(SCG_CLOCK_SOURCE_t src)
{
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;
    uint8_t valid = 0;
    uint32_t timeOut = 0;
    volatile const uint32_t *reg = 0;

    switch (src)
    {
        case SCG_SOSC_CLK:
            reg = &(IP_SCG->SOSCCSR);
            break;
        case SCG_SIRC_CLK:
            reg = &(IP_SCG->SIRCCSR);
            break;
        case SCG_FIRC_CLK:
            reg = &(IP_SCG->FIRCCSR);
            break;
        case SCG_SPLL_CLK:
            reg = &(IP_SCG->SPLLCSR);
            break;
        default:
            result = SCG_STATUS_ERROR;
    }

    while ((timeOut < 1000000) && (valid == 0))
    {
        valid = ((((*reg) & 0x01000000) >> SCG_CSR_SCS_SHIFT) & 0x1U);
        ++timeOut;
    }

    if (timeOut >= 1000000)
    {
        result = SCG_STATUS_TIMEOUT;
    }
    else
    {
        result = SCG_STATUS_SUCCESS;
    }

    return result;
}

/**
 * @brief 
 * 
 * @param src 
 * @param mode 
 */
static void scg_config_system_source(SCG_CLOCK_SOURCE_t src, uint32_t *pReg)
{
    /* */
    *pReg &= ~(0xFU << SCG_CSR_SCS_SHIFT);

    /* */
    *pReg |= (src << SCG_CSR_SCS_SHIFT);
}

/**
 * @brief 
 * 
 * @param div_core 
 * @param mode 
 */
static void scg_config_system_div_core(SCG_DIV_CORE_t div_core, uint32_t *pReg)
{
    *pReg = ((*pReg) & ~(0xFU << SCG_CSR_DIVCORE_SHIFT)) | div_core;
}

/**
 * @brief 
 * 
 * @param div_bus 
 * @param mode 
 */
static void scg_config_system_div_bus(SCG_DIV_CORE_t div_bus, uint32_t *pReg)
{

    *pReg = ((*pReg) & ~(0xFU << SCG_CSR_DIVBUS_SHIFT)) | div_bus;
}

/**
 * @brief 
 * 
 * @param div_slow 
 * @param mode 
 */
static void scg_config_system_div_slow(SCG_DIV_CORE_t div_slow, uint32_t *pReg)
{
    *pReg = ((*pReg) & ~(0xFU << SCG_CSR_DIVSLOW_SHIFT)) | div_slow;
}

/**
 * @brief 
 * 
 * @param src 
 * @return SCG_STATUS_t 
 */
static SCG_STATUS_t scg_system_clock_status(SCG_CLOCK_SOURCE_t src)
{
    SCG_STATUS_t result = SCG_STATUS_ERROR;
    uint32_t readVal = 0;
    uint32_t timeOut = 0;

    while (timeOut < 1000000)
    {
        readVal = (((IP_SCG->CSR & (0xFU << SCG_CSR_SCS_SHIFT)) >> SCG_CSR_SCS_SHIFT) & 0xFU);
        ++timeOut;
    }

    /* Check the result */
    if (readVal == src)
    {
        result = SCG_STATUS_SUCCESS;
    }
    else
    {
        result = SCG_STATUS_ERROR;
    }

    return result;
}

/**
 * @brief 
 * 
 * @param config 
 * @return SCG_STATUS_t 
 */

SCG_STATUS_t SCG_SetSystemClockConfig(scg_config_struct_t *config)
{
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Copy value of register to temp value */
    uint32_t regValue = scg_get_mode_reg(config->mode);

    /* Check source is valid or not */
    result = scg_check_source_valid(config->src);

    /* Config clock source */
    scg_config_system_source(config->src, &regValue);

    /* Config system div core */
    scg_config_system_div_core(config->div_core, &regValue);

    /* Config system div bus */
    scg_config_system_div_bus(config->div_bus, &regValue);

    /* Config system div slow */
    scg_config_system_div_slow(config->div_slow, &regValue);


    /* Write to the register */

    switch (config->mode)
    {
        case SCG_HSRUN_MODE:
            IP_SCG->HCCR = regValue;
            break;
        case SCG_RUN_MODE:
            IP_SCG->RCCR = regValue;
            break;
        case SCG_VLPR_MODE:
            IP_SCG->VCCR = regValue;
            break;
        default:
            result = SCG_STATUS_ERROR;
            break;
    }

     /* Check clock status */
    scg_system_clock_status(config->src);

    return result;
}

/**
 * @brief 
 * 
 * @param src 
 * @return SCG_STATUS_t 
 */
SCG_STATUS_t SCG_SourceEnable(SCG_CLOCK_SOURCE_t src)
{
    SCG_STATUS_t result = SCG_STATUS_ERROR;
    uint8_t checkBit = 0;
    uint32_t timeOut = 1000000;
    uint32_t *pReg = NULL;

    if (src == SCG_SOSC_CLK)
    {
        pReg = &(IP_SCG->SOSCCSR);
    }
    else if (src == SCG_SIRC_CLK)
    {
        pReg = &(IP_SCG->SIRCCSR);
    }
    else if (src == SCG_FIRC_CLK)
    {
        pReg = &(IP_SCG->FIRCCSR);
    }
    else
    {
        printf("Check the source !\n");
    }

    (*pReg) |= (1U << SCG_SOSCCSR_SOSCEN_SHIFT);

    while ((timeOut != 0) && (checkBit == 0))
    {
        checkBit = ((((*pReg) & (1U << SCG_SOSCCSR_SOSCVLD_SHIFT)) >> SCG_SOSCCSR_SOSCVLD_SHIFT) & 1U);
        timeOut--;
    }

    if (timeOut == 0)
    {
        result = SCG_STATUS_TIMEOUT;
    }
    else if (checkBit == 1)
    {
        result = SCG_STATUS_SUCCESS;
    }

    return result;
}
