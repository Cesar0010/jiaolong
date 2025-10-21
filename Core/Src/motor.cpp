//
// Created by Sesar on 2025/10/20.
//
#include "motor.h"
float linearMapping(float input, float input_min, float input_max, float output_min, float output_max)
{
    return (input - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
}

void Motor::canRxMsgCallback(const uint8_t rx_data[8])
{
    last_ecd_angle_ = ecd_angle_;
    ecd_angle_ = (rx_data[0] << 8) | rx_data[1];
    ecd_angle_=linearMapping(ecd_angle_, 0.0, 8191.0, 0.0, 360.0);
    delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_;
    if (delta_ecd_angle_ > 180.0f) {
        delta_ecd_angle_ -= 360.0f;
    } else if (delta_ecd_angle_ < -180.0f) {
        delta_ecd_angle_ += 360.0f;
    }
    delta_angle_ = delta_ecd_angle_ /ratio_;
    angle_ += delta_angle_;
    int16_t speed_raw = (rx_data[2] << 8) | rx_data[3];
    rotate_speed_ = static_cast<float>(speed_raw);
    int16_t current_raw = (rx_data[4] << 8) | rx_data[5];
    current_raw = static_cast<float>(current_raw);
    current_ = linearMapping(current_raw, -16384.0, 16383.0, -20.0, 20.0);
    temp_ = rx_data[6];
}

void Motor::SetPosition(float target_position, float feedforward_speed, float feedforward_intensity)
{
    target_angle_ = target_position;
    feedforward_speed_ = feedforward_speed;
    feedforward_intensity_ = feedforward_intensity;
    control_method_ = POSITION_SPEED;
}

void Motor::SetSpeed(float target_speed, float feedforward_intensity)
{
    target_angle_ = target_speed;
    feedforward_intensity_ = feedforward_intensity;
    control_method_ = SPEED;
}

void Motor::SetIntensity(float intensity)
{
    feedforward_intensity_ = intensity;
    control_method_ = TORQUE;
}

void Motor::handle()
{
    switch (control_method_)
    {
        case TORQUE:
            {
                output_intensity_ = feedforward_intensity_;
                break;
            }
        case SPEED:
            {
                float intensity_from_speed = spid_.calc(target_speed_,fdb_speed_);
                output_intensity_ = intensity_from_speed + feedforward_intensity_;
                break;
            }
        case POSITION_SPEED:
            {
                float speed_from_position = spid_.calc(target_angle_,fdb_angle_);
                float total_speed = speed_from_position + feedforward_speed_;
                float intensity_from_speed = ppid_.calc(target_speed_,total_speed);
                output_intensity_ = intensity_from_speed + feedforward_intensity_;
            }
    }
}

