#ifndef DRIVER_SCG_H_
#define DRIVER_SCG_H_

#include "Driver_Common.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 *                                  Definitions
 ******************************************************************************/
#define SCG_ENABLE		1
#define SCG_DISABLE 	0

/**
 * @brief SCG driver status codes returned by public APIs.
 * 
 * SCG_STATUS_SUCCESS  Operation completed successfully.
 * SCG_STATUS_TIMEOUT  Hardware did not reach the expected state before timeout.
 * SCG_STATUS_ERROR    Generic error (invalid parameter or unsupported state).
 */
typedef enum
{
	SCG_STATUS_SUCCESS = 0,
	SCG_STATUS_TIMEOUT = -1,
	SCG_STATUS_ERROR = -2
}SCG_STATUS_t;

/**
 * @brief System clock operating modes used when selecting the active configuration register.
 * 
 * SCG_HSRUN_MODE  High speed run mode (requires prior enabling in power management).
 * SCG_RUN_MODE    Normal run mode.
 * SCG_VLPR_MODE   Very low power run mode (reduced frequency & consumption).
 */
typedef enum
{
	SCG_NONE_MODE,
	SCG_HSRUN_MODE,
	SCG_RUN_MODE,
	SCG_VLPR_MODE
}SCG_CLOCK_MODE_t;

/**
 * @brief SCG system clock source selectors (values correspond to CSR SCS field encoding).
 * 
 * SCG_SOSC_CLK  System OSC (external source).
 * SCG_SIRC_CLK  Slow Internal RC oscillator.
 * SCG_FIRC_CLK  Fast Internal RC oscillator.
 * SCG_SPLL_CLK  System PLL (uses SOSC or FIRC as input depending on MCU config).
 */
typedef enum
{
	SCG_SOSC_CLK = 1U,
	SCG_SIRC_CLK= 2U,
	SCG_FIRC_CLK= 3U,
	SCG_SPLL_CLK = 6U
}SCG_CLOCK_SOURCE_t;

/**
 * @brief Core clock divider enumeration (encoded field value, not literal divide value minus 1).
 * 
 * Each value corresponds directly to the field written into DIVCORE. Effective divider is
 * (enum_value + 1) per reference manual specification.
 */
typedef enum
{
	SCG_DIV_CORE_BY_1 = 0,
	SCG_DIV_CORE_BY_2,
	SCG_DIV_CORE_BY_3,
	SCG_DIV_CORE_BY_4,
	SCG_DIV_CORE_BY_5,
	SCG_DIV_CORE_BY_6,
	SCG_DIV_CORE_BY_7,
	SCG_DIV_CORE_BY_8,
	SCG_DIV_CORE_BY_9,
	SCG_DIV_CORE_BY_10,
	SCG_DIV_CORE_BY_11,
	SCG_DIV_CORE_BY_12,
	SCG_DIV_CORE_BY_13,
	SCG_DIV_CORE_BY_14,
	SCG_DIV_CORE_BY_15,
	SCG_DIV_CORE_BY_16
}SCG_DIV_CORE_t;

/**
 * @brief Bus clock divider enumeration for DIVBUS field (effective divide = value + 1).
 */
typedef enum
{
	SCG_DIV_BUS_BY_1 = 0,
	SCG_DIV_BUS_BY_2,
	SCG_DIV_BUS_BY_3,
	SCG_DIV_BUS_BY_4,
	SCG_DIV_BUS_BY_5,
	SCG_DIV_BUS_BY_6,
	SCG_DIV_BUS_BY_7,
	SCG_DIV_BUS_BY_8,
	SCG_DIV_BUS_BY_9,
	SCG_DIV_BUS_BY_10,
	SCG_DIV_BUS_BY_11,
	SCG_DIV_BUS_BY_12,
	SCG_DIV_BUS_BY_13,
	SCG_DIV_BUS_BY_14,
	SCG_DIV_BUS_BY_15,
	SCG_DIV_BUS_BY_16
}SCG_DIV_BUS_t;

/**
 * @brief Slow clock divider enumeration for DIVSLOW field (effective divide = value + 1).
 */
typedef enum
{
	SCG_DIV_SLOW_BY_1 = 0,
	SCG_DIV_SLOW_BY_2,
	SCG_DIV_SLOW_BY_3,
	SCG_DIV_SLOW_BY_4,
	SCG_DIV_SLOW_BY_5,
	SCG_DIV_SLOW_BY_6,
	SCG_DIV_SLOW_BY_7,
	SCG_DIV_SLOW_BY_8,
	SCG_DIV_SLOW_BY_9,
	SCG_DIV_SLOW_BY_10,
	SCG_DIV_SLOW_BY_11,
	SCG_DIV_SLOW_BY_12,
	SCG_DIV_SLOW_BY_13,
	SCG_DIV_SLOW_BY_14,
	SCG_DIV_SLOW_BY_15,
	SCG_DIV_SLOW_BY_16
}SCG_DIV_SLOW_t;

/**
 * @brief Peripheral clock output divider selection (for optional derived outputs).
 * 
 * SCG_OUTPUT_DISABLED Output disabled.
 * SCG_DIVIDE_BY_X     Divide-by-N selection for routed peripheral clock outputs.
 */
typedef enum
{
	SCG_OUTPUT_DISABLED,
	SCG_DIVIDE_BY_1,
	SCG_DIVIDE_BY_2,
	SCG_DIVIDE_BY_3,
	SCG_DIVIDE_BY_4,
	SCG_DIVIDE_BY_5,
	SCG_DIVIDE_BY_6,
	SCG_DIVIDE_BY_7,
}SCG_PERIPH_CLOCK_DIVIDE;

/**
 * @brief Composite configuration for a system clock mode transition.
 * 
 * The structure encapsulates the target mode register (mode), the system source (src),
 * and the three primary divider selections for core, bus and slow clock domains.
 */
typedef struct
{
	SCG_CLOCK_MODE_t mode;
	SCG_CLOCK_SOURCE_t src;
	SCG_DIV_CORE_t div_core;
	SCG_DIV_BUS_t div_bus;
	SCG_DIV_SLOW_t div_slow;
}scg_config_struct_t;

/*******************************************************************************
 *                                      API
 ******************************************************************************/

/**
 * @brief Enable (initialize) a specific SCG clock source and wait until it becomes valid.
 * 
 * The function sets the enable bit of the selected source control register and polls
 * its valid flag until asserted or a timeout occurs.
 *
 * @param src Clock source to enable.
 * @return SCG_STATUS_t SUCCESS, TIMEOUT or ERROR if parameter invalid.
 */
SCG_STATUS_t SCG_SourceInit(SCG_CLOCK_SOURCE_t src);

/**
	* @brief Apply a system clock configuration (mode, source, dividers) atomically.
	* 
	* Performs: validate source, assemble register value, write to the appropriate mode
	* configuration register (HCCR/RCCR/VCCR) and poll until the system clock source shows
	* active. Assumes the source was previously enabled via SCG_SourceInit.
	*
	* @param config Pointer to configuration descriptor (must not be NULL).
	* @return SCG_STATUS_t SUCCESS / TIMEOUT / ERROR.
	*/
SCG_STATUS_t SCG_SetSystemClockConfig(scg_config_struct_t *config);

/**
 * @brief Preset: RUN mode using FIRC (nominal 48MHz) with fixed divider pattern.
 */
SCG_STATUS_t SCG_FIRC_SlowRun_48Mhz(void);

/**
 * @brief Preset: RUN mode using SPLL targeting ~80MHz core clock.
 */
SCG_STATUS_t SCG_SPLL_NormalRun_80Mhz(void);

/**
 * @brief Preset: RUN mode using SPLL targeting ~64MHz core clock.
 */
SCG_STATUS_t SCG_SPLL_NormalRun_64Mhz(void);

/**
 * @brief Preset: HSRUN mode using SPLL targeting ~112MHz core clock.
 */
SCG_STATUS_t SCG_SPLL_HSRun_112Mhz(void);

/**
 * @brief Preset: HSRUN mode using SPLL targeting ~80MHz core clock.
 */
SCG_STATUS_t SCG_SPLL_HSRun_80Mhz(void);

/**
 * @brief Preset: VLPR mode using SIRC targeting ~4MHz core clock.
 */
SCG_STATUS_t SCG_SIRC_VLPRRun_4Mhz(void);


/**
 * @brief Retrieve the current bus clock frequency (Hz).
 * 
 * Implementation pending – will derive from active source + divider settings.
 *
 * @return uint32_t Bus frequency in Hz (placeholder until implemented).
 */
uint32_t SCG_GetBusClock(void);

/**
 * @brief Retrieve the current core clock frequency (Hz).
 * 
 * Implementation pending – will compute based on CSR SCS field and core divider.
 *
 * @return uint32_t Core frequency in Hz (placeholder until implemented).
 */
uint32_t SCG_GetCoreClock(void);

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_SCG_H_ */
