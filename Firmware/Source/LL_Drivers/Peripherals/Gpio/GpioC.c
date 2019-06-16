#include "GpioC.h"

#include <string.h>

#define TOTAL_GPIO_PORTS_AMOUNT 11
#define TOTAL_PINS_FOR_PORT     16

#define RCC_GPIOA_CLK_ENABLED 0
#define RCC_GPIOB_CLK_ENABLED 0
#define RCC_GPIOC_CLK_ENABLED 0
#define RCC_GPIOD_CLK_ENABLED 0
#define RCC_GPIOE_CLK_ENABLED 0
#define RCC_GPIOF_CLK_ENABLED 0
#define RCC_GPIOG_CLK_ENABLED 0
#define RCC_GPIOH_CLK_ENABLED 0
#define RCC_GPIOI_CLK_ENABLED 0
#define RCC_GPIOJ_CLK_ENABLED 0
#define RCC_GPIOK_CLK_ENABLED 0

static GPIO_Handler s_gpio[TOTAL_GPIO_PORTS_AMOUNT][TOTAL_PINS_FOR_PORT] = {0};
static GPIO_TypeDef* sGPIO_EnablePort(GPIO_Port port);

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

GPIO_Handler* GPIO_InitInput(GPIO_Port port, GPIO_Pin pin,
                             GPI_InPull pull, GPIO_Speed speed)
{
    // Prepare objects before initialisation.
    memset(&s_gpio[(u8)port][(u8)pin], 0x00, sizeof(GPIO_Handler));
    // Prepare value for input type.
    GPIO_TypeDef* l_gpio = sGPIO_EnablePort(port);
    u32 l_mode  = 0xFFFFFFFF & ( 0xFFFFFFFC << ((u8)pin * 2)),
        l_speed = 0xFFFFFFFF & ((0xFFFFFFFC | (u8)speed) << ((u8)pin * 2)),
        l_pull  = 0xFFFFFFFF & ((0xFFFFFFFC | (u8)pull)  << ((u8)pin * 2));
    // Configure pin as input.
    l_gpio->MODER &= l_mode;
    // Configure input speed.
    l_gpio->OSPEEDR &= l_speed;
    // Configure input pull.
    l_gpio->PUPDR &= l_pull;
    // Prepare pin structure object.
    s_gpio[(u8)port][(u8)pin].m_pInstance = l_gpio;
    s_gpio[(u8)port][(u8)pin].m_port = port;
    s_gpio[(u8)port][(u8)pin].m_pin = pin;
    s_gpio[(u8)port][(u8)pin].m_type = GPIO_INPUT;
    s_gpio[(u8)port][(u8)pin].m_pullType = pull;
    s_gpio[(u8)port][(u8)pin].m_outType = GPO_ISNT_IN_USE;
    s_gpio[(u8)port][(u8)pin].m_state = GPIO_RESET;

    return &s_gpio[(u8)port][(u8)pin];
}

GPIO_Handler* GPIO_InitOutput(GPIO_Port port, GPIO_Pin pin,
                              GPO_OutType type, GPIO_Speed speed,
                              GPIO_Signal value)
{
    // Prepare object before initialisation.
    memset(&s_gpio[(u8)port][(u8)pin], 0x00, sizeof(GPIO_Handler));
    // Prepare value for output type.
    GPIO_TypeDef* l_gpio = sGPIO_EnablePort(port);
    u32 l_mode  = 0xFFFFFFFF & ( 0xFFFFFFFD << ((u8)pin * 2)),
        l_speed = 0xFFFFFFFF & ((0xFFFFFFFC | (u8)speed) << ((u8)pin * 2)),
        l_type  = 0x0000FFFF & ((0x0000FFFE | (u8)type)  << (u8)pin);
    // Configure pin as output.
    l_gpio->MODER &= l_mode;
    // Configure output speed.
    l_gpio->OSPEEDR &= l_speed;
    // Configure output type.
    l_gpio->OTYPER &= l_type;
    // Set start value.
    l_gpio->ODR |= (0x0000FFFF & value) << (u8)pin;
    // Prepare pin structure object.
    s_gpio[(u8)port][(u8)pin].m_pInstance = l_gpio;
    s_gpio[(u8)port][(u8)pin].m_port = port;
    s_gpio[(u8)port][(u8)pin].m_pin = pin;
    s_gpio[(u8)port][(u8)pin].m_type = GPIO_OUTPUT;
    s_gpio[(u8)port][(u8)pin].m_pullType = GPI_ISNT_IN_USE;
    s_gpio[(u8)port][(u8)pin].m_outType = type;
    s_gpio[(u8)port][(u8)pin].m_state = value;
    
    return &s_gpio[(u8)port][(u8)pin];
}

GPIO_Error GPIO_GetInput(GPIO_Handler* pHandler, u8* pState)
{
    // Check if input handler and input state pointer are empty.
    if (!pHandler && !pState)
    {
        return GPIO_NULL_ERROR;
    }
    // Get input value.
    *pState = 0;
    if (pHandler->m_pInstance->IDR & (0x00000001 << pHandler->m_pin))
    {
        *pState = 1;
    }
    
    return GPIO_NOERROR;
}

GPIO_Error GPIO_SetOutput(GPIO_Handler* pHandler, GPIO_Signal output)
{
    // Check if output handler is empty.
    if (!pHandler)
    {
        return GPIO_NULL_ERROR;
    }
    // Check if handler is "output" typed.
    if (pHandler->m_type != GPIO_OUTPUT)
    {
        return GPIO_ISNT_OUTPUT;
    }
    // Set output signal level.
    if (output == GPIO_SET)
    {
        pHandler->m_pInstance->BSRR = 0x00000001 << pHandler->m_pin;    
    }
    else
    {
        pHandler->m_pInstance->BSRR = 0x00000001 << (pHandler->m_pin + 16);  
    }
    
    return GPIO_NOERROR;
}

GPIO_Error GPIO_Deinit(GPIO_Handler* pHandler)
{
    // Check if input handler is empty.
    if (!pHandler)
    {
        return GPIO_NULL_ERROR;
    }
    // 

    return GPIO_NOERROR;
}
