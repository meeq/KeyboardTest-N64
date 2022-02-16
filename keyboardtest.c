/**
 * @file keyboardtest.c
 * @author Christopher Bonhage (me@christopherbonhage.com)
 * @brief N64 test ROM for RandNET Keyboard peripheral
 */

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <libdragon.h>

#define KEYBOARD_IDENTIFIER 0x0002

const char * format_controller_identifier( uint16_t identifier )
{
    switch( identifier )
    {
        case 0x0000: return "Unknown     ";
        case 0x0500: return "Controller  ";
        case 0x0001: return "VRU         ";
        case 0x0200: return "Mouse       ";
        case 0x0002: return "Keyboard    ";
        case 0xFFFF: return "Disconnected";
        default:     return "Unsupported ";
    }
}

uint64_t joybus_controller_identifiers( void )
{
    const uint64_t input[JOYBUS_BLOCK_DWORDS] =
    {
        0xff010300ffffffff,
        0xff010300ffffffff,
        0xff010300ffffffff,
        0xff010300ffffffff,
        0xfe00000000000000,
        0,
        0,
        1
    };
    uint64_t output[JOYBUS_BLOCK_DWORDS];

    joybus_exec( input, output );

    uint32_t * out_words = (uint32_t *)output;

    uint16_t port1 = out_words[1] >> 16;
    uint16_t port2 = out_words[3] >> 16;
    uint16_t port3 = out_words[5] >> 16;
    uint16_t port4 = out_words[7] >> 16;

    return (
        ((uint64_t)port1 << 48) |
        ((uint64_t)port2 << 32) |
        ((uint64_t)port3 << 16) |
        ((uint64_t)port4 << 0)
    );
}

void dump_keyboard_presses( uint8_t port )
{
    assert( port < 4 );

    /* Turn on the Power LED for all connected keyboards */
    static uint8_t led_status[4] = { 0x04, 0x04, 0x04, 0x04 };

    const uint64_t input[JOYBUS_BLOCK_DWORDS] = { 0, 0, 0, 0, 0, 0, 0, 1 };
    uint64_t output[JOYBUS_BLOCK_DWORDS];

    uint8_t * input_bytes = (uint8_t *)input;

    input_bytes[port + 0] = 0x02; // Send bytes
    input_bytes[port + 1] = 0x07; // Recv bytes
    input_bytes[port + 2] = 0x13; // Command
    input_bytes[port + 3] = led_status[port];

    joybus_exec( input, output );

    uint8_t * recv_bytes = ((uint8_t *)output) + port + 4;

    uint16_t key1 = ((uint16_t)recv_bytes[0] << 8) | recv_bytes[1];
    uint16_t key2 = ((uint16_t)recv_bytes[2] << 8) | recv_bytes[3];
    uint16_t key3 = ((uint16_t)recv_bytes[4] << 8) | recv_bytes[5];
    uint8_t out_status = recv_bytes[6];
    
    printf(
        "Port %d        %02x    %04x   %04x   %04x     %02x\n",
        port + 1, led_status[port], key1, key2, key3, out_status
    );

    /* Turn on Caps Lock LED when Caps Lock key is pressed */
    if( key1 == 0x0F05 || key2 == 0x0F05 || key3 == 0x0F05 )
    {
        led_status[port] |= 0x02;
    }
    else
    {
        led_status[port] &= ~0x02;
    }
    /* Turn on Num Lock LED when Num Lock key is pressed */
    if( key1 == 0x0A05 || key2 == 0x0A05 || key3 == 0x0A05 )
    {
        led_status[port] |= 0x01;
    }
    else
    {
        led_status[port] &= ~0x01;
    }
}

int main(void)
{
    /* Initialize peripherals */
    display_init(RESOLUTION_320x240, DEPTH_32_BPP, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    console_init();
    controller_init();

    console_set_render_mode(RENDER_MANUAL);

    uint64_t controller_identifiers = joybus_controller_identifiers();
    uint16_t port1 = controller_identifiers >> 48;
    uint16_t port2 = controller_identifiers >> 32;
    uint16_t port3 = controller_identifiers >> 16;
    uint16_t port4 = controller_identifiers >> 0;

    /* Main loop test */
    while(1) 
    {
        console_clear();

        printf("RandNET Keyboard Test ROM\n\n");

        printf("Controller Port 1: %s (0x%04X)\n", format_controller_identifier(port1), port1);
        printf("Controller Port 2: %s (0x%04X)\n", format_controller_identifier(port2), port2);
        printf("Controller Port 3: %s (0x%04X)\n", format_controller_identifier(port3), port3);
        printf("Controller Port 4: %s (0x%04X)\n", format_controller_identifier(port4), port4);

        printf("\n");
        printf("Keyboard:    LEDs   Key1   Key2   Key3   Status\n");
        printf("---------- | ---- | ---- | ---- | ---- | ------\n");

        if( port1 == KEYBOARD_IDENTIFIER ) dump_keyboard_presses( 0 );
        if( port2 == KEYBOARD_IDENTIFIER ) dump_keyboard_presses( 1 );
        if( port3 == KEYBOARD_IDENTIFIER ) dump_keyboard_presses( 2 );
        if( port4 == KEYBOARD_IDENTIFIER ) dump_keyboard_presses( 3 );

        console_render();
    }
}
