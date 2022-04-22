#ifndef __ODOMETORY__
#define __ODOMETORY__

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
        private:
            pcnt_counter::PcntCounter* _front_r_pcnt;
            pcnt_counter::PcntCounter* _front_l_pcnt;
            pcnt_counter::PcntCounter* _rear_r_pcnt;
            pcnt_counter::PcntCounter* _rear_l_pcnt;
    };
    
}
#endif