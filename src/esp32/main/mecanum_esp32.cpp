#include "mecanum_esp32.hpp"
#include "config.hpp"
#include "common.hpp"
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

/**
 * @brief メインループ処理
 * 
 */
void MecanumESP32::run_loop(){
    _odom->update();
}

/**
 * @brief 内部状態をデバッグプリント
 * 
 */
void MecanumESP32::_status_print(){
    if((common::millis() - _last_staus_print) >= config::STATUS_PRINT_MS){
        double* dt = _odom->get_pcnt_dt(0);
        double* rad1 = _odom->get_motor_velocity_rad(0);
        double* rad2 = _odom->get_motor_velocity_rad(1);
        double* rad3 = _odom->get_motor_velocity_rad(2);
        double* rad4 = _odom->get_motor_velocity_rad(3);
        _last_staus_print = common::millis();
        ESP_LOGI(common::TAG_MAIN, "rad1 : %lf, rad2 : %lf, rad3 : %lf, rad4 : %lf", *rad1, *rad2, *rad3, *rad4);
    }
}

void MecanumESP32::run(){
    int64_t msec = 0;
    while(1){
        msec = common::usecs();
        run_loop();
        _status_print();
        // ESP_LOGI(common::TAG_MAIN, "span:%lf", double(common::usecs() - msec)/1000000);
        vTaskDelay(25);
    }
}
extern "C" void app_main(void)
{
    MecanumESP32* esp32 = new MecanumESP32();
    esp32->run();
    delete esp32;
}

