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
 *                                  Definitions
 ******************************************************************************/

#define TIMEOUT                       10000
#define SCG_CSR_VLD_BIT_MASK         (0x1000000U)
#define SCG_CSR_VLD_BIT_SHIFT        (0x24U)
#define SCG_CSR_ENABLE_BIT_SHIFT     (0x1U)
/*******************************************************************************
 * 									Prototypes
 ******************************************************************************/
static uint32_t scg_get_mode_reg(SCG_CLOCK_MODE_t mode);
static SCG_STATUS_t scg_check_source_valid(SCG_CLOCK_SOURCE_t src);
static void scg_config_system_source(SCG_CLOCK_SOURCE_t src, uint32_t *pReg);
static void scg_config_system_div_core(SCG_DIV_CORE_t div_core, uint32_t *pReg);
static void scg_config_system_div_bus(SCG_DIV_BUS_t div_bus, uint32_t *pReg);
static void scg_config_system_div_slow(SCG_DIV_SLOW_t div_slow,uint32_t *pReg);
static SCG_STATUS_t scg_system_clock_status(SCG_CLOCK_SOURCE_t src);

/*******************************************************************************
 * 										Code
 ******************************************************************************/

/**
 * @brief Get the current value of the clock configuration register for the selected mode.
 *
 * Reads a snapshot of the HCCR/RCCR/VCCR register corresponding to HSRUN, RUN or VLPR
 * mode. If an invalid mode is provided this function returns 0.
 *
 * @param mode System mode (HSRUN/RUN/VLPR) to read.
 * @return uint32_t Raw register value for the requested mode; 0 if mode is invalid.
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
 * @brief Check whether the specified clock source is valid (ready).
 *
 * Polls the VALID bit of the related source control register (SOSC/SIRC/FIRC/SPLL)
 * until the source becomes valid or a TIMEOUT occurs. Returns SCG_STATUS_TIMEOUT
 * if the source does not become valid within the configured loop count.
 *
 * @param src Clock source to validate.
 * @return SCG_STATUS_t SCG_STATUS_SUCCESS if ready; SCG_STATUS_TIMEOUT if timed out; SCG_STATUS_ERROR if source is invalid.
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

    while ((timeOut < TIMEOUT) && (valid == 0))
    {
        valid = ((((*reg) & SCG_CSR_VLD_BIT_MASK) >> SCG_CSR_VLD_BIT_SHIFT) & 0x1U);
        ++timeOut;
    }

    if (timeOut >= TIMEOUT)
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
 * @brief Configure (prepare) the system clock source selection field.
 *
 * Clears the previous source selection in the working copy (pReg) and inserts
 * the new SCS field value. This does not write hardware directly; the caller
 * later commits the assembled register value.
 *
 * @param src Target system clock source.
 * @param pReg Pointer to working register copy for the active mode.
 */
static void scg_config_system_source(SCG_CLOCK_SOURCE_t src, uint32_t *pReg)
{
    /* Clear the previous source */
    *pReg &= ~(0xFU << SCG_CSR_SCS_SHIFT);

    /* Config system source */
    *pReg |= (src << SCG_CSR_SCS_SHIFT);
}

/**
 * @brief Set the core clock divider field in the working copy.
 *
 * Masks out the previous DIVCORE field then inserts the new divider. Does not
 * access hardware directly.
 *
 * @param div_core Core divider enumeration value.
 * @param mode (Unused) kept for comment style consistency.
 */
static void scg_config_system_div_core(SCG_DIV_CORE_t div_core, uint32_t *pReg)
{
    *pReg = ((*pReg) & ~(0xFU << SCG_CSR_DIVCORE_SHIFT)) | div_core;
}

/**
 * @brief Set the bus clock divider field in the working copy.
 *
 * Clears DIVBUS bits then inserts the provided divider value.
 *
 * @param div_bus Bus divider enumeration value.
 * @param mode (Unused).
 */
static void scg_config_system_div_bus(SCG_DIV_BUS_t div_bus, uint32_t *pReg)
{

    *pReg = ((*pReg) & ~(0xFU << SCG_CSR_DIVBUS_SHIFT)) | div_bus;
}

/**
 * @brief Set the slow clock divider field in the working copy.
 *
 * Clears DIVSLOW bits and writes the new divider. Used for low-speed peripheral domain.
 *
 * @param div_slow Slow divider enumeration value.
 * @param mode (Unused).
 */
static void scg_config_system_div_slow(SCG_DIV_SLOW_t div_slow, uint32_t *pReg)
{
    *pReg = ((*pReg) & ~(0xFU << SCG_CSR_DIVSLOW_SHIFT)) | div_slow;
}

/**
 * @brief Poll system clock status until the selected source is active or timeout.
 *
 * Continuously reads the SCS field in CSR until it matches the requested source
 * or TIMEOUT is reached.
 *
 * @param src Expected active system clock source.
 * @return SCG_STATUS_t SCG_STATUS_SUCCESS if source is active, otherwise SCG_STATUS_ERROR.
 */
static SCG_STATUS_t scg_system_clock_status(SCG_CLOCK_SOURCE_t src)
{
    SCG_STATUS_t result = SCG_STATUS_ERROR;
    uint32_t readVal = 0;
    uint32_t timeOut = 0;

    while (timeOut < TIMEOUT && readVal != src)
    {
        readVal = (((IP_SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT) & 0xFU);
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
 * @brief Apply a system clock configuration specified by the user.
 *
 * Sequence: (1) Snapshot current mode register; (2) Validate source; (3) Update
 * source and divider fields in local copy; (4) Commit to the appropriate mode
 * register; (5) Confirm switch. Source must be enabled beforehand (call
 * SCG_SourceInit if required).
 *
 * @param config Pointer to configuration struct (mode, source, dividers), must not be NULL.
 * @return SCG_STATUS_t SUCCESS / TIMEOUT / ERROR depending on operation result.
 */

SCG_STATUS_t SCG_SetSystemClockConfig(scg_config_struct_t *config)
{
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    if (config == NULL)
    {
        return SCG_STATUS_ERROR;
    }

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
    result = scg_system_clock_status(config->src);

    return result;
}

/**
 * @brief Enable (initialize) a specific SCG clock source.
 *
 * Sets the ENABLE bit for the given source and polls the VALID flag. Does not
 * configure detailed parameters (range, internal dividers) – assumes hardware
 * reset defaults are suitable.
 *
 * @param src Source to enable (SOSC/SIRC/FIRC/SPLL).
 * @return SCG_STATUS_t SUCCESS if becomes valid; TIMEOUT if not; ERROR if invalid source.
 */
SCG_STATUS_t SCG_SourceInit(SCG_CLOCK_SOURCE_t src)
{
    SCG_STATUS_t result = SCG_STATUS_ERROR;
    uint8_t checkBit = 0;
    uint32_t timeOut = TIMEOUT;
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
    else if (src == SCG_SPLL_CLK)
    {
        pReg = &(IP_SCG->SPLLCSR);
    }
    else
    {
        printf("Check the source !\n");
    }

    (*pReg) |= (1U << SCG_CSR_ENABLE_BIT_SHIFT);

    while ((timeOut != 0) && (checkBit == 0))
    {
        checkBit = ((((*pReg) & SCG_CSR_VLD_BIT_MASK) >> SCG_CSR_VLD_BIT_SHIFT) & 1U);
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


/**
 * @brief Configure RUN mode to use FIRC as system clock (nominal 48 MHz).
 *
 * Sets system source = FIRC and dividers: core /1, bus /1, slow /2, then verifies
 * the switch.
 *
 * @return SCG_STATUS_t SUCCESS if switched; ERROR/TIMEOUT otherwise.
 */
SCG_STATUS_t SCG_FIRC_SlowRun_48Mhz(void)
{
    volatile uint32_t tempReg = 0;
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Check FIRC is valid */
    result = scg_check_source_valid(SCG_FIRC_CLK);

     /* Return if the source is not valid */
    if (result != SCG_STATUS_SUCCESS)
    {
        return result;
    }

    /* Config FIRC for the source of system */
    tempReg |= SCG_FIRC_CLK << SCG_RCCR_SCS_SHIFT;

    /* Config divcore divide by 1 */
    tempReg |= SCG_DIV_CORE_BY_1 << SCG_RCCR_DIVCORE_SHIFT;

    /* Config divbus divide by 1 */
    tempReg |= SCG_DIV_BUS_BY_1 << SCG_RCCR_DIVBUS_SHIFT;

    /* Config divslow divide by 2 */
    tempReg |= SCG_DIV_SLOW_BY_2 << SCG_RCCR_DIVSLOW_SHIFT;

    /* Write temp reg value to RCCR */
    IP_SCG->RCCR = tempReg;

    /* Check the source is init successfully or not */
    result = scg_system_clock_status(SCG_FIRC_CLK);

    return result;
}

/**
 * @brief Configure RUN mode to use SPLL targeting ~80 MHz core clock.
 *
 * Uses SPLL as source with dividers: core /2, bus /2, slow /3. Actual frequency
 * depends on prior SPLL setup.
 *
 * @return SCG_STATUS_t Switch result status.
 */
SCG_STATUS_t SCG_SPLL_NormalRun_80Mhz()
{
    volatile uint32_t tempReg = 0;
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Check SPLL is valid */
    result = scg_check_source_valid(SCG_SPLL_CLK);

    /* Return if the source is not valid */
    if (result != SCG_STATUS_SUCCESS)
    {
        return result;
    }

    /* Config FIRC for the source of system */
    tempReg |= SCG_SPLL_CLK << SCG_RCCR_SCS_SHIFT;

    /* Config divcore divide by 1 */
    tempReg |= SCG_DIV_CORE_BY_2 << SCG_RCCR_DIVCORE_SHIFT;

    /* Config divbus divide by 1 */
    tempReg |= SCG_DIV_BUS_BY_2 << SCG_RCCR_DIVBUS_SHIFT;

    /* Config divslow divide by 2 */
    tempReg |= SCG_DIV_SLOW_BY_3 << SCG_RCCR_DIVSLOW_SHIFT;

    /* Write temp reg value to RCCR */
    IP_SCG->RCCR = tempReg;

    /* Check the source is init successfully or not */
    result = scg_system_clock_status(SCG_SPLL_CLK);

    return result;
}

/**
 * @brief Configure RUN mode to use SPLL targeting ~64 MHz core clock.
 *
 * Uses SPLL as source with dividers matching the 80 MHz profile placeholder.
 * Adjust external SPLL settings accordingly.
 *
 * @return SCG_STATUS_t Switch result status.
 */
SCG_STATUS_t SCG_SPLL_NormalRun_64Mhz()
{
    volatile uint32_t tempReg = 0;
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Check SPLL is valid */
    result = scg_check_source_valid(SCG_SPLL_CLK);

     /* Return if the source is not valid */
    if (result != SCG_STATUS_SUCCESS)
    {
        return result;
    }

    /* Config FIRC for the source of system */
    tempReg |= SCG_SPLL_CLK << SCG_RCCR_SCS_SHIFT;

    /* Config divcore divide by 1 */
    tempReg |= SCG_DIV_CORE_BY_2 << SCG_RCCR_DIVCORE_SHIFT;

    /* Config divbus divide by 1 */
    tempReg |= SCG_DIV_BUS_BY_2 << SCG_RCCR_DIVBUS_SHIFT;

    /* Config divslow divide by 2 */
    tempReg |= SCG_DIV_SLOW_BY_3 << SCG_RCCR_DIVSLOW_SHIFT;

    /* Write temp reg value to RCCR */
    IP_SCG->RCCR = tempReg;

    /* Check the source is init successfully or not */
    result = scg_system_clock_status(SCG_SPLL_CLK);

    return result;
}

/**
 * @brief Configure HSRUN mode to use SPLL targeting ~112 MHz core clock.
 *
 * Dividers: core /1, bus /2, slow /4. High performance profile – ensure HSRUN
 * is enabled at system level.
 *
 * @return SCG_STATUS_t Switch result status.
 */
SCG_STATUS_t SCG_SPLL_HSRun_112Mhz()
{
    volatile uint32_t tempReg = 0;
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Check SPLL is valid */
    result = scg_check_source_valid(SCG_SPLL_CLK);

    /* Return if the source is not valid */
    if (result != SCG_STATUS_SUCCESS)
    {
        return result;
    }

    /* Config FIRC for the source of system */
    tempReg |= SCG_SPLL_CLK << SCG_RCCR_SCS_SHIFT;

    /* Config divcore divide by 1 */
    tempReg |= SCG_DIV_CORE_BY_1 << SCG_RCCR_DIVCORE_SHIFT;

    /* Config divbus divide by 1 */
    tempReg |= SCG_DIV_BUS_BY_2 << SCG_RCCR_DIVBUS_SHIFT;

    /* Config divslow divide by 2 */
    tempReg |= SCG_DIV_SLOW_BY_4 << SCG_RCCR_DIVSLOW_SHIFT;

    /* Write temp reg value to RCCR */
    IP_SCG->RCCR = tempReg;

    /* Check the source is init successfully or not */
    result = scg_system_clock_status(SCG_SPLL_CLK);

    return result;
}

/**
 * @brief Configure HSRUN mode to use SPLL targeting ~80 MHz core clock.
 *
 * Dividers: core /2, bus /2, slow /3.
 *
 * @return SCG_STATUS_t Switch result status.
 */
SCG_STATUS_t SCG_SPLL_HSRun_80Mhz()
{
    volatile uint32_t tempReg = 0;
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Check SPLL is valid */
    result = scg_check_source_valid(SCG_SPLL_CLK);

    /* Return if the source is not valid */
    if (result != SCG_STATUS_SUCCESS)
    {
        return result;
    }

    /* Config FIRC for the source of system */
    tempReg |= SCG_SPLL_CLK << SCG_RCCR_SCS_SHIFT;

    /* Config divcore divide by 1 */
    tempReg |= SCG_DIV_CORE_BY_2 << SCG_RCCR_DIVCORE_SHIFT;

    /* Config divbus divide by 1 */
    tempReg |= SCG_DIV_BUS_BY_2 << SCG_RCCR_DIVBUS_SHIFT;

    /* Config divslow divide by 2 */
    tempReg |= SCG_DIV_SLOW_BY_3 << SCG_RCCR_DIVSLOW_SHIFT;

    /* Write temp reg value to RCCR */
    IP_SCG->RCCR = tempReg;

    /* Check the source is init successfully or not */
    result = scg_system_clock_status(SCG_SPLL_CLK);

    return result;
}

/**
 * @brief Configure VLPR mode to use SIRC targeting ~4 MHz core clock.
 *
 * Dividers: core /2, bus /1, slow /4. Intended for low power operation.
 *
 * @return SCG_STATUS_t Switch result status.
 */
SCG_STATUS_t SCG_SIRC_VLPRRun_4Mhz()
{
    volatile uint32_t tempReg = 0;
    SCG_STATUS_t result = SCG_STATUS_SUCCESS;

    /* Check SIRC is valid */
    result = scg_check_source_valid(SCG_SIRC_CLK);

    /* Return if the source is not valid */
    if (result != SCG_STATUS_SUCCESS)
    {
        return result;
    }

    /* Config FIRC for the source of system */
    tempReg |= SCG_SIRC_CLK << SCG_RCCR_SCS_SHIFT;

    /* Config divcore divide by 1 */
    tempReg |= SCG_DIV_CORE_BY_2 << SCG_RCCR_DIVCORE_SHIFT;

    /* Config divbus divide by 1 */
    tempReg |= SCG_DIV_BUS_BY_1 << SCG_RCCR_DIVBUS_SHIFT;

    /* Config divslow divide by 2 */
    tempReg |= SCG_DIV_SLOW_BY_4 << SCG_RCCR_DIVSLOW_SHIFT;

    /* Write temp reg value to RCCR */
    IP_SCG->RCCR = tempReg;

    /* Check the source is init successfully or not */
    result = scg_system_clock_status(SCG_SIRC_CLK);

    return result;
}

/**
 * @brief (Not implemented) Return current bus clock frequency.
 *
 * Planned: Read active system source, determine base frequency, apply BUS divider.
 * Currently unimplemented: returns empty value.
 *
 * @return uint32_t Bus frequency (Hz) – TBD.
 */
uint32_t SCG_GetBusClock(void)
{

}

/**
 * @brief (Not implemented) Return current core clock frequency.
 *
 * Planned: Read SCS field to identify source, map to base (FIRC/SIRC/SOSC/SPLL),
 * apply CORE divider. Currently unimplemented.
 *
 * @return uint32_t Core frequency (Hz) – TBD.
 */
uint32_t SCG_GetCoreClock(void)
{

}
