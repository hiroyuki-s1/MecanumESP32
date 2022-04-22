#ifndef __CONFIG__
#define __CONFIG__
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/pcnt.h"
namespace config{
    #define TEST INT16_MAX
    // エンコーダー設定
    int16_t PULSE_COUNT_LIMIT_MAX =  INT16_MAX;
    int16_t PULSE_COUNT_LIMIT_MIN = -INT16_MAX;

    static const int FRONT_RIGHT_ENC_PCNT_UNIT = PCNT_UNIT_0;
    static const int FRONT_RIGHT_ENC_CHANNEL   = PCNT_CHANNEL_0;
    static const int FRONT_RIGHT_ENC_LAYER_A   = 36;
    static const int FRONT_RIGHT_ENC_LAYER_B   = 39;

    int FRONT_LEFT_ENC_PCNT_UNIT  = PCNT_UNIT_1;
    int FRONT_LEFT_ENC_CHANNEL    = PCNT_CHANNEL_0;
    int FRONT_LEFT_ENC_LAYER_A    = 34;
    int FRONT_LEFT_ENC_LAYER_B    = 35;

    // static const int REAR_RIGHT_ENC_PCNT_UNIT  = PCNT_UNIT_0;
    // static const int REAR_RIGHT_ENC_CHANNEL    = PCNT_CHANNEL_1;
    // static const int REAR_RIGHT_ENC_LAYER_A    = 32;
    // static const int REAR_RIGHT_ENC_LAYER_B    = 33;

    // static const int REAR_LEFT_ENC_PCNT_UNIT   = PCNT_UNIT_1;
    // static const int REAR_LEFT_ENC_CHANNEL     = PCNT_CHANNEL_1;
    // static const int REAR_LEFT_ENC_LAYER_A     = 25;
    // static const int REAR_LEFT_ENC_LAYER_B     = 26;
}


#endif