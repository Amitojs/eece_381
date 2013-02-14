#ifndef INITILIZE_VGA_H_
#define INITILIZE_VGA_H_

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <string.h>
#include <altera_up_avalon_video_pixel_buffer_dma.h>
#include <altera_up_avalon_video_character_buffer_with_dma.h>


//------------------------------------------------------
// Local NIOS numbers:

//Change this in your local S ram address in QSYS
#define YOUR_SRAM_ADDR          0x80000

//Change this to your local SW address
#define YOUR_SWITCHES_ADDR        0x2000  //Scott's
//#define YOUR_SWITCHES_ADDR      0x4010  //Amitoj's

//Change to your QSYS local name
#define YOUR_PIXEL_BUFFER_NAME    "/dev/video_pixel_buffer_dma_0"  //Scott's
//#define YOUR_PIXEL_BUFFER_NAME  "/dev/pixel_buffer_dma"  //Amitoj's

//Change to your QSYS local name
#define YOUR_CHAR_BUFFER_NAME   "/dev/char_drawer"

//------------------------------------------------------
// Global Variables

alt_up_pixel_buffer_dma_dev* pixel_buffer;
alt_up_char_buffer_dev *char_buffer;

//------------------------------------------------------

void initilize_vga(){
    // Use the name of your pixel buffer DMA core
    pixel_buffer = alt_up_pixel_buffer_dma_open_dev(YOUR_PIXEL_BUFFER_NAME);

    unsigned int pixel_buffer_addr1 = YOUR_SRAM_ADDR;
    unsigned int pixel_buffer_addr2 = YOUR_SRAM_ADDR + (320 * 240 * 2);

    // Set the 1st buffer address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr1);

    // Swap buffers – we have to swap because there is only an API function
    // to set the address of the background buffer.
    alt_up_pixel_buffer_dma_swap_buffers(pixel_buffer);
    while (alt_up_pixel_buffer_dma_check_swap_buffers_status(pixel_buffer));

    // Set the 2nd buffer address
    alt_up_pixel_buffer_dma_change_back_buffer_address(pixel_buffer, pixel_buffer_addr2);

    // Clear both buffers (this makes all pixels black)
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 1);

    // Initialize
    char_buffer = alt_up_char_buffer_open_dev(YOUR_CHAR_BUFFER_NAME);
    alt_up_char_buffer_init(char_buffer);

    // Clear the screen
    alt_up_pixel_buffer_dma_clear_screen(pixel_buffer, 0);
    alt_up_char_buffer_clear(char_buffer);
}




#endif
