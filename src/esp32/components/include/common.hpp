#ifndef __COMMON__
#define __COMMON__

namespace common{
    const int VERSION_MAJOR = 0;
    const int VERSION_MINOR = 0;
    const int VERSION_PATCH = 0;
    static const char *TAG_MAIN = "mecanum_esp32";

    static uint32_t millis(){
        return (uint32_t)(esp_timer_get_time() / 1000ULL);
    }
}
#endif