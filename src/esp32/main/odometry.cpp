#include "odometry.hpp"
#include "common.hpp"
#include "config.hpp"

#define PCNT_H_LIM_VAL      10
#define PCNT_L_LIM_VAL     -10
#define PCNT_THRESH1_VAL    5
#define PCNT_THRESH0_VAL   -5
#define PCNT_INPUT_SIG_IO   36  // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO  5  // Control GPIO HIGH=count up, LOW=count down
#define LEDC_OUTPUT_IO      18 // Output GPIO of a sample 1 Hz pulse generator

            // PcntCounter(int pcnt_unit, int layer_a, int layer_b, 
            // int16_t h_limit, int16_t l_limit, int channel,
            // LIMIT_BURST_L callback_limit_burst_l,
            // LIMIT_BURST_H callback_limit_burst_h);
namespace odom{
    Odometry::Odometry(){
        _front_r_pcnt = new pcnt_counter::PcntCounter(config::FRONT_RIGHT_ENC_PCNT_UNIT,
            config::FRONT_RIGHT_ENC_LAYER_A, config::FRONT_RIGHT_ENC_LAYER_B, config::PULSE_COUNT_LIMIT_MAX, config::PULSE_COUNT_LIMIT_MIN,
            config::FRONT_RIGHT_ENC_CHANNEL);
        ESP_LOGI(common::TAG_MAIN, "odometry initiale complete");
    }

    void Odometry::update(){
        _front_r_pcnt->update();
        ESP_LOGI(common::TAG_MAIN, "count :%d  around %lld ",_front_r_pcnt->pulse_count, _front_r_pcnt->count_around);
    }
}

