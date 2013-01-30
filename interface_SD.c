/*
 * SD_Interface.c
 *
 *  Last Modified: Jan 29, 2013
 *  Author: Leslie Mui
 *
 *  This file contains methods for functionality from the SD card
 *  All read operations read into the a buffer supplied by the macrofunction that calls it
 *  This is to enable the use of a single multi-purpose array for reading to the SD card and
 *  at a future date, write operations may be supported. The pointer to the filenames is not modified
 *  by the functions below and may be reused by other functions
 *
 */

/*
 * This requires a character array to be created beforehand and a pointer to the beginning of it.
 * It is recommended that the character buffer be at minimum 256 characters long.
 * The arguments taken by the function are a pointer to the beginning of an array to store the data from the file,
 * the name of a file to be read from and the length of the array. The name can be input directly i.e. "Readme.txt"
 * No information is known about the case sensitivity of functions so matching the capitalization is prudent
 * The function returns an integer with the number of characters read into the array. If an error occurs and the file
 * cannot be read, it will return a negative number with:
 * -1, file could not be read
 * -2, file is open/in use by another application
 */

#include <stdio.h>
#include "interface_SD.h"
#include <altera_up_sd_card_avalon_interface.h>


/*
 * This function is used for reading data to a buffer from the SD card. It takes in three arguments, a
 * pointer to a character buffer where the data will be stored, the name of a file i.e. "Readme.txt"
 * and the size of the character buffer. It will return the number of
 * characters read into the buffer.
 */
int file_read(char* charbuffer, char* filename, int charmax){



	//Check if the SD card is inserted and the formatting correct before reading from it.

	if ((alt_up_sd_card_is_Present())) {
		if (alt_up_sd_card_is_FAT16()) {

			short int file0;
			int k;
			int charcount;

			//Open the file in the argument for reading. False means that the file will not be created
			//if it is not found. A negative return value means that the file was not opened successfully
			file0 = alt_up_sd_card_fopen(filename, false);

			if (file0 == -1) return -1;
			if (file0 == -2) return -2;

			//Read as many characters as possible from the file, up to charmax or until the file has been read
			//to completion.
			else {
				for(k=0; k<charmax; k++){
					*(charbuffer+k) = alt_up_sd_card_read(file0);

					//If the end of file has been reached, stop reading. EOF is designated with a negative value
					if (*(charbuffer+k) < 0){
						break;
					}

					charcount = k;

				}

			}

			//Close the file and return the amount of characters read into the buffer
			alt_up_sd_card_fclose(file0);
			return charcount;
		}

	}


}

/* Returns the number of files within the SD card and adds all all filenames to an array provided
 * by the calling function.
 * The array will contain the filenames in the format [filename].[extension].
 * The filename must be eight characters or less in length and have a three character extension
 * The function returns the number of files within the array
 */

int filenames_read(char* filename, int charmax){

	//Test if the SD card is inserted and is in a valid file format for reading

	if ((alt_up_sd_card_is_Present())) {
		if (alt_up_sd_card_is_FAT16()) {

			int offset = 0;
			int filecount = 0;

			int returnval = alt_up_sd_card_find_first(0x0, filename);

			/*
			 * Read the first filename within the SD card as long as there is sufficient space within the character buffer
			 */
			if((returnval == 0))
			{
				filecount++;
				while(*(filename+offset) != '.')
					offset++;
				offset+=4;
				*(filename+offset) = '\n';
				offset++;
			}

			/* As long as there is sufficient space within the character buffer, will read every name within the
			 * SD card to a buffer.
			 */
			while (1){

				returnval = alt_up_sd_card_find_next((filename+offset));

				if (returnval == -1 ){
					break;
				}

				filecount++;

				if((charmax-offset) <= 12){
					break;
				}
				while(*(filename+offset) != '.'){
					offset++;
				}
				offset+=4;
				*(filename+offset) = '\n';
				offset++;
			}

			return filecount;

		}
	}

	return -1;

}


/*
 * This function adds the leveldata from a text file into a 10 character buffer for storing the level data
 * This function takes two arguments, the filename and a pointer to the beginning of the
 * levelbuffer.
 */
void level_data(char* filename, char* levelbuffer){

	char data[256];
	char* dataptr;
	int k;
	dataptr = &data[0];

	file_read(dataptr, filename, 256);

	while (*dataptr != '*')
		dataptr++;
	dataptr++;
	for(k=0; k<10; k++){
		*levelbuffer = *dataptr;
		levelbuffer++;
		dataptr++;
	}

}

