#include <application.h>

// LED instance
twr_led_t led;

void twr_music_set_radio(uint64_t *id, const char *topic, void *value, void *param);

// subscribe table, format: topic, expect payload type, callback, user param
static const twr_radio_sub_t subs[] = {
    {"mp3-module/-/cmd/play", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_PLAY},
    {"mp3-module/-/cmd/pause", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_PAUSE},
    {"mp3-module/-/cmd/stop", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_STOP},
    {"mp3-module/-/cmd/next", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_NEXT},
    {"mp3-module/-/cmd/prev", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_PREV},
    {"mp3-module/-/cmd/volume-up", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_VOLUME_UP},
    {"mp3-module/-/cmd/volume-down", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_VOLUME_DOWN},
    {"mp3-module/-/cmd/sleep", TWR_RADIO_SUB_PT_NULL, twr_music_set_radio, (void *) TWR_MUSIC_SLEEP},
    {"mp3-module/-/cmd/volume-set", TWR_RADIO_SUB_PT_INT, twr_music_set_radio, (void *) TWR_MUSIC_VOLUME_SET},
    {"mp3-module/-/cmd/song-index", TWR_RADIO_SUB_PT_INT, twr_music_set_radio, (void *) TWR_MUSIC_INDEX},
    {"mp3-module/-/cmd/folder", TWR_RADIO_SUB_PT_INT, twr_music_set_radio, (void *) TWR_MUSIC_FOLDER}
};

// This function initialize music module on UART0 interface
void twr_music_init()
{
    twr_uart_init(TWR_UART_UART0, TWR_UART_BAUDRATE_9600, TWR_UART_SETTING_8N1);
}

// This function sends command depending on @command. Some commands can recieve more data that is passed as hex number in @data. @data ex. 0x0000
void twr_music_set_radio(uint64_t *id, const char *topic, void *value, void *param)
{
    twr_led_pulse(&led, 1000);
    int command = (int *)param;
    int data = *(int *)value;

    commandBuffer[3] = controlCommands[command];
    if(command == TWR_MUSIC_FOLDER)
    {
        commandBuffer[6] = 0x02;
        commandBuffer[5] = (uint8_t)(data);
    }
    else
    {
        commandBuffer[5] = (uint8_t)(data >> 8);
        commandBuffer[6] = (uint8_t)(data);
    }

    twr_uart_write(TWR_UART_UART0, &commandBuffer, 8);
}

void application_init(void)
{
    // Initialize LED
    twr_led_init(&led, TWR_GPIO_LED, false, false);
    twr_led_set_mode(&led, TWR_LED_MODE_OFF);

    //twr_log_init(TWR_LOG_LEVEL_DUMP, TWR_LOG_TIMESTAMP_ABS);

    // Initialize radio
    twr_radio_init(TWR_RADIO_MODE_NODE_LISTENING);
    twr_radio_set_subs((twr_radio_sub_t *) subs, sizeof(subs)/sizeof(twr_radio_sub_t));

    // Initialize mp3 module
    twr_music_init();

    // Send pairing request
    twr_radio_pairing_request("mp3-player", VERSION);

    twr_led_pulse(&led, 2000);

}
