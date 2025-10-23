
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
extern float target_angle;
Motor motor(3591.0f/187.0f,10,0,0,22,0,6,1000,1000,5000,5000,0.1,0.02);

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
            float feedforward_intensity =motor.FeedforwardIntensityCalc();
            motor.SetPosition(target_angle,0.0f,feedforward_intensity);
            //motor.SetSpeed(target_angle,feedforward_intensity);
            //motor.SetIntensity(feedforward_intensity);
            motor.handle();
            motor.output();
        }
    }
}
