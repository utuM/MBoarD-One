/**
  * @filename Gpio.h
  * @author   utuM
  * @date     16.06.2019 (creation)
  * @version  0.9.0
  * @brief    Low level (LL) general purpose input/output (GPIO) pins driver.
  *           Provides control of MCU pins with input/output pin
  *           initialisation posibilities, and could be used to
  *           initialise pins with alternate functions for any communication
  *           interface.
  *           Input, output and alternate function pins could be enabled
  *           with different functions. To initialise input pins used
  *           'GPIO_InitInput(...)', for output - use 'GPIO_InitOutput(...)'.
  *           Like an initialisation options, pull up/down, output type and
  *           switching speed could be used as function input parameters.
  *           Alternate function pin is not recommended to use separately
  *           from any of the interfaces.
  *           Current LL-library could be used for STM32H743xx MCU family.
  **/

#ifndef __DRV_PERIPHERAL_GPIO_H__
#define __DRV_PERIPHERAL_GPIO_H__

#include "stm32h743xx.h"
#include "Typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

/**
  * @brief GPIO handlers for STM32H753xx series MCU.
  **/
enm
{
    GPIOA_PORT        = 0x00U,
    GPIOB_PORT        = 0x01U,
    GPIOC_PORT        = 0x02U,
    GPIOD_PORT        = 0x03U,
    GPIOE_PORT        = 0x04U,
    GPIOF_PORT        = 0x05U,
    GPIOG_PORT        = 0x06U,
    GPIOH_PORT        = 0x07U,
    GPIOI_PORT        = 0x08U,
    GPIOJ_PORT        = 0x09U,
    GPIOK_PORT        = 0x0AU
} GPIO_Port;

/**
  * @brief GPIO pins for STM32H753xx series MCU.
  **/
enm
{
    GPIO_PIN0        = 0x00U,
    GPIO_PIN1        = 0x01U,
    GPIO_PIN2        = 0x02U,
    GPIO_PIN3        = 0x03U,
    GPIO_PIN4        = 0x04U,
    GPIO_PIN5        = 0x05U,
    GPIO_PIN6        = 0x06U,
    GPIO_PIN7        = 0x07U,
    GPIO_PIN8        = 0x08U,
    GPIO_PIN9        = 0x09U,
    GPIO_PIN10       = 0x0AU,
    GPIO_PIN11       = 0x0BU,
    GPIO_PIN12       = 0x0CU,
    GPIO_PIN13       = 0x0DU,
    GPIO_PIN14       = 0x0EU,
    GPIO_PIN15       = 0x0FU
} GPIO_Pin;

/**
  * @brief GPIO type to be initialised.
  **/
enm
{
    GPIO_INPUT    = 0x00U,
    GPIO_OUTPUT   = 0x01U,
    GPIO_ALT_FUNC = 0x02U
} GPIO_Type;

/**
  * @brief GPIO output level.
  **/
enm
{
    GPIO_RESET    = 0x00U,
    GPIO_SET      = 0x01U
} GPIO_Signal;

/**
  * @brief General purpose input pin pull type.
  **/
enm
{
    GPI_NO_PULL     = 0x00U,
    GPI_PULL_UP     = 0x01U,
    GPI_PULL_DOWN   = 0x02U,
    GPI_ISNT_IN_USE = 0x03U
} GPI_InPull;

/**
  * @brief General purpose output pin type.
  **/
enm
{
    GPO_PUSH_PULL   = 0x00U,
    GPO_OPEN_DRAIN  = 0x01U,
    GPO_ISNT_IN_USE = 0x02U
} GPO_OutType;

/**
  * @brief GPIO alternate functions values.
  **/
enm
{
    // Alternate for UART instances.
    GPIO_LPUART_AF3 = 0x03U,
    GPIO_UART1_AF4  = 0x04U,
    GPIO_UART4_AF6  = 0x06U,
    GPIO_UART1_AF7  = 0x07U,
    GPIO_UART2_AF7  = 0x07U,
    GPIO_UART3_AF7  = 0x07U,
    GPIO_UART6_AF7  = 0x07U,
    GPIO_UART7_AF7  = 0x07U,
    GPIO_UART4_AF8  = 0x08U,
    GPIO_UART5_AF8  = 0x08U,
    GPIO_UART8_AF8  = 0x08U,
    GPIO_LPUART_AF8 = 0x08U,
    GPIO_UART7_AF11 = 0x0BU,
    GPIO_UART5_AF14 = 0x0EU,
    GPIO_NO_FUNC    = 0xFFU
} GPIO_AltFunc;

/**
  * @brief GPIO speed.
  **/
enm
{
    GPIO_LOW_SPEED = 0x00U, ///<  2MHz speed.
    GPIO_MED_SPEED = 0x01U, ///< 10MHz speed.
    GPIO_HI_SPEED  = 0x03U  ///< 50MHz speed.
} GPIO_Speed;

/**
  * @brief GPIO handler errors.
  **/
enm
{
    GPIO_NOERROR     = 0x00U,
    GPIO_ISNT_INIT   = 0x01U,
    GPIO_UNKNOWN_PIN = 0x02U,
    GPIO_ISNT_OUTPUT = 0x03U
} GPIO_Error;

/**
  * @brief GPIO handler.
  **/
stc
{
    flg m_isInitialised;       ///< Complited initialisation flag.
    GPIO_TypeDef* m_pInstance; ///< Pointer to port instance.
    GPIO_Port m_port;          ///< GPIO port.
    GPIO_Pin m_pin;            ///< GPIO pin.
    GPIO_Type m_type;          ///< GPIO type: input or output.
    GPI_InPull m_pullType;     ///< Only for input pin: pull type.
    GPO_OutType m_outType;     ///< Only for output pin: type.
    GPIO_AltFunc m_func;       ///< Alternate function for current pin.
    GPIO_Signal m_state;       ///< Current signal level (mostly for output).
} GPIO_Handler;

#pragma pack(pop)

GPIO_Handler* GPIO_InitInput(GPIO_Port port, GPIO_Pin pin, GPI_InPull pull,
                             GPIO_Speed speed);
GPIO_Handler* GPIO_InitOutput(GPIO_Port port, GPIO_Pin pin, GPO_OutType type,
                              GPIO_Speed speed, GPIO_Signal state);
GPIO_Handler* GPIO_InitAltFunc(GPIO_Port port, GPIO_Pin pin, GPIO_AltFunc func,
                               GPIO_Speed speed);
u8 GPIO_GetInput(GPIO_Port port, GPIO_Pin pin);
GPIO_Error GPIO_SetOutput(GPIO_Port port, GPIO_Pin pin, GPIO_Signal value);
GPIO_Error GPIO_ToggleOutput(GPIO_Port port, GPIO_Pin pin);
GPIO_Error GPIO_Deinit(GPIO_Port port, GPIO_Pin pin);

#ifdef __cplusplus
}
#endif

#endif // __DRV_PERIPHERAL_GPIO_H__