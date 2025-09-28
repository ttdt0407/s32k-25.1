/*
 * Copyright (c) 2023 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Driver_GPIO.h"
#include "S32k144.h"

// Pin mapping
#define GPIO_MAX_PINS           88U
#define PIN_IS_AVAILABLE(n)     ((n) <= GPIO_MAX_PINS)

// Array of callback function
ARM_GPIO_SignalEvent_t cbFunc[GPIO_MAX_PINS];

// Get value prototype
static GPIO_Type *GPIO_GetValue(ARM_GPIO_Pin_t pin);

// Get value of pins
static GPIO_Type *GPIO_GetValue (ARM_GPIO_Pin_t pin)
{
  int result = NULL;
  if (PIN_IS_AVAILABLE(pin))
  {
    int gpio = pin / 18;
    switch (gpio)
    {
      case 0:
    	  result = IP_PTA;
      case 1:
    	  result = IP_PTB;
      case 2:
    	  result = IP_PTC;
      case 3:
    	  result = IP_PTD;
      case 4:
    	  result = IP_PTE;
      default:
    	  result = NULL;
      }
  }
  else
  {
    return result;
  }
}
// Get value prototype
static GPIO_Type *PORT_GetValue(ARM_GPIO_Pin_t pin);

// Get value of pins
static GPIO_Type *PORT_GetValue (ARM_GPIO_Pin_t pin)
{
  int result = NULL;
  if (PIN_IS_AVAILABLE(pin))
  {
    int port = pin / 18;
    switch (port)
    {
      case 0:
    	  result = IP_PORTA;
      case 1:
    	  result = IP_PORTB;
      case 2:
    	  result = IP_PORTC;
      case 3:
    	  result = IP_PORTD;
      case 4:
    	  result = IP_PORTE;
      default:
    	  result = NULL;
      }
  }
  else
  {
    return result;
  }
}

// Setup GPIO Interface
static int32_t GPIO_Setup (ARM_GPIO_Pin_t pin, ARM_GPIO_SignalEvent_t cb_event) {
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin)) {
    cbFunc[pin] = cb_event;
  }
  else
  {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

// Set GPIO Direction
static int32_t GPIO_SetDirection (ARM_GPIO_Pin_t pin, ARM_GPIO_DIRECTION direction) {
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin)) {
    GPIO_Type *gpio = GPIO_GetValue(pin);
    ARM_GPIO_Pin_t pinValue = (ARM_GPIO_Pin_t)(pin % 18);
    switch (direction) {
      case ARM_GPIO_INPUT:
        gpio->PDDR &= ~((ARM_GPIO_Pin_t)((ARM_GPIO_Pin_t)0x1U << pinValue));
        break;
      case ARM_GPIO_OUTPUT:
        gpio->PDDR |= ((ARM_GPIO_Pin_t)((ARM_GPIO_Pin_t)direction << pinValue));
        break;
      default:
        result = ARM_DRIVER_ERROR_PARAMETER;
        break;
    }
  } else {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

// Set GPIO Output Mode
static int32_t GPIO_SetOutputMode (ARM_GPIO_Pin_t pin, ARM_GPIO_OUTPUT_MODE mode) {
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin)) {

    PORT_Type *port = PORT_GetValue(pin);
    ARM_GPIO_Pin_t pinValue = pin % 18;

    switch (mode) {

      case ARM_GPIO_PUSH_PULL:
      /* Config gpio */
        port->PCR[pinValue] &= (~PORT_PCR_MUX_MASK) | (PORT_PCR_MUX(1));
        break;

      case ARM_GPIO_OPEN_DRAIN:
      /* S32k1xx does not support config open-drain mode as a general gpio configuration */
        break;

      default:
        result = ARM_DRIVER_ERROR_PARAMETER;
        break;
    }
  }
  else {

    result = ARM_GPIO_ERROR_PIN;

  }

  return result;
}

// Set GPIO Pull Resistor
static int32_t GPIO_SetPullResistor (ARM_GPIO_Pin_t pin, ARM_GPIO_PULL_RESISTOR resistor) {
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin)) {

    PORT_Type *port = PORT_GetValue(pin);
    ARM_GPIO_Pin_t pinValue = pin % 18;

    switch (resistor)
    {
    case ARM_GPIO_PULL_NONE:
      /* Pull disable */
      port->PCR[pinValue] &= ~PORT_PCR_PE_MASK | PORT_PCR_PE(0);
      break;

    case ARM_GPIO_PULL_UP:
      /* Pull enable*/
      port->PCR[pinValue] &= ~PORT_PCR_PE_MASK | PORT_PCR_PE(1);

      /* Pull up */
      port->PCR[pinValue] &= ~PORT_PCR_PS_MASK | PORT_PCR_PS(1);
      break;

    case ARM_GPIO_PULL_DOWN:
      /* Pull enable*/
      port->PCR[pinValue] &= ~PORT_PCR_PE_MASK | PORT_PCR_PE(1);

      /* Pull down */
      port->PCR[pinValue] &= ~PORT_PCR_PS_MASK | PORT_PCR_PS(0);
      break;

    default:
      result = ARM_DRIVER_ERROR_PARAMETER;
      break;
    }

  } else {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

// Set GPIO Event Trigger
static int32_t GPIO_SetEventTrigger (ARM_GPIO_Pin_t pin, ARM_GPIO_EVENT_TRIGGER trigger) {
  int32_t result = ARM_DRIVER_OK;

  if (PIN_IS_AVAILABLE(pin)) {

    PORT_Type *port = PORT_GetValue(pin);
    ARM_GPIO_Pin_t pinValue = pin % 18;
    switch (trigger)
    {
    case ARM_GPIO_TRIGGER_NONE:
      port->PCR[pinValue] &= ~PORT_PCR_IRQC_MASK | PORT_PCR_IRQC(0);
      break;
    case ARM_GPIO_TRIGGER_RISING_EDGE:
      port->PCR[pinValue]  &= ~PORT_PCR_IRQC_MASK | PORT_PCR_IRQC(9);
      break;
    case ARM_GPIO_TRIGGER_FALLING_EDGE:
      port->PCR[pinValue]  &= ~PORT_PCR_IRQC_MASK | PORT_PCR_IRQC(10);
      break;
    case ARM_GPIO_TRIGGER_EITHER_EDGE:
      port->PCR[pinValue]  &= ~PORT_PCR_IRQC_MASK | PORT_PCR_IRQC(11);
      break;
    default:
      result = ARM_DRIVER_ERROR_PARAMETER;
      break;
    }
  } else {
    result = ARM_GPIO_ERROR_PIN;
  }

  return result;
}

// Set GPIO Output Level
static void GPIO_SetOutput (ARM_GPIO_Pin_t pin, uint32_t val) {

  if (PIN_IS_AVAILABLE(pin)) {
    GPIO_Type *gpio = GPIO_GetValue(pin);
    ARM_GPIO_Pin_t pinValue = pin % 18;
    if (val)
    {
      gpio->PSOR |= (1U << pinValue);
    }
    else
    {
      gpio->PCOR |= (1U << pinValue);
    }
  }
}

// Get GPIO Input Level
static uint32_t GPIO_GetInput (ARM_GPIO_Pin_t pin) {
  uint32_t val = 0U;

  if (PIN_IS_AVAILABLE(pin)) {
    GPIO_Type *gpio = GPIO_GetValue(pin);
    ARM_GPIO_Pin_t pinValue = pin % 18;
    val = ((uint32_t)(gpio->PDIR >> pinValue) & (uint32_t)0x1U);
  }
  return val;
}


// GPIO Driver access structure
ARM_DRIVER_GPIO Driver_GPIO0 = {
  GPIO_Setup,
  GPIO_SetDirection,
  GPIO_SetOutputMode,
  GPIO_SetPullResistor,
  GPIO_SetEventTrigger,
  GPIO_SetOutput,
  GPIO_GetInput
};
