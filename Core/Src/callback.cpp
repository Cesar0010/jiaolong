
#include <cmath>

#include "can.h"
#include "motor.h"
#include "tim.h"

extern CAN_RxHeaderTypeDef rx_header;
extern CAN_TxHeaderTypeDef tx_header;
extern uint8_t tx_data[8];
extern uint8_t rx_data[8];
extern uint32_t can_tx_mail_box_;
extern uint8_t stop_flag;
Motor motor(3591.0f/187.0f,0,0,0,0,0,0,0,0,0,0,0,0);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == htim6.Instance)
    {
        if (!stop_flag)
        {
            HAL_CAN_AddTxMessage(&hcan1, &tx_header, tx_data, &can_tx_mail_box_);
        }
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan -> Instance == CAN1)
    {
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
        if (rx_header.StdId == 0x201)
        {
            motor.canRxMsgCallback(rx_data);
            motor.FeedforwardIntensityCalc();
            motor.handle();
            motor.output();
        }
    }
}
