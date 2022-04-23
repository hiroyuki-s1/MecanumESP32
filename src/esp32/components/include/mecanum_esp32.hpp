#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <esp_pthread.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include "odometry.hpp"

class MecanumESP32 {
    public:
        MecanumESP32();
        void run_loop();
        void run();
        
    private:
        odom::Odometry* _odom;
        uint32_t _last_loop_time;
};