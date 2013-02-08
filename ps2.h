/*
 * ps2.h
 *
 *  Created on: Feb 6, 2013
 *
 */
#ifndef PS2_H_
#define PS2_H_

#include "altera_up_avalon_ps2.h"
#include "altera_up_ps2_keyboard.h"
#include "main.h"

#define SCAN_CODE_NUM  102

void initialize_ps2();

//dir getDirectionFromKeyboard(dir lastdir);

//helper function for get_next_state
unsigned get_multibyte_makecode_index(alt_u8 code);

//helper function for get_next_state
unsigned get_singlebyte_makecode_index(alt_u8 code);


#endif /* PS2_H_ */
