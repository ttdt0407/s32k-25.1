/**
 * @file Driver_PCC.h
 * @author Ta Tran Dinh Tien (tatrandinhtien@gmail.com)
 * @brief Peripheral Clock Controller (PCC) driver interface for S32K144.
 * @version 0.1
 * @date 2025-10-01
 *
 * Provides lightweight helpers to enable/disable peripheral clocks and (future)
 * configure peripheral clock sources/dividers. Only a subset of capability is
 * currently implemented; configuration API is a stub placeholder.
 */

#ifndef DRIVER_PCC_H_
#define DRIVER_PCC_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *                                  Definitions
 ******************************************************************************/

/**
 * @brief PCC driver status codes.
 *
 * PCC_STATUS_SUCCESS  Operation completed successfully.
 * PCC_STATUS_ERROR    Invalid parameter or peripheral not present (PR bit = 0).
 */
typedef enum
{
    PCC_STATUS_SUCCESS,
    PCC_STATUS_ERROR
} PCC_STATUS_t;

/**
 * @brief Peripheral register offset identifiers used with PCC base.
 *
 * Each enumerator value is the byte offset from IP_PCC_BASE to the PCCn register
 * of the corresponding peripheral. Values follow the device reference manual.
 */
typedef enum
{
    PCC_FTFFC = 0x80,
    PCC_DMAMUX = 0x84,
    PCC_FLEXCAN0 = 0x90,
    PCC_FLEXCAN1 = 0x94,
    PCC_FTM3 = 0x98,
    PCC_ADC1 = 0x9C,
    PCC_FLEXCAN2 = 0xAC,
    PCC_LPSPI0 = 0xB0,
    PCC_LPSPI1 = 0xB4,
    PCC_LPSPI2 = 0xB8,
    PCC_PDB1 = 0xC4,
    PCC_CRC = 0xC8,
    PCC_PDB0 = 0xD8,
    PCC_LPIT = 0xDC,
    PCC_FTM0 = 0xE0,
    PCC_FTM1 = 0xE4,
    PCC_FTM2 = 0xE8,
    PCC_ADC0 = 0xEC,
    PCC_RTC = 0xF4,
    PCC_LPTMR0 = 0x100,
    PCC_PORTA = 0x124,
    PCC_PORTB = 0x128,
    PCC_PORTC = 0x12C,
    PCC_PORTD = 0x130,
    PCC_PORTE = 0x134,
    PCC_SAI0 = 0x150,
    PCC_SAI1 = 0x154,
    PCC_FLEXIO = 0x168,
    PCC_EWM = 0x184,
    PCC_LPI2C0 = 0x198,
    PCC_LPI2C1 = 0x19C,
    PCC_LPUART0 = 0x1A8,
    PCC_LPUART1 = 0x1AC,
    PCC_LPUART2 = 0x1B0,
    PCC_FTM4 = 0x1B8,
    PCC_FTM5 = 0x1BC,
    PCC_FTM6 = 0x1C0,
    PCC_FTM7 = 0x1C4,
    PCC_CMP0 = 0x1CC,
    PCC_QSPI = 0x1D8,
    PCC_ENET = 0x1E4
} PCC_PERIPHERALS_t;

/**
 * @brief Peripheral clock source selection (PCS field) enumeration.
 *
 * Symbols map to potential selectable clock sources for peripherals. Some entries
 * may not be valid for all peripherals; user must ensure compatibility.
 */
typedef enum
{
    PCC_PCS_SPLLDIV1_CLK,
    PCC_PCS_SPLLDIV2_CLK,
    PCC_PCS_FIRCDIV1_CLK,
    PCC_PCS_FIRCDIV2_CLK,
    PCC_PCS_SIRCDIV1_CLK,
    PCC_PCS_FIRCDIV2_CLK,
    PCC_PCS_SOSCDIV1_CLK,
    PCC_PCS_SOSCDIV2_CLK,
    PCC_PCS_LPO128K_CLK
} PCC_PCS_t;

/**
 * @brief Peripheral clock divider (PCD field) divide ratios.
 *
 * Effective divide value = enumerator numeric value + 1.
 */
typedef enum
{
    PCD_DIVIDE_BY_1 = 0,
    PCD_DIVIDE_BY_2,
    PCD_DIVIDE_BY_3,
    PCD_DIVIDE_BY_4,
    PCD_DIVIDE_BY_5,
    PCD_DIVIDE_BY_6,
    PCD_DIVIDE_BY_7,
    PCD_DIVIDE_BY_8
} PCC_PCD_DIVIDE_t;

/**
 * @brief Fraction (FRAC field) selection for peripheral clock.
 *
 * FRAC_0 No fractional addition (divide as-is).
 * FRAC_1 Adds 0.5 step (when supported) to modify effective clock.
 */
typedef enum
{
    FRAC_0,
    FRAC_1
} PCC_FRAC_VALUE_t;

/**
 * @brief Composite peripheral clock configuration descriptor.
 *
 * Encapsulates source selector, integer divider and optional fractional control.
 * Used by PCC_SetClockConfiguration() (currently stub / not implemented).
 */
typedef struct
{
    PCC_PCS_t source;
    PCC_PCD_DIVIDE_t divide;
    PCC_FRAC_VALUE_t fraction;
} pcc_clock_config_t;

/*******************************************************************************
 *                                      API
 ******************************************************************************/

/**
 * @brief Configure peripheral clock source/divider (stub – not yet implemented).
 *
 * Placeholder API. Future implementation will write PCS/PCD/FRAC fields for the
 * specified peripheral PCCn register. Currently performs no action.
 *
 * @param peripheral Peripheral identifier (PCC_PERIPHERALS_t).
 * @param config Desired clock configuration.
 */
void PCC_SetClockConfiguration(PCC_PERIPHERALS_t peripheral, pcc_clock_config_t config);

/**
 * @brief Enable (gate on) the clock for a peripheral.
 *
 * Sets the CGC bit after validating the PR (presence) bit. Returns error if the
 * peripheral is not present or parameter invalid.
 *
 * @param peripheral Peripheral identifier (offset enumeration).
 * @return PCC_STATUS_t SUCCESS on success, ERROR otherwise.
 */
PCC_STATUS_t PCC_EnableClock(PCC_PERIPHERALS_t peripheral);

/**
 * @brief Disable (gate off) the clock for a peripheral.
 *
 * Clears the CGC bit if the peripheral is present (PR bit set). Returns error on
 * invalid peripheral.
 *
 * @param peripheral Peripheral identifier.
 * @return PCC_STATUS_t SUCCESS on success, ERROR otherwise.
 */
PCC_STATUS_t PCC_DisableClock(PCC_PERIPHERALS_t peripheral);

#ifdef __cplusplus
}
#endif

#endif  /* DRIVER_PCC_H_ */