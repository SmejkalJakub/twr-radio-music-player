#ifndef _APPLICATION_H
#define _APPLICATION_H

#ifndef VERSION
#define VERSION "vdev"
#endif


typedef enum
{
    TWR_MUSIC_NEXT = 0,
    TWR_MUSIC_PREV = 1,
    TWR_MUSIC_INDEX = 2,
    TWR_MUSIC_VOLUME_UP = 3,
    TWR_MUSIC_VOLUME_DOWN = 4,
    TWR_MUSIC_VOLUME_SET = 5,
    TWR_MUSIC_CYCLE = 6,
    TWR_MUSIC_SLEEP = 7,
    TWR_MUSIC_WAKE = 8,
    TWR_MUSIC_PLAY = 9,
    TWR_MUSIC_PAUSE = 10,
    TWR_MUSIC_STOP = 11,
    TWR_MUSIC_FOLDER = 12

}twr_music_commands_t;


#include <bcl.h>
#include <twr.h>


/*
* Packet for controling UART mp3 player(commandBuffer[3] = controlCommand[x];)
*/
uint8_t commandBuffer[8] = {0x7e, 0xff, 0x06, 0x0d, 0x00, 0x00, 0x00, 0xef};

uint8_t controlCommands[13] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x19, 0x0A, 0x0B, 0x0D, 0x0E, 0x16, 0x17};

void twr_music_set(twr_music_commands_t command, uint16_t dat);

void twr_music_init();

#endif
