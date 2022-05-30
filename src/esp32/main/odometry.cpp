#include "odometry.hpp"
#include "common.hpp"
#include "config.hpp"

#define FRONT_R 0
#define FRONT_L 1
#define REAR_R  2
#define REAR_L 3
#define MOTOR_SIZE 4



            // PcntCounter(int pcnt_unit, int layer_a, int layer_b, 
            // int16_t h_limit, int16_t l_limit, int channel,
            // LIMIT_BURST_L callback_limit_burst_l,
            // LIMIT_BURST_H callback_limit_burst_h);
namespace odom{
    Odometry::Odometry(){
        _pcnt_counter = std::vector<pcnt_counter::PcntCounter*>(MOTOR_SIZE);
        
        _pcnt_counter[FRONT_R] = new pcnt_counter::PcntCounter(config::FRONT_RIGHT_ENC_PCNT_UNIT,
            config::FRONT_RIGHT_ENC_LAYER_A, config::FRONT_RIGHT_ENC_LAYER_B, config::PULSE_COUNT_LIMIT_MAX, config::PULSE_COUNT_LIMIT_MIN,
            config::FRONT_RIGHT_ENC_CHANNEL, FRONT_R);
        _pcnt_counter[FRONT_L] = new pcnt_counter::PcntCounter(config::FRONT_LEFT_ENC_PCNT_UNIT,
            config::FRONT_LEFT_ENC_LAYER_A, config::FRONT_LEFT_ENC_LAYER_B, config::PULSE_COUNT_LIMIT_MAX, config::PULSE_COUNT_LIMIT_MIN,
            config::FRONT_LEFT_ENC_CHANNEL, FRONT_L);
        _pcnt_counter[REAR_R] = new pcnt_counter::PcntCounter(config::REAR_RIGHT_ENC_PCNT_UNIT,
            config::REAR_RIGHT_ENC_LAYER_A, config::REAR_RIGHT_ENC_LAYER_B, config::PULSE_COUNT_LIMIT_MAX, config::PULSE_COUNT_LIMIT_MIN,
            config::REAR_RIGHT_ENC_CHANNEL, REAR_R);
        _pcnt_counter[REAR_L] = new pcnt_counter::PcntCounter(config::REAR_LEFT_ENC_PCNT_UNIT,
            config::REAR_LEFT_ENC_LAYER_A, config::REAR_LEFT_ENC_LAYER_B, config::PULSE_COUNT_LIMIT_MAX, config::PULSE_COUNT_LIMIT_MIN,
            config::REAR_LEFT_ENC_CHANNEL, REAR_L);     
        // resume(true)は一発目だけ       
        _pcnt_counter[FRONT_R]->resume(true);
        _pcnt_counter[FRONT_L]->resume(false);
        _pcnt_counter[REAR_R]->resume(false);
        _pcnt_counter[REAR_L]->resume(false);
        _rotate_puluse_count= config::PULSE_ROTATION_COUNT_DEFAULT;
        
        _motor_verocity_rad = std::vector<double>(MOTOR_SIZE, 0.0);
        // _dt = config::DT_SEC;
        ESP_LOGI(common::TAG_MAIN, "odometry initiale complete");
    }

    void Odometry::update(){
        double m_count, prev_m_count;
        int i = 0;
        // return ;
        for (auto cnt : _pcnt_counter) {
            cnt->update();
            m_count = (cnt->pulse_count + config::PULSE_COUNT_LIMIT_MAX * cnt->count_around) ;
            prev_m_count = (cnt->prev_pulse_count + config::PULSE_COUNT_LIMIT_MAX * cnt->prev_count_around) ;
            _motor_verocity_rad[i]=(m_count - prev_m_count) / _rotate_puluse_count * M_PI * 2 / cnt->dt;
            // _motor_verocity_rad[i]=(m_count - prev_m_count) ;
            // if(i==0) ESP_LOGI(common::TAG_MAIN, "%d", (int)(m_count - prev_m_count));
            // ESP_LOGI(common::TAG_MAIN, "dt %lf, %lf, %lf, %d", _dt, m_rad, prev_m_rad,cnt->pulse_count);
            i++;
        }
        // ESP_LOGI(common::TAG_MAIN, "FRONT R %d, FRONT L %d, REAR R %d, REAR L %d",
        //     _pcnt_counter[FRONT_R]->pulse_count, _pcnt_counter[FRONT_L]->pulse_count, _pcnt_counter[REAR_R]->pulse_count, _pcnt_counter[REAR_L]->pulse_count );

        // ESP_LOGI(common::TAG_MAIN, "FRONT R % lf, FRONT L %lf, REAR R %lf, REAR L %lf, dt:%lf , count:%d",
        //     _motor_verocity_rad[FRONT_R], _motor_verocity_rad[FRONT_L], _motor_verocity_rad[REAR_R], _motor_verocity_rad[REAR_L], _pcnt_counter[FRONT_R]->dt, _pcnt_counter[FRONT_R]->pulse_count);
    }

    /**
     * @brief 直前のパルスカウントと今のパルスカウントから、モーターの回転速度を計算
     * 
     */
    void Odometry::_calc_motor_vecoity(){
    }
}

