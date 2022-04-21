#include "odometry.hpp"
#include "common.hpp"

#define PCNT_H_LIM_VAL      10
#define PCNT_L_LIM_VAL     -10
#define PCNT_THRESH1_VAL    5
#define PCNT_THRESH0_VAL   -5
#define PCNT_INPUT_SIG_IO   36  // Pulse Input GPIO
#define PCNT_INPUT_CTRL_IO  5  // Control GPIO HIGH=count up, LOW=count down
#define LEDC_OUTPUT_IO      18 // Output GPIO of a sample 1 Hz pulse generator

namespace odom{
    xQueueHandle Odometry::_pcnt_evt_queue = xQueueCreate(10, sizeof(pcnt_evt_t));
    void IRAM_ATTR Odometry::_pcnt_intr_handler(void *arg)
    {
        int pcnt_unit = (int)arg;
        pcnt_evt_t evt;
        evt.unit = pcnt_unit;
        pcnt_get_event_status(static_cast<pcnt_unit_t>(pcnt_unit), &evt.status);
        xQueueSendFromISR(_pcnt_evt_queue, &evt, NULL);
    }

    Odometry::Odometry(){
        
        ESP_LOGI(common::TAG_MAIN, "Odometry Initial Complete " );
        // pcnt_example_init(1);
    };

    void Odometry::_pcnt_init(int unit)
    {
        pcnt_unit_t pcnt_unit = static_cast<pcnt_unit_t>(unit);
        pcnt_count_mode_t pos = static_cast<pcnt_count_mode_t>(PCNT_COUNT_INC);
        /* Prepare configuration for the PCNT unit */
        pcnt_config_t pcnt_config = {
            // Set PCNT input signal and control GPIOs
            .pulse_gpio_num = PCNT_INPUT_SIG_IO,
            .ctrl_gpio_num = PCNT_INPUT_CTRL_IO,
            .lctrl_mode = static_cast<pcnt_ctrl_mode_t>(PCNT_MODE_REVERSE), // Reverse counting direction if low
            .hctrl_mode = static_cast<pcnt_ctrl_mode_t>(PCNT_MODE_KEEP),    // Keep the primary counter mode if high
            .pos_mode = pos,   // Count up on the positive edge
            .neg_mode = static_cast<pcnt_count_mode_t>(PCNT_COUNT_DIS),   // Keep the counter value on the negative edge
            // What to do on the positive / negative edge of pulse input?
            // What to do when control input is low or high?
            // Set the maximum and minimum limit values to watch
            .counter_h_lim = PCNT_H_LIM_VAL,
            .counter_l_lim = PCNT_L_LIM_VAL,
            .unit = pcnt_unit,
            .channel = static_cast<pcnt_channel_t>(PCNT_CHANNEL_0),
        };
        /* Initialize PCNT unit */
        pcnt_unit_config(&pcnt_config);

        /* Configure and enable the input filter */
        pcnt_set_filter_value(pcnt_unit, 100);
        pcnt_filter_enable(pcnt_unit);

        /* Set threshold 0 and 1 values and enable events to watch */
        pcnt_set_event_value(pcnt_unit, PCNT_EVT_THRES_1, PCNT_THRESH1_VAL);
        pcnt_event_enable(pcnt_unit, PCNT_EVT_THRES_1);
        pcnt_set_event_value(pcnt_unit, PCNT_EVT_THRES_0, PCNT_THRESH0_VAL);
        pcnt_event_enable(pcnt_unit, PCNT_EVT_THRES_0);
        /* Enable events on zero, maximum and minimum limit values */
        pcnt_event_enable(pcnt_unit, PCNT_EVT_ZERO);
        pcnt_event_enable(pcnt_unit, PCNT_EVT_H_LIM);
        pcnt_event_enable(pcnt_unit, PCNT_EVT_L_LIM);

        /* Initialize PCNT's counter */
        pcnt_counter_pause(pcnt_unit);
        pcnt_counter_clear(pcnt_unit);

        /* Install interrupt service and add isr callback handler */
        pcnt_isr_service_install(0);
        pcnt_isr_handler_add(pcnt_unit, _pcnt_intr_handler,(void *)unit);

        /* Everything is set up, now go to counting */
        pcnt_counter_resume(pcnt_unit);
    }

    void Odometry::run_test()
    {
        int pcnt_unit = PCNT_UNIT_0;
        pcnt_unit_t p_unit = static_cast<pcnt_unit_t>(pcnt_unit);

        /* Initialize LEDC to generate sample pulse signal */
        // ledc_init();

        /* Initialize PCNT event queue and PCNT functions */
        _pcnt_init(pcnt_unit);

        int16_t count = 0;
        pcnt_evt_t evt;
        portBASE_TYPE res;
        while (1) {
            /* Wait for the event information passed from PCNT's interrupt handler.
            * Once received, decode the event type and print it on the serial monitor.
            */
            res = xQueueReceive(_pcnt_evt_queue, &evt, 1000 / portTICK_PERIOD_MS);
            if (res == pdTRUE) {
                pcnt_get_counter_value(p_unit, &count);
                ESP_LOGI(common::TAG_MAIN, "Event PCNT unit[%d]; cnt: %d", evt.unit, count);
                if (evt.status & PCNT_EVT_THRES_1) {
                    ESP_LOGI(common::TAG_MAIN, "THRES1 EVT");
                }
                if (evt.status & PCNT_EVT_THRES_0) {
                    ESP_LOGI(common::TAG_MAIN, "THRES0 EVT");
                }
                if (evt.status & PCNT_EVT_L_LIM) {
                    ESP_LOGI(common::TAG_MAIN, "L_LIM EVT");
                }
                if (evt.status & PCNT_EVT_H_LIM) {
                    ESP_LOGI(common::TAG_MAIN, "H_LIM EVT");
                }
                if (evt.status & PCNT_EVT_ZERO) {
                    ESP_LOGI(common::TAG_MAIN, "ZERO EVT");
                }
            } else {
                // pcnt_get_counter_value(p_unit, &count);
                // ESP_LOGI(common::TAG_MAIN, "Current counter value :%d", count);
            }
        }
    }
}

