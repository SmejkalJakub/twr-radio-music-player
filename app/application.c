#include <application.h>

// LED instance
bc_led_t led;

void bc_music_set_radio(uint64_t *id, const char *topic, void *value, void *param);

// subscribe table, format: topic, expect payload type, callback, user param
static const bc_radio_sub_t subs[] = {
    {"mp3-module/-/cmd/play", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_PLAY},
    {"mp3-module/-/cmd/pause", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_PAUSE},
    {"mp3-module/-/cmd/stop", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_STOP},
    {"mp3-module/-/cmd/next", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_NEXT},
    {"mp3-module/-/cmd/prev", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_PREV},
    {"mp3-module/-/cmd/volume-up", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_VOLUME_UP},
    {"mp3-module/-/cmd/volume-down", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_VOLUME_DOWN},
    {"mp3-module/-/cmd/sleep", BC_RADIO_SUB_PT_NULL, bc_music_set_radio, (void *) BC_MUSIC_SLEEP},
    {"mp3-module/-/cmd/volume-set", BC_RADIO_SUB_PT_INT, bc_music_set_radio, (void *) BC_MUSIC_VOLUME_SET},
    {"mp3-module/-/cmd/song-index", BC_RADIO_SUB_PT_INT, bc_music_set_radio, (void *) BC_MUSIC_INDEX},
    {"mp3-module/-/cmd/folder", BC_RADIO_SUB_PT_INT, bc_music_set_radio, (void *) BC_MUSIC_FOLDER}
};

// This function initialize music module on UART0 interface
void bc_music_init()
{
    bc_uart_init(BC_UART_UART0, BC_UART_BAUDRATE_9600, BC_UART_SETTING_8N1);
}

// This function sends command depending on @command. Some commands can recieve more data that is passed as hex number in @data. @data ex. 0x0000 
void bc_music_set_radio(uint64_t *id, const char *topic, void *value, void *param)
{
    bc_led_pulse(&led, 1000);
    int command = (int *)param;
    int data = *(int *)value;

    commandBuffer[3] = controlCommands[command];
    if(command == BC_MUSIC_FOLDER)
    {
        commandBuffer[6] = 0x02;
        commandBuffer[5] = (uint8_t)(data);
    }
    else
    {
        commandBuffer[5] = (uint8_t)(data >> 8);
        commandBuffer[6] = (uint8_t)(data);
    }

    bc_uart_write(BC_UART_UART0, &commandBuffer, 8);
}

void application_init(void)
{
    // Initialize LED
    bc_led_init(&led, BC_GPIO_LED, false, false);
    bc_led_set_mode(&led, BC_LED_MODE_OFF);

    //bc_log_init(BC_LOG_LEVEL_DUMP, BC_LOG_TIMESTAMP_ABS);

    // Initialize radio
    bc_radio_init(BC_RADIO_MODE_NODE_LISTENING);
    bc_radio_set_subs((bc_radio_sub_t *) subs, sizeof(subs)/sizeof(bc_radio_sub_t));

    // Initialize mp3 module
    bc_music_init();

    // Send pairing request
    bc_radio_pairing_request("mp3-player", VERSION);

    bc_led_pulse(&led, 2000);

}
