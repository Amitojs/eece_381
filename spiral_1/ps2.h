#ifndef PS2_H_
#define PS2_H_

#include <stdio.h>
#include <string.h>
#include "main.h"

#define PS_0_NAME "/dev/ps2_0"

// Pointer to our ps2 device
alt_up_ps2_dev* ps2;


// Helper function to clear keyboard buffer
void clear_ps2_fifo() {
	alt_up_ps2_clear_fifo(ps2);
}

// PS2 Device initialization
void initialize_ps2 () {
	
	usleep(5000000);
	
	ps2 = alt_up_ps2_open_dev(PS2_0_NAME);
	if (ps2 == NULL) {
		alt_printf ("Error: could not open ps2 device\n");
		keyb_set = 0;
	} else
		alt_printf ("Opened ps2 device\n");

	ps2->timeout = 500000;

	alt_up_ps2_clear_fifo(ps2);

	alt_up_ps2_init(ps2);
	if (ps2->device_type == 0) {
		printf("Opened mouse\n");
		keyb_set = 0;
	} else if (ps2->device_type == 1) {
		printf("Opened keyboard\n");
		keyb_set = 1;
		set_keyboard_rate(ps2, 0xFF);
	} else {
		keyb_set = 0;
		printf("Unknown device\n");
	}
}

// This function seeks and returns the direction on the arrow keys (including the arrow keys on the numpad)
// that is being pressed on the keyboard, as well as the pause button (Home key)
dir getDirectionFromKeyboard(dir lastdir, dir secondlastdir) {

	int PS2_data, RAVAIL;
	volatile int * PS2_ptr = (int *) ps2->base;

	alt_u8 byte3 = 0;

	PS2_data = IORD_16DIRECT(PS2_ptr, 2); // extract the RAVAIL field
	RAVAIL = PS2_data;
		if (RAVAIL > 0) { // RAVAIL indicates how many code are there in the buffer
			byte3 = IORD_8DIRECT(PS2_ptr, 0);
			// When a key is pressed, the keyboard sends a Make Code (in Hex, one byte) of the key.
			// Ex: 'A' = 0x1C. However here we only check for the Arrow Keys and the Home button.
			// Other key pressed is ignored. Arrow keys will send a Long Make Code (2 bytes) instead
			// of a normal Make Code. Long Make Code will send 0xE0 preceding the key's Make Code 
			// (Ex: Up Arrow will send 0xE0 and 0x75 when pressed). Upon released, 0xF0 will be sent 
			// preceding the Make Code of the key to indicate that this time the key is released. 
			// For example, releasing Up Arrow key will send 0xE0, 0xF0, and 0x75. Few variables are
			// used here as toggles to indicate the state of each Make Code received.
 			if (  (byte3 == 0x75) && (lastdir == nodir) && (secondlastdir == changedir)  ) {
 				return up; // Up Arrow or KeyPad 8 is pressed
			} else if (  (byte3 == 0x6B) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return left; // Left Arrow or KeyPad 4 is pressed
			} else if (  (byte3 == 0x72) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return down; // Down Arrow or KeyPad 2 is pressed
			} else if (  (byte3 == 0x74) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return right; // Right Arrow or KeyPad 6 is pressed
			} else if (  (byte3 == 0x6C) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return dpause; // Home key is pressed
			} else if ( byte3 == 0xE0) {
				return changedir; // If we receive a Long Make Code indicator
			} else if ( byte3 == 0xF0)
				return dontchangedir; // We always ignore the byte following 0xF0
			else {
				return nodir;
			}
		}
		return nodir;
}
#endif
