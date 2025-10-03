#include <stdint.h>

class M3508_Motor
{
private:
    const float ratio_;
    float angle_ = 0.0f;
    float delta_angle_ = 0.0f;
    float ecd_angle_ = 0.0f;
    float last_ecd_angle_ = 0.0f;
    float delta_ecd_angle_ = 0.0f;
    float rotate_speed_ = 0.0f;
    float current_ = 0.0f;
    float temp_ = 0.0f;
public:
    explicit M3508_Motor(const float ratio) : ratio_(ratio) {};
    void canRxMsgCallback(const uint8_t rx_data[8]);
};
float linearMapping(float input, float input_min, float input_max, float output_min, float output_max)
{
    return (input - input_min) * (output_max - output_min) / (input_max - input_min) + output_min;
}
void M3508_Motor::canRxMsgCallback(const uint8_t rx_data[8])
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