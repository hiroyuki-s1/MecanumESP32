#ifndef __PCNT_COUNTER__
#define __PCNT_COUNTER__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/pcnt.h"
#include "esp_attr.h"
#include "esp_log.h"

namespace pcnt_counter{

    typedef struct {
        int unit;  // the PCNT unit that originated an interrupt
        uint32_t status; // information on the event type that caused the interrupt
    } pcnt_evt_t;


    class PcntCounter {
        public:
            PcntCounter(int pcnt_unit, int layer_a, int layer_b, 
            int16_t h_limit, int16_t l_limit, int channel, int index);
            void update();
            int16_t pulse_count; // パルスカウント
            int16_t prev_pulse_count;
            int64_t count_around;
            int64_t prev_count_around;
            void resume(bool service);
        private:
            void _pcnt_init();
            static xQueueHandle _pcnt_evt_queue;
            static void IRAM_ATTR _pcnt_intr_handler(void *arg);
            int _pcnt_unit;
            pcnt_unit_t _pcnt_unit_type;
            int _layer_a, _layer_b; 
            int16_t _h_limit, _l_limit;
            int _pcnt_channel;
            pcnt_evt_t _evt;
            portBASE_TYPE _res;
            int _index;
    };
    
}
#endif