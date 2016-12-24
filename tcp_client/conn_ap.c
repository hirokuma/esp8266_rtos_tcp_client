/*
 * ESPRSSIF MIT License
 *
 * Copyright (c) 2015 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#include "esp_common.h"

#include "espconn.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "conn_ap.h"

static void wifi_handle_event_cb(System_Event_t *evt);


static void wifi_handle_event_cb(System_Event_t *evt)
{
    printf("event %x\n", evt->event_id);

    switch (evt->event_id) {
    case EVENT_STAMODE_CONNECTED:
        printf("connect to ssid %s, channel %d\n",
               evt->event_info.connected.ssid,
               evt->event_info.connected.channel);
        break;
    case EVENT_STAMODE_DISCONNECTED:
        printf("disconnect from ssid %s, reason %d\n",
               evt->event_info.disconnected.ssid,
               evt->event_info.disconnected.reason);
        break;
    case EVENT_STAMODE_AUTHMODE_CHANGE:
        printf("mode: %d -> %d\n",
               evt->event_info.auth_change.old_mode,
               evt->event_info.auth_change.new_mode);
        break;
    case EVENT_STAMODE_GOT_IP:
        printf("ip:" IPSTR ",mask:" IPSTR ",gw:" IPSTR,
               IP2STR(&evt->event_info.got_ip.ip),
               IP2STR(&evt->event_info.got_ip.mask),
               IP2STR(&evt->event_info.got_ip.gw));
        printf("\n");
        break;
    case EVENT_SOFTAPMODE_STACONNECTED:
        printf("station: " MACSTR "join, AID = %d\n",
               MAC2STR(evt->event_info.sta_connected.mac),
               evt->event_info.sta_connected.aid);
        break;
    case EVENT_SOFTAPMODE_STADISCONNECTED:
        printf("station: " MACSTR "leave, AID = %d\n",
               MAC2STR(evt->event_info.sta_disconnected.mac),
               evt->event_info.sta_disconnected.aid);
        break;
    default:
        break;
    }
}


static void wifi_connect_wait(void)
{
    uint8 con_status = wifi_station_get_connect_status();
    while(con_status != STATION_GOT_IP) {
        con_status = wifi_station_get_connect_status();
        printf("Connect ap %s\n", con_status == STATION_IDLE ? "IDLE" : con_status == STATION_CONNECTING ? \
               "Connecting..." : con_status == STATION_WRONG_PASSWORD ? \
               "Password" : con_status == STATION_NO_AP_FOUND ? \
               "ap_not_find" : con_status == STATION_CONNECT_FAIL ? "Connect fail" : "Get ip");
        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    printf("---- wifi connected ----\n");
}


void conn_ap_start(void)
{
    struct station_config config;

    wifi_set_opmode(STATION_MODE);

    memset(&config, 0, sizeof(config));
    sprintf(config.ssid, DEMO_AP_SSID);
    sprintf(config.password, DEMO_AP_PASSWORD);
    wifi_station_set_config(&config);

    wifi_set_event_handler_cb(wifi_handle_event_cb);
    wifi_station_connect();

    wifi_connect_wait();
}
