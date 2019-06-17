/**
  * @filename Gpio.c
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

#include "GpioC.h"

#include <string.h>

// RCC generating flags.
#define RCC_GPIOA_CLK_ENABLED      0
#define RCC_GPIOB_CLK_ENABLED      0
#define RCC_GPIOC_CLK_ENABLED      0
#define RCC_GPIOD_CLK_ENABLED      0
#define RCC_GPIOE_CLK_ENABLED      0
#define RCC_GPIOF_CLK_ENABLED      0
#define RCC_GPIOG_CLK_ENABLED      0
#define RCC_GPIOH_CLK_ENABLED      0
#define RCC_GPIOI_CLK_ENABLED      0
#define RCC_GPIOJ_CLK_ENABLED      0
#define RCC_GPIOK_CLK_ENABLED      0
// Other definitions.
#define TOTAL_GPIO_PORTS_AMOUNT   11
#define TOTAL_PORT_PINS           16
// Global static GPIOs array.
static GPIO_Handler s_gpio[TOTAL_GPIO_PORTS_AMOUNT][TOTAL_PORT_PINS] = {null};

static GPIO_TypeDef* sGPIO_EnablePort(GPIO_Port port);

/**
  * @brief Port enabling.
  * @param[i] port - port clock generating to be enabled.
  * @return Pointer to global port object where selected pin is placed.
  **/
static GPIO_TypeDef* sGPIO_EnablePort(GPIO_Port port)
{
    GPIO_TypeDef* l_gpio = 0;
    switch (port)
    {
        case GPIOA_PORT:
#if (RCC_GPIOA_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOAEN;
            #undef  RCC_GPIOA_CLK_ENABLED
            #define RCC_GPIOA_CLK_ENABLED 1
#endif
            l_gpio = GPIOA;
            break;
            
        case GPIOB_PORT:
#if (RCC_GPIOB_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOBEN;
            #undef  RCC_GPIOB_CLK_ENABLED
            #define RCC_GPIOB_CLK_ENABLED 1
#endif
            l_gpio = GPIOB;
            break;
            
        case GPIOC_PORT:
#if (RCC_GPIOC_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOCEN;
            #undef  RCC_GPIOC_CLK_ENABLED
            #define RCC_GPIOC_CLK_ENABLED 1
#endif
            l_gpio = GPIOC;
            break;
            
        case GPIOD_PORT:
#if (RCC_GPIOD_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIODEN;
            #undef  RCC_GPIOD_CLK_ENABLED
            #define RCC_GPIOD_CLK_ENABLED 1
#endif
            l_gpio = GPIOD;
            break;
            
        case GPIOE_PORT:
#if (RCC_GPIOE_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOEEN;
            #undef  RCC_GPIOE_CLK_ENABLED
            #define RCC_GPIOE_CLK_ENABLED 1
#endif
            l_gpio = GPIOE;
            break;
            
        case GPIOF_PORT:
#if (RCC_GPIOF_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOFEN;
            #undef  RCC_GPIOF_CLK_ENABLED
            #define RCC_GPIOF_CLK_ENABLED 1
#endif
            l_gpio = GPIOF;
            break;
            
        case GPIOG_PORT:
#if (RCC_GPIOG_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOGEN;
            #undef  RCC_GPIOG_CLK_ENABLED
            #define RCC_GPIOG_CLK_ENABLED 1
#endif
            l_gpio = GPIOG;
            break;
            
        case GPIOH_PORT:
#if (RCC_GPIOH_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOHEN;
            #undef  RCC_GPIOH_CLK_ENABLED
            #define RCC_GPIOH_CLK_ENABLED 1
#endif
            l_gpio = GPIOH;
            break;
            
        case GPIOI_PORT:
#if (RCC_GPIOI_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOIEN;
            #undef  RCC_GPIOI_CLK_ENABLED
            #define RCC_GPIOI_CLK_ENABLED 1
#endif
            l_gpio = GPIOI;
            break;
            
        case GPIOJ_PORT:
#if (RCC_GPIOJ_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOJEN;
            #undef  RCC_GPIOJ_CLK_ENABLED
            #define RCC_GPIOJ_CLK_ENABLED 1
#endif
            l_gpio = GPIOJ;
            break;
            
        case GPIOK_PORT:
#if (RCC_GPIOK_CLK_ENABLED == 0)
            RCC->AHB4ENR |= RCC_AHB4ENR_GPIOKEN;
            #undef  RCC_GPIOK_CLK_ENABLED
            #define RCC_GPIOK_CLK_ENABLED 1
#endif
            l_gpio = GPIOK;
            break;
    }  
    
    return l_gpio;
}

/**
  * @brief Input pin initialisation.
  * @param[i] port      - pin port.
  * @param[i] pin       - pin number by order.
  * @param[i] pull      - pull direction.
  * @param[i] speed     - pin state switching speed.
  * @param[i] pCallback - pointer to function which will be trigger by external
  *                       interrupt.
  * @return Pointer to input pin object that is initialised.
  **/
GPIO_Handler* GPIO_InitInput(GPIO_Port port, GPIO_Pin pin,
                             GPI_InPull pull, GPIO_Speed speed)
{
    // Prepare objects before initialisation.
    memset(&s_gpio[(u8)port][(u8)pin], null, sizeof(GPIO_Handler));
    // Prepare value for input type.
    GPIO_TypeDef* l_gpio = sGPIO_EnablePort(port);
    // Configure pin as input.
    l_gpio->MODER   =  l_gpio->MODER   & (0xFFFFFFFC << ((u8)pin * 2));
    // Configure input speed.
    l_gpio->OSPEEDR = (l_gpio->OSPEEDR & (0xFFFFFFFC << ((u8)pin * 2))) | 
                       ((u32)speed << ((u8)pin * 2));
    // Configure input pull.
    l_gpio->PUPDR   = (l_gpio->PUPDR   & (0xFFFFFFFC << ((u8)pin * 2))) |
                       ((u32)pull  << ((u8)pin * 2));
    // Prepare pin structure object.
    s_gpio[(u8)port][(u8)pin].m_pInstance = l_gpio;
    s_gpio[(u8)port][(u8)pin].m_port = port;
    s_gpio[(u8)port][(u8)pin].m_pin = pin;
    s_gpio[(u8)port][(u8)pin].m_type = GPIO_INPUT;
    s_gpio[(u8)port][(u8)pin].m_pullType = pull;
    s_gpio[(u8)port][(u8)pin].m_outType = GPO_ISNT_IN_USE;
    s_gpio[(u8)port][(u8)pin].m_state = GPIO_RESET;
    s_gpio[(u8)port][(u8)pin].m_func = GPIO_NO_FUNC;
    s_gpio[(u8)port][(u8)pin].m_isInitialised = yes;

    return &s_gpio[(u8)port][(u8)pin];
}

/**
  * @brief Output pin initialisation.
  * @param[i] port  - pin port.
  * @param[i] pin   - pin number by order.
  * @param[i] type  - output type.
  * @param[i] speed - pin state switching speed.
  * @param[i] state - default signal state.
  * @return Pointer to output pin object that is initialised.
  **/
GPIO_Handler* GPIO_InitOutput(GPIO_Port port, GPIO_Pin pin,
                              GPO_OutType type, GPIO_Speed speed,
                              GPIO_Signal state)
{
    // Prepare object before initialisation.
    memset(&s_gpio[(u8)port][(u8)pin], null, sizeof(GPIO_Handler));
    // Prepare value for output type.
    GPIO_TypeDef* l_gpio = sGPIO_EnablePort(port);
    // Configure pin as output.
    l_gpio->MODER   =  l_gpio->MODER   & (0xFFFFFFFD << ((u8)pin * 2));
    // Configure output speed.
    l_gpio->OSPEEDR = (l_gpio->OSPEEDR & (0xFFFFFFFC << ((u8)pin * 2))) |
                       ((u32)speed << ((u8)pin * 2));
    // Configure output type.
    l_gpio->OTYPER  = (l_gpio->OTYPER  & (0x0000FFFE <<  (u8)pin)) |
                       ((u32)type  << ((u8)pin * 2));
    // Set start value.
    l_gpio->ODR = (l_gpio->ODR & (0x0000FFFE << (u8)pin)) | 
                  ((u16)state << (u8)pin);
    // Prepare pin structure object.
    s_gpio[(u8)port][(u8)pin].m_pInstance = l_gpio;
    s_gpio[(u8)port][(u8)pin].m_port = port;
    s_gpio[(u8)port][(u8)pin].m_pin = pin;
    s_gpio[(u8)port][(u8)pin].m_type = GPIO_OUTPUT;
    s_gpio[(u8)port][(u8)pin].m_pullType = GPI_ISNT_IN_USE;
    s_gpio[(u8)port][(u8)pin].m_outType = type;
    s_gpio[(u8)port][(u8)pin].m_state = state;
    s_gpio[(u8)port][(u8)pin].m_func = GPIO_NO_FUNC;
    s_gpio[(u8)port][(u8)pin].m_isInitialised = yes;
    
    return &s_gpio[(u8)port][(u8)pin];
}

/**
  * @brief Alternate function pin initialisation.
  *        Use for interfaces.
  * @param[i] port  - pin port.
  * @param[i] pin   - pin number by order.
  * @param[i] func  - alternate function.
  * @param[i] speed - pin state switching speed.
  * @return 0x00 or 0x01     - current pin state;
  *         GPIO_UNKNOWN_PIN - pin isn't initialised or pin isn't input typed.
  **/
GPIO_Handler* GPIO_InitAltFunc(GPIO_Port port, GPIO_Pin pin, GPIO_AltFunc func,
                               GPIO_Speed speed)
{
    // Prepare object before initialisation.
    memset(&s_gpio[(u8)port][(u8)pin], null, sizeof(GPIO_Handler));
    // Prepare value for output type.
    GPIO_TypeDef* l_gpio = sGPIO_EnablePort(port);
    // Configure pin as alternate function pin.
    l_gpio->MODER   =  l_gpio->MODER   & (0xFFFFFFFE << ((u8)pin * 2));
    // Configure output speed.
    l_gpio->OSPEEDR = (l_gpio->OSPEEDR & (0xFFFFFFFC << ((u8)pin * 2))) |
                       ((u32)speed << ((u8)pin * 2));
    // Configure alternate function.
    u8 l_afrIndex = 0x01 & ((u8)pin >> 3);
    u8 l_index = (u8)pin;
    if (l_index >= 8)
    {
        l_index -= 8;
    }
    l_gpio->AFR[l_afrIndex] = (l_gpio->AFR[l_afrIndex] &
                              (0xFFFFFFF0 << (4 * l_index))) |
                              ((u32)func << (4 * l_index));
    // Prepare pin structure object.
    s_gpio[(u8)port][(u8)pin].m_pInstance = l_gpio;
    s_gpio[(u8)port][(u8)pin].m_port = port;
    s_gpio[(u8)port][(u8)pin].m_pin = pin;
    s_gpio[(u8)port][(u8)pin].m_type = GPIO_ALT_FUNC;
    s_gpio[(u8)port][(u8)pin].m_pullType = GPI_ISNT_IN_USE;
    s_gpio[(u8)port][(u8)pin].m_outType = GPO_ISNT_IN_USE;
    s_gpio[(u8)port][(u8)pin].m_state = GPIO_RESET;
    s_gpio[(u8)port][(u8)pin].m_func = func;
    s_gpio[(u8)port][(u8)pin].m_isInitialised = yes;
    
    return &s_gpio[(u8)port][(u8)pin];
}

/**
  * @brief Read current input pin state.
  * @param[i] port  - pin port.
  * @param[i] pin   - pin number by order.
  * @return 0x00 or 0x01     - current pin state;
  *         GPIO_UNKNOWN_PIN - pin isn't initialised or pin isn't input typed.
  **/
u8 GPIO_GetInput(GPIO_Port port, GPIO_Pin pin)
{
    // Check if input handler and input state pointer are empty.
    if (!s_gpio[(u8)port][(u8)pin].m_isInitialised ||
         s_gpio[(u8)port][(u8)pin].m_type != GPIO_INPUT)
    {
        return GPIO_UNKNOWN_PIN;
    }
    // Get input value.
    s_gpio[(u8)port][(u8)pin].m_state = 0x00;
    if (s_gpio[(u8)port][(u8)pin].m_pInstance->IDR & (0x00000001 << pin))
    {
        s_gpio[(u8)port][(u8)pin].m_state = 0x01;
    }
    
    return s_gpio[(u8)port][(u8)pin].m_state;
}

/**
  * @brief Set current output signal level.
  * @param[i] port   - pin port.
  * @param[i] pin    - pin number by order.
  * @param[i] output - preferred output signal level.  
  * @return GPIO_NOERROR     - output level is changed successfully;
  *         GPIO_UNKNOWN_PIN - pin isn't initialised or pin isn't output typed.
  **/
GPIO_Error GPIO_SetOutput(GPIO_Port port, GPIO_Pin pin, GPIO_Signal output)
{
    // Check if output handler is empty.
    if (!s_gpio[(u8)port][(u8)pin].m_isInitialised ||
         s_gpio[(u8)port][(u8)pin].m_type != GPIO_OUTPUT)
    {
        return GPIO_UNKNOWN_PIN;
    }
    // Set output signal level.
    GPIO_TypeDef* l_pInstance = s_gpio[(u8)port][(u8)pin].m_pInstance;
    s_gpio[(u8)port][(u8)pin].m_state = (u8)output;
    if (output == GPIO_SET)
    {
        l_pInstance->BSRR = (u32)output << (u8)pin;    
    }
    else
    {
        l_pInstance->BSRR = (u32)output << ((u8)pin + 16);  
    }
    
    return GPIO_NOERROR;
}

/**
  * @brief Toggle output signal level.
  * @param[i] port   - pin port.
  * @param[i] pin    - pin number by order.  
  * @return GPIO_NOERROR     - output level is changed successfully;
  *         GPIO_UNKNOWN_PIN - pin isn't initialised or pin isn't output typed.
  **/
GPIO_Error GPIO_ToggleOutput(GPIO_Port port, GPIO_Pin pin)
{
    // Check if output handler is empty.
    if (!s_gpio[(u8)port][(u8)pin].m_isInitialised ||
         s_gpio[(u8)port][(u8)pin].m_type != GPIO_OUTPUT)
    {
        return GPIO_UNKNOWN_PIN;
    }
    // Toggle output signal level.
    GPIO_TypeDef* l_pInstance = s_gpio[(u8)port][(u8)pin].m_pInstance;
    if (l_pInstance->ODR & (0x0001 << (u8)pin))
    {
        l_pInstance->BSRR = 0x00000001 << (u8)pin; 
    }
    else
    {
        l_pInstance->BSRR = 0x00000001 << ((u8)pin + 16); 
    }
    l_pInstance = 0;

    return GPIO_NOERROR;
}

/**
  * @brief Deinitialize input/output pin.
  * @param[i] port   - pin port.
  * @param[i] pin    - pin number by order.  
  * @return GPIO_NOERROR   - pin is deinitialised successful;
  *         GPIO_ISNT_INIT - pin isn't initialised yet.
  **/
GPIO_Error GPIO_Deinit(GPIO_Port port, GPIO_Pin pin)
{
    // Check if input handler is empty.
    if (!s_gpio[(u8)port][(u8)pin].m_isInitialised)
    {
        return GPIO_ISNT_INIT;
    }
    // Set pin into analog mode, set the speed into lowest possible speed, and
    // erase the pin object.
    GPIO_TypeDef* l_pInstance = s_gpio[(u8)port][(u8)pin].m_pInstance;
    l_pInstance->MODER = l_pInstance->MODER | ((u32)0x03 << ((u8)pin * 2));
    l_pInstance->OSPEEDR = l_pInstance->OSPEEDR & (0xFFFFFFFC << ((u8)pin * 2));
    memset(&s_gpio[(u8)port][(u8)pin], 0x00, sizeof(GPIO_Handler));

    return GPIO_NOERROR;
}