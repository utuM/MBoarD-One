/**
  * @file    Gpio.cpp
  * @version 1.0.0
  * @author  Kostyantyn Komarov (utuM)
  * @data    19.03.2019 (creation)
  * @data    23.03.2019 (release)
  * @brief   GPIO driver class implementation.
  *          Current driver can initializes every input and output pins are
  *          required. Every pin is represented as a 'InputPin' or 'OutputPin'
  *          class object. Every time on pin initialization, binded port will
  *          be enabling using RCC staff.
  *          For input pins, there is only 'getPinState()'. For output pins,
  *          there are 'setPinState()' and 'togglePin()'.
  *          On object desctructure, current pin will be de-initialized.
  **/

#include "Gpioj.h"

/**
  * Enable port by input port index.
  * @param[i] port - index of port to be activated.
  * @return None.
  **/
void Driver::Gpio::enablePort(Gpio::GpioPort port)
{
    // Check which port need to be activated.
    switch (port) {
        case Gpio::kGpioPortA : {
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortB : {
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortC : {
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortD : {
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortE : {
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortF : {
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortG : {
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortH : {
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
        }
        
        default : { 
            break;
        }
    }
}

/**
  * Disable port by input port index.
  * @param[i] port - index of port to be activated.
  * @return None.
  **/
void Driver::Gpio::disablePort(Gpio::GpioPort port)
{
    switch (port) {
        case Gpio::kGpioPortA : {
            __HAL_RCC_GPIOA_CLK_DISABLE();
            break;
        }
        
        case Gpio::kGpioPortB : {
            __HAL_RCC_GPIOB_CLK_DISABLE();
            break;
        }
        
        case Gpio::kGpioPortC : {
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortD : {
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortE : {
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortF : {
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortG : {
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
        }
        
        case Gpio::kGpioPortH : {
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
        }
        
        default : { 
            break;
        }
    }
}
    
/**
  * Input pin constructor with input parameters.
  * @param[i] name  - pin description (can be empty).
  * @param[i] port  - hardware port index that pin is occupied.
  * @param[i] pin   - hardware pin index that pin is occupied.
  * @param[i] speed - pin switching speed.
  * @param[i] pull  - no-pull/pull activation. 
  * @return None.
  **/
Driver::InputPin::InputPin(std::string name,
                           Gpio::GpioPort port,
                           Gpio::GpioPin pin,
                           Gpio::GpioSpeed speed,
                           Pull pull) : Gpio(name)
{
    // Enable port generating.
    enablePort(port);
    // Initialize input pin.
    GPIO_InitTypeDef l_gpiSettings = {0};
    l_gpiSettings.Pin = pin;
    l_gpiSettings.Speed = speed;
    l_gpiSettings.Pull = pull;
    HAL_GPIO_Init((GPIO_TypeDef *)port, &l_gpiSettings);
    // Save input port and pin.
    setPort(port);
    setPin(pin);
}

/**
  * Input pin destructor.
  * @return None.
  **/
Driver::InputPin::~InputPin(void)
{
    HAL_GPIO_DeInit(getPort(), getPin());
}

/**
  * Get input pin state.
  * @return true  - pin is 'High' level;
  *         false - pin is 'Low' level.
  **/
bool Driver::InputPin::getPinState(void)
{
    return HAL_GPIO_ReadPin(getPort(), getPin());
}

/**
  * Output pin constructor with input parameters.
  * @param[i] name  - pin description (can be empty).
  * @param[i] port  - hardware port index that pin is occupied.
  * @param[i] pin   - hardware pin index that pin is occupied.
  * @param[i] speed - pin switching speed.
  * @param[i] state - default pin level that need to be generated. 
  * @return None.
  **/
Driver::OutputPin::OutputPin(std::string name,
                             Gpio::GpioPort port,
                             Gpio::GpioPin pin,
                             Gpio::GpioSpeed speed,
                             State state) : Gpio(name)
{
    // Enable port generating.
    enablePort(port);
    // Save input port and pin.
    setPort(port);
    setPin(pin);
    // Initialize output pin.
    setPinState(kStateReset);
    GPIO_InitTypeDef l_gpiSettings = {0};
    l_gpiSettings.Pin = pin;
    l_gpiSettings.Speed = speed;
    l_gpiSettings.Pull = Driver::InputPin::kInputNoPull;
    l_gpiSettings.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init((GPIO_TypeDef *)port, &l_gpiSettings);
}

/**
  * Output pin destructor.
  * @return None.
  **/
Driver::OutputPin::~OutputPin(void)
{
    HAL_GPIO_DeInit(getPort(), getPin());
}

/**
  * Output pin level changing.
  * @param[i] state - new level to be generated on current pin.
  * @return None.
  **/
void Driver::OutputPin::setPinState(Driver::OutputPin::State state)
{
    HAL_GPIO_WritePin(getPort(), getPin(), (GPIO_PinState)state);
}

/**
  * Output pin level toggling.
  * @return None.
  **/
void Driver::OutputPin::togglePin(void)
{
    HAL_GPIO_TogglePin(getPort(), getPin());
}