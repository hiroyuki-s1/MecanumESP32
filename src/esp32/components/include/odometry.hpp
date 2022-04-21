#ifndef __ODOMETORY__
#define __ODOMETORY__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"

namespace odom{

    typedef struct {
        int unit;  // the PCNT unit that originated an interrupt
        uint32_t status; // information on the event type that caused the interrupt
    } pcnt_evt_t;


    class Odometry {
        public:
            Odometry();
            void run_test();
            static xQueueHandle _pcnt_evt_queue;
            static void IRAM_ATTR _pcnt_intr_handler(void *arg);
        private:
            void _pcnt_init(int unit);
    };
    
}
#endif