/*
 * ps2.c
 *
   add this to the main.c global variable

 	// For keyboard
	extern alt_up_ps2_dev* ps2;
	extern char* p;
	//--------------------------------

	and edit typedef enum dir in main.h to these

	//Directions
	typedef enum { up, down, left, right, nodir, dpause, changedir, dontchangedir } dir;
	//------------------------------------------------------
  
  the starting of getDir() function should be like this

	dir getdir(){
	if ( (ps2->device_type == 1) || (ps2->device_type == PS2_KEYBOARD) ) { // get direction from keyboard
		dir dirFromKeyB;
		dirFromKeyB = getDirectionFromKeyboard(lastdir, secondlastdir);
		if ( (dirFromKeyB == dpause) || (dirFromKeyB == up) || (dirFromKeyB == down) || (dirFromKeyB == right) || (dirFromKeyB == left) ) {
			lastdir = dirFromKeyB;
			return dirFromKeyB;
		} else if ( (dirFromKeyB == changedir) || (dirFromKeyB == dontchangedir) ){
			secondlastdir = dirFromKeyB;
			lastdir = nodir;
			return nodir;
		} else
			lastdir = nodir;
			return nodir;
	} else
	if (swfinder() == 0){

 *
 *
 */

#include <stdio.h>
#include <string.h>
#include "ps2.h"

/* don't need this for now
char *A_key_table[SCAN_CODE_NUM] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "`", "-", "=", "\\", "BKSP", "SPACE", "TAB", "CAPS", "L SHFT", "L CTRL", "L GUI", "L ALT", "R SHFT", "R CTRL", "R GUI", "R ALT", "APPS", "ENTER", "ESC", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "SCROLL", "[", "INSERT", "HOME", "PG UP", "DELETE", "END", "PG DN", "U ARROW", "L ARROW", "D ARROW", "R ARROW", "NUM", "KP /", "KP *", "KP -", "KP +", "KP ENTER", "KP .", "KP 0", "KP 1", "KP 2", "KP 3", "KP 4", "KP 5", "KP 6", "KP 7", "KP 8", "KP 9", "]", ";", "'", ",", ".", "/" };

alt_u8 A_single_byte_make_code_table[SCAN_CODE_NUM] = { 0x1C, 0x32, 0x21, 0x23, 0x24, 0x2B, 0x34, 0x33, 0x43, 0x3B, 0x42, 0x4B, 0x3A, 0x31, 0x44, 0x4D, 0x15, 0x2D, 0x1B, 0x2C, 0x3C, 0x2A, 0x1D, 0x22, 0x35, 0x1A, 0x45, 0x16, 0x1E, 0x26, 0x25, 0x2E, 0x36, 0x3D, 0x3E, 0x46, 0x0E, 0x4E, 0x55, 0x5D, 0x66, 0x29, 0x0D, 0x58, 0x12, 0x14, 0, 0x11, 0x59, 0, 0, 0, 0, 0x5A, 0x76, 0x05, 0x06, 0x04, 0x0C, 0x03, 0x0B, 0x83, 0x0A, 0x01, 0x09, 0x78, 0x07, 0x7E, 0x54, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x77, 0, 0x7C, 0x7B, 0x79, 0, 0x71, 0x70, 0x69, 0x72, 0x7A, 0x6B, 0x73, 0x74, 0x6C, 0x75, 0x7D, 0x5B, 0x4C, 0x52, 0x41, 0x49, 0x4A };

alt_u8 A_multi_byte_make_code_table[SCAN_CODE_NUM] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x1F, 0, 0, 0x14, 0x27, 0x11, 0x2F, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x70, 0x6C, 0x7D, 0x71, 0x69, 0x7A, 0x75, 0x6B, 0x72, 0x74, 0, 0x4A, 0, 0, 0, 0x5A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	KB_CODE_TYPE decode_mode;
	alt_u8 buf;
	char ascii;
	alt_u8 byte;
*/

void initialize_ps2 () {
	// setting up ps2
	usleep(5000000);
	ps2 = alt_up_ps2_open_dev(PS2_CONTROLLER_NAME);
	if (ps2 == NULL)
		alt_printf ("Error: could not open ps2 device\n");
	else
		alt_printf ("Opened ps2 device\n");

	ps2->timeout = 500000;

	alt_up_ps2_clear_fifo(ps2);

	alt_up_ps2_init(ps2);
	if (ps2->device_type == 0)
		printf("Opened mouse\n");
	else if (ps2->device_type == 1) {
		printf("Opened keyboard\n");
		set_keyboard_rate(ps2, 0xFF);
	} else
		printf("Unknown device\n");
}

/* don't need these for now
//helper function for get_next_state
unsigned get_multibyte_makecode_index(alt_u8 code)
{
	unsigned i;
	for (i = 0; i < SCAN_CODE_NUM; i++ )
	{
		if ( A_multi_byte_make_code_table[i] == code )
			return i;
	}
	return SCAN_CODE_NUM;
}

//helper function for get_next_state
unsigned get_singlebyte_makecode_index(alt_u8 code)
{
	unsigned i;
	for (i = 0; i < SCAN_CODE_NUM; i++ )
	{
		if ( A_single_byte_make_code_table[i] == code )
			return i;
	}
	return SCAN_CODE_NUM;
}
*/
dir getDirectionFromKeyboard(dir lastdir, dir secondlastdir) {

	int PS2_data, RAVAIL;
	unsigned idx = SCAN_CODE_NUM;
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
