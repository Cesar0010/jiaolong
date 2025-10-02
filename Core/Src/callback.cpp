
#include "math.h"
#include "tim.h"
#include "main.h"
#include "usart.h"

extern uint32_t ticks;
extern uint8_t rx_msg[4];
extern uint8_t tx_msg[10];
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1)
    {
        //ticks++;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY_Pin)
    {
        //ticks++;
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart7)
    {

        HAL_UART_Transmit(&huart7,rx_msg,3,1000);

    }
}