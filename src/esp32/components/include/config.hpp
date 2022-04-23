#ifndef __CONFIG__
#define __CONFIG__
#include <cmath>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/pcnt.h"
namespace config{
    #define TEST INT16_MAX
    // エンコーダー設定
    static int16_t PULSE_COUNT_LIMIT_MAX =  INT16_MAX;
    static int16_t PULSE_COUNT_LIMIT_MIN = -INT16_MAX;

    static const int FRONT_RIGHT_ENC_PCNT_UNIT = PCNT_UNIT_0;
    static const int FRONT_RIGHT_ENC_CHANNEL   = PCNT_CHANNEL_0;
    static const int FRONT_RIGHT_ENC_LAYER_A   = 36;
    static const int FRONT_RIGHT_ENC_LAYER_B   = 39;

    static const int FRONT_LEFT_ENC_PCNT_UNIT  = PCNT_UNIT_1;
    static const int FRONT_LEFT_ENC_CHANNEL    = PCNT_CHANNEL_0;
    static const int FRONT_LEFT_ENC_LAYER_A    = 34;
    static const int FRONT_LEFT_ENC_LAYER_B    = 35;

    static const int REAR_RIGHT_ENC_PCNT_UNIT  = PCNT_UNIT_2;
    static const int REAR_RIGHT_ENC_CHANNEL    = PCNT_CHANNEL_0;
    static const int REAR_RIGHT_ENC_LAYER_A    = 32;
    static const int REAR_RIGHT_ENC_LAYER_B    = 33;

    static const int REAR_LEFT_ENC_PCNT_UNIT   = PCNT_UNIT_3;
    static const int REAR_LEFT_ENC_CHANNEL     = PCNT_CHANNEL_0;
    static const int REAR_LEFT_ENC_LAYER_A     = 25;
    static const int REAR_LEFT_ENC_LAYER_B     = 26;

    // オドメトリ計算デフォルトパラメータ
    static const int DT_MS = 16;
    static const double DT_SEC =  (double)DT_MS / 1000;
    static const int PULSE_ROTATION_COUNT_DEFAULT = 700; 
    static const double ROLLER_ANGLE_DEFAULT = M_PI/4; 
    static const double WHEEL_BASE_DEFAULT = 0.4;

}

#endif