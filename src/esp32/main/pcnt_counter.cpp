#include "pcnt_counter.hpp"
#include "common.hpp"

#define WAKE_TIME 0
#define BUFFER_SIZE 10

namespace pcnt_counter{
    xQueueHandle PcntCounter::_pcnt_evt_queue = xQueueCreate(BUFFER_SIZE, sizeof(pcnt_evt_t));
    void IRAM_ATTR PcntCounter::_pcnt_intr_handler(void *arg)
    {
        int pcnt_unit = (int)arg;
        pcnt_evt_t evt;
        evt.unit = pcnt_unit;
        pcnt_get_event_status(static_cast<pcnt_unit_t>(pcnt_unit), &evt.status);
        xQueueSendFromISR(_pcnt_evt_queue, &evt, NULL);
    }

    /**
     * @brief パルスカウンターからエンコーダー情報を管理する
     * 
     * @param pcnt_unit PCNT UNIT
     * @param layer_a  A層のGPIO
     * @param layer_b  B層のGPIO
     * @param h_limit　カウント値の最大
     * @param l_limit  カウント値の最小
     * @param channel  PCNTチャンネル
     * @param callback_limit_burst_l  カウント満了時のコールバック関数(最大)
     * @param callback_limit_burst_h  カウント満了時のコールバック関数(最小)
     */
    PcntCounter::PcntCounter(int pcnt_unit, int layer_a, int layer_b, 
            int16_t h_limit, int16_t l_limit, int channel, int index
            ) : _pcnt_unit(pcnt_unit), _layer_a(layer_a), _layer_b(layer_b),
            _h_limit(h_limit), _l_limit(l_limit), _pcnt_channel(channel), _index(index)
    {
        _pcnt_unit_type = static_cast<pcnt_unit_t>(_pcnt_unit);
        count_around = 0;
        _pcnt_init();
        ESP_LOGI(common::TAG_MAIN, "[PcntCounter Settings]layer a: %d, layer b: %d, ch: %d, unit: %d",_layer_a, _layer_b, _pcnt_channel, _pcnt_unit);
    }

    /**
     * @brief pcnt初期化処理
     * 
     */
    void PcntCounter::_pcnt_init()
    {
        pcnt_unit_t pcnt_unit = static_cast<pcnt_unit_t>(_pcnt_unit);
        pcnt_count_mode_t pos = static_cast<pcnt_count_mode_t>(PCNT_COUNT_INC);
        /* Prepare configuration for the PCNT unit */
        pcnt_config_t pcnt_config = {
            // Set PCNT input signal and control GPIOs
            .pulse_gpio_num = _layer_a,
            .ctrl_gpio_num = _layer_b,
            .lctrl_mode = static_cast<pcnt_ctrl_mode_t>(PCNT_MODE_REVERSE), // Reverse counting direction if low
            .hctrl_mode = static_cast<pcnt_ctrl_mode_t>(PCNT_MODE_KEEP),    // Keep the primary counter mode if high
            .pos_mode = pos,   // Count up on the positive edge
            .neg_mode = static_cast<pcnt_count_mode_t>(PCNT_COUNT_DIS),   // Keep the counter value on the negative edge
            .counter_h_lim = _h_limit,
            .counter_l_lim = _l_limit,
            .unit = pcnt_unit,
            .channel = static_cast<pcnt_channel_t>(_pcnt_channel),
        };
        /* Initialize PCNT unit */
        pcnt_unit_config(&pcnt_config);

        /* Configure and enable the input filter */
        // pcnt_set_filter_value(pcnt_unit, 100);
        // pcnt_filter_enable(pcnt_unit);

        pcnt_event_enable(pcnt_unit, PCNT_EVT_H_LIM);
        pcnt_event_enable(pcnt_unit, PCNT_EVT_L_LIM);

        /* Initialize PCNT's counter */
        pcnt_counter_pause(pcnt_unit);
        pcnt_counter_clear(pcnt_unit);

        /* Install interrupt service and add isr callback handler */
        // pcnt_isr_service_install(0);
        // pcnt_isr_handler_add(pcnt_unit, _pcnt_intr_handler,(void *)_pcnt_unit);

        /* Everything is set up, now go to counting */
        // pcnt_counter_resume(pcnt_unit);
    }

    void PcntCounter::resume(bool service){
        pcnt_unit_t pcnt_unit = static_cast<pcnt_unit_t>(_pcnt_unit);
        if(service){
            pcnt_isr_service_install(0);
        }
        pcnt_isr_handler_add(pcnt_unit, _pcnt_intr_handler,(void *)_pcnt_unit);
        pcnt_counter_resume(pcnt_unit);

    }

    /**
     * @brief パルスカウントを更新
     * 
     */
    void PcntCounter::update(){
        _res = xQueueReceive(_pcnt_evt_queue, &_evt, WAKE_TIME / portTICK_PERIOD_MS);
        prev_count_around = count_around;
        prev_pulse_count = pulse_count;
        pcnt_get_counter_value(_pcnt_unit_type, &pulse_count);
        if (_res == pdTRUE) {
            if (_evt.status & PCNT_EVT_L_LIM) {
                ESP_LOGI(common::TAG_MAIN, "L_LIM EVT");
                count_around--;
            }
            if (_evt.status & PCNT_EVT_H_LIM) {
                ESP_LOGI(common::TAG_MAIN, "H_LIM EVT");
                count_around++;
            }
        }
    }
}

