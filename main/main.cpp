#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include "esp_log.h"

#include "pid.h"
#include "wifi.h"


#define MAX_DUTY 1023  // Assuming a 10-bit resolution (2^10 - 1)
#define SAMPLE_CNT 32
static const adc1_channel_t adc_channel = (adc1_channel_t)ADC_CHANNEL_4;
#define PHOTORESIST_IN 32
static const adc1_channel_t photoresist_in = (adc1_channel_t)ADC_CHANNEL_7;
#define LEDC_GPIO 27
static ledc_channel_config_t ledc_channel;

WIFI::Wifi wifi;


int led_value = 0;

static void init_hw(void)
{
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_channel, ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(photoresist_in, ADC_ATTEN_DB_11);
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_10_BIT,   // 10-bit resolution (1024 levels)
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
    wifi.wifi_setup();

    char *ourTaskName = pcTaskGetName(NULL);
    ESP_LOGI(ourTaskName, "Starting Up!\n");

    while(wifi.GetState() != WIFI::Wifi::state_e::CONNECTED){
        vTaskDelay(pdMS_TO_TICKS(1000));
        wifi.wifi_run();
    }

    init_hw();
    while (1)
    {
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

        pid_algortihms calc(5);
        led_value = calc.calculate_proportional_value(photo_in,adc_val);

        char *ourTaskName = pcTaskGetName(NULL);
        ESP_LOGI(ourTaskName, "Set value: %lu", (unsigned long)adc_val);
        ESP_LOGI(ourTaskName, "Photoresist Value: %lu", (unsigned long)photo_in);
        ESP_LOGI(ourTaskName, "Led Value: %lu", (unsigned long)led_value);
        

        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, led_value);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(pdMS_TO_TICKS(1000));  // Convert to ticks safely


    }
}