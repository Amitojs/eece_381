#ifndef PS2_H_
#define PS2_H_

#include <stdio.h>
#include <string.h>
#include "main.h"

#define PS2_0_NAME "/dev/ps2_0"

alt_up_ps2_dev* ps2;
char* p;

void clear_ps2_fifo() {
	alt_up_ps2_clear_fifo(ps2);
}

void initialize_ps2 () {
	// setting up ps2
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

dir getDirectionFromKeyboard(dir lastdir, dir secondlastdir) {

	int PS2_data, RAVAIL;
	volatile int * PS2_ptr = (int *) ps2->base;

	alt_u8 byte3 = 0;

	PS2_data = IORD_16DIRECT(PS2_ptr, 2); // extract the RAVAIL field
	RAVAIL = PS2_data;
		if (RAVAIL > 0) {
			byte3 = IORD_8DIRECT(PS2_ptr, 0);

 			if (  (byte3 == 0x75) && (lastdir == nodir) && (secondlastdir == changedir)  ) {
 				return up;
			} else if (  (byte3 == 0x6B) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return left;
			} else if (  (byte3 == 0x72) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return down;
			} else if (  (byte3 == 0x74) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return right;
			} else if (  (byte3 == 0x6C) && (lastdir == nodir) && (secondlastdir == changedir) ) {
				return dpause;
			} else if ( byte3 == 0xE0) {
				return changedir;
			} else if ( byte3 == 0xF0)
				return dontchangedir;
			else {
				return nodir;
			}
		}
		return nodir;
}
#endif
