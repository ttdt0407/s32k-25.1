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
 * @brief 
 * 
 */
typedef enum
{
	SCG_STATUS_SUCCESS,
	SCG_STATUS_TIMEOUT,
	SCG_STATUS_ERROR
}SCG_STATUS_t;

/**
 * @brief 
 * 
 */
typedef enum
{
	SCG_NONE_MODE,
	SCG_HSRUN_MODE,
	SCG_RUN_MODE,
	SCG_VLPR_MODE
}SCG_CLOCK_MODE_t;

/**
 * @brief 
 * 
 */
typedef enum
{
	SCG_SOSC_CLK = 1U,
	SCG_SIRC_CLK= 2U,
	SCG_FIRC_CLK= 3U,
	SCG_SPLL_CLK = 6U
}SCG_CLOCK_SOURCE_t;

/**
 * @brief 
 * 
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
 * @brief 
 * 
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
 * @brief 
 * 
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
 * @brief 
 * 
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
 * @brief 
 * 
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
  * @brief 
  * 
  * @param config 
  * @return SCG_STATUS_t 
  */
SCG_STATUS_t  SCG_SetSystemClockConfig(scg_config_struct_t *config);

/**
 * @brief 
 * 
 * @param src 
 * @return SCG_STATUS_t 
 */
SCG_STATUS_t  SCG_SourceEnable(SCG_CLOCK_SOURCE_t src);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t      SCG_GetBusClock(void);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t      SCG_GetCoreClock(void);

#ifdef __cplusplus
}
#endif

#endif /* DRIVER_SCG_H_ */
