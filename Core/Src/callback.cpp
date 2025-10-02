
#include "math.h"
#include "tim.h"
#include "main.h"
#include "usart.h"

extern uint32_t ticks;
extern uint8_t rx_msg[4];
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
        if (rx_msg[0] == 'R')
        {
            HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_SET);


        }
        else if (rx_msg[0] == 'M')
        {
            HAL_GPIO_WritePin(LEDR_GPIO_Port, LEDR_Pin, GPIO_PIN_RESET);
        }
        HAL_UART_Receive_IT(&huart7, rx_msg, 1);
    }
}