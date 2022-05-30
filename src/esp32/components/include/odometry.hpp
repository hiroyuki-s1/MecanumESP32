#ifndef __ODOMETORY__
#define __ODOMETORY__
#include <vector>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"
#include "pcnt_counter.hpp"

namespace odom{
    class Odometry {
        public:
            Odometry();
            void update();
            void set_config(double rotate_pulse_count);
            double* get_pcnt_dt(int index){
                return &(_pcnt_counter[index]->dt);
            }

            double* get_motor_velocity_rad(int i){
                return &_motor_verocity_rad[i];
            }

        private:
            // パルスカウント
            std::vector<pcnt_counter::PcntCounter*> _pcnt_counter;

            // オドメトリ計算用ホイールパラメータ
            double _wheel_width, _wheel_height; // 機体の幅、高さ
            double _roller_rad; // ローラ角度
            
            // オドメトリ変数
            std::vector<double> _v_vec;
            double _dt; //　更新周期
            std::vector<double> _motor_verocity_rad; // 角モータの角速度
            double _rotate_puluse_count; // １回転したときのパルス数           

            void _calc_motor_vecoity(); 
    };
    
}
#endif