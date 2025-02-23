#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include <deque>

#include "pid.h"
#include "wifi.h"
#include "mqtt.h"

#include "json.hpp"


#define MAX_DUTY 1023
#define SAMPLE_CNT 32
static const adc1_channel_t adc_channel = (adc1_channel_t)ADC_CHANNEL_4;
#define PHOTORESIST_IN 32
static const adc1_channel_t photoresist_in = (adc1_channel_t)ADC_CHANNEL_7;
#define LEDC_GPIO 27
static ledc_channel_config_t ledc_channel;

WIFI::Wifi wifi;
MQTT::mqtt mqtt;


int led_value = 0;

static void init_hw(void)
{
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_channel, ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(photoresist_in, ADC_ATTEN_DB_11);
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = LEDC_GPIO;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.hpoint = 0;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel_config(&ledc_channel);
}

extern "C" void app_main()
{
    wifi.connect_to_wifi();
    esp_mqtt_client_handle_t mqtt_handle = mqtt.mqtt_app_start();

    init_hw();
    std::deque<int> current_value_deque;
    int max_deque_size = 10;

    while (1)
    {

        char *ourTaskName = pcTaskGetName(NULL);
        uint32_t adc_val = 0;
        for (int i = 0; i < SAMPLE_CNT; ++i)
        {
            adc_val += adc1_get_raw(adc_channel);
        }
        adc_val /= SAMPLE_CNT;

        uint32_t photo_in = 0;
        for (int i = 0; i < SAMPLE_CNT; ++i)
        {
            photo_in += adc1_get_raw(photoresist_in);
        }
        photo_in /= SAMPLE_CNT;

        if(current_value_deque.size() <= max_deque_size){
            current_value_deque.push_back(photo_in);
        }
        else{
            current_value_deque.pop_front();
            current_value_deque.push_back(photo_in);
        }

        pid_algorithms pid_instance(5,0.5,0.5);
        led_value = pid_instance.calculate_next_value(photo_in,adc_val, current_value_deque, ourTaskName);

        nlohmann::json_abi_v3_11_3::json led_pid_data;
        led_pid_data["s"] = adc_val;
        led_pid_data["c"] = photo_in;
        std::string led_pid_data_str = led_pid_data.dump();
        const char* led_pid_data_char = led_pid_data_str.c_str();
        esp_mqtt_client_publish(mqtt_handle, "/pid/led", led_pid_data_char , 0, 1, 0);

        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, led_value);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}