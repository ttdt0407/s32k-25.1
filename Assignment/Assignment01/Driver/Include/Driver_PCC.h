#ifndef DRIVER_PCC_H_
#define DRIVER_PCC_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "Driver_Common.h"

/**
 * @brief Enumeration for available PCC ports (PORTA to PORTE)
 *
 * Used to select the port for clock initialization.
 */
typedef enum
{
    PA,
    PB,
    PC,
    PD,
    PE
} ARM_PCC_PORT;

/**
 * @brief PCC driver access structure
 *
 * Contains pointer to the Initialize function for enabling port clocks.
 */
typedef struct __ARM_DRIVER_PCC
{
    int32_t (*Initialize)(ARM_PCC_PORT port);
} ARM_DRIVER_PCC;


#ifdef __cplusplus
}
#endif

#endif /* DRIVER_PCC_H_ */