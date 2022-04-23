#include "mecanum_esp32.hpp"
#include "common.hpp"
#include "config.hpp"
#include "FreeRTOSConfig.h"

MecanumESP32::MecanumESP32(){
    // 各モジュール初期化
    _odom = new odom::Odometry();

    ESP_LOGI(common::TAG_MAIN, "########################################");
    ESP_LOGI(common::TAG_MAIN, "## MECANUM ESP32 MOTOR DRIVER " );
    ESP_LOGI(common::TAG_MAIN, "## git page: https://github.com/hiroyuki-s1/MecanumESP32" );
    ESP_LOGI(common::TAG_MAIN, "## Ver: %d.%d.%d",common::VERSION_MAJOR, common::VERSION_MINOR, common::VERSION_PATCH );
    ESP_LOGI(common::TAG_MAIN, "########################################");
}

void MecanumESP32::run_loop(){
    _odom->update();
}

void MecanumESP32::run(){
    uint32_t msec ;
    while(1){
        msec = common::millis();
        if(msec - _last_loop_time >= config::DT_MS){
            ESP_LOGI(common::TAG_MAIN, "ms %d", (int)(msec - _last_loop_time));
            _last_loop_time = common::millis();
            run_loop();
        }
        vTaskDelay(1/portTICK_PERIOD_MS);
    }
}
extern "C" void app_main(void)
{
    MecanumESP32* esp32 = new MecanumESP32();
    esp32->run();
    delete esp32;
}

