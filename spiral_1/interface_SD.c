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
 * This function initializes the SD card for use. It must be run once and only once before any other functions within this file are used
 */
void sd_init(void){

	alt_up_sd_card_dev *device_reference = NULL;
	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");
}


/*
 * This function is used for reading data to a buffer from the SD card. It takes in three arguments, a
 * pointer to a character buffer where the data will be stored, the name of a file i.e. "Readme.txt"
 * and the size of the character buffer. It will return the number of
 * characters read into the buffer.
 *
 * Example of use
 *
 *  char charbuffer[1024];
	int charbuffersize = 1024;
	char* charptr;
	charptr = &charbuffer[0];
	int charcount=0;
 *  charcount = file_read(charptr, "README.TXT", charbuffersize);
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
					*(charbuffer+k) = (char)alt_up_sd_card_read(file0);

					//If the end of file has been reached, stop reading. EOF is designated with a negative value
					if ((*(charbuffer+k) == -1) && (*(charbuffer+k-1) == -1) && (*(charbuffer+k-2) == -1) && (*(charbuffer+k-3)== -1)){
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
	return -1;

}

/* Returns the number of files 	 within the SD card and adds all all filenames to an array provided
 * by the calling function.
 * The array will contain the filenames in the format [filename].[extension].
 * The filename must be eight characters or less in length and have a three character extension
 * The function returns the number of files within the array
 *
 * Example of use:
 *
 * 	char charbuffer[1024];
	int charbuffersize = 1024;
	char* charptr;
	charptr = &charbuffer[0];
	int charcount=0;

	charcount = filenames_read(charptr, charbuffersize);
 */

int filenames_read(char* filename, int charmax){

	//Test if the SD card is inserted and is in a valid file format for reading

	if ((alt_up_sd_card_is_Present())) {
		if (alt_up_sd_card_is_FAT16()) {

			int offset = 0;
			int filecount = 0;

			int returnval = alt_up_sd_card_find_first(0x0, filename);
			if (returnval < 0){
				printf("the read failed");
			}

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
 * Requires a pointer to a matrix in the format levelmatrix [n][6] where n is an integer
 * stores the filenames, one per line, 6 characters for every name
 * returns the number of levels stored on the SD card
 */
int levelnames(char* levelmatrix){

	char filenames[1024];
	char* files;
	int lvlcount = 0;
	files = &filenames[0];
	int filecount = filenames_read(filenames, 1024);
	int count = 0;
	while (count < filecount){

		if(*files == '.')
			count++;
		if(*files == 'L' && *(files+1) == '_'){
			lvlcount++;
			files +=2;
			int i = 0;
			while(*files != '.'){
				*(levelmatrix+i) = *files;

				files++;
				i++;
			}
			levelmatrix += 6;

		}
		files++;

	}
	return lvlcount;

}
/*
 * This function adds the leveldata from a text file into a 10 character buffer for storing the level data
 * This function takes two arguments, the filename and a pointer to the beginning of the
 * levelbuffer.
 *
 * Example of use:
 *
 *  char levelbuffer[10][16];
 *  char objectbuffer [10][16];
	char* levelptr;
	char* objectptr;
	int j;
	levelptr = &levelbuffer[0][0];
	objectptr = &objectbuffer[0][0];
	level_data("level1.txt",levelptr, objectptr);
 */
void level_data(char* filename, char* levelbuffer, char* objectbuffer){

	char data[1024];
	char* dataptr;
	int width;
	int height;
	int k;
	int j;
	dataptr = &data[0];

	file_read(dataptr, filename, 1024);

	//Search for the beginning of leveldata marker '*' and then read the data into the array.
	while (*dataptr != '^')
		dataptr++;
	dataptr++;
	if(*dataptr == 'S'){
		width = 16;
		height = 10;
	}
	else if(*dataptr == 'M'){
		width = 20;
		height = 16;
	}
	else{
		width = 32;
		height = 24;
	}



	while (*dataptr != '*')
		dataptr++;
	dataptr++;
	for(k=0; k<height; k++){
		for(j=0;j<width;j++){
			*levelbuffer = *dataptr;
			levelbuffer++;
			dataptr++;
		}
	}
	//Search for the beginning of objectdata marker '$' and then read the following data into the array

	while (*dataptr != '$')
		dataptr++;
	dataptr++;
	for(k=0; k<height; k++){
		for(j=0;j<width;j++){
			*objectbuffer = *dataptr;
			objectbuffer++;
			dataptr++;
		}
	}

}

void level_save(char* levelbuffer, char* objectbuffer){
	if ((alt_up_sd_card_is_Present())) {
		if (alt_up_sd_card_is_FAT16()) {

			short int save;
			int k;

			//Open the file in the argument for reading. If no save exists, create it.
			save = alt_up_sd_card_fopen("save.txt", true);
			alt_up_sd_card_write(save, '*');
			for (k=0;k<320;k++){
				alt_up_sd_card_write(save, *levelbuffer);
				levelbuffer++;
			}
			alt_up_sd_card_write(save, '$');
			for (k=0;k<320;k++){
				alt_up_sd_card_write(save, *objectbuffer);
				objectbuffer++;
			}
			alt_up_sd_card_fclose(save);
			return;

		}
	}
}

void level_load(char* levelbuffer, char* objectbuffer){
	level_data("playersave.txt",levelbuffer,objectbuffer);
}

/*
Enter a filename such as "blank.txt" including the quotes, and an array with enough storage for one byte per pixel (20x24 = 480 bytes = 480 element array)
It will return a pointer to an array containing three byte color data. The colours in the array are arranged in BGR order.
I'm not sure about how colours are saved within windows. There is a chance that all palettes are identical, in which case we only need to strip the palette
once. Inside the initial array, all pixels are stored going from left to right, row by row from the bottom to top of the image. The size of the image is not stripped
so I propose to have the storage array change based on the size of the level (S/M/L)
 */


char* bmp_read(char* filename, char* pixeldata) {

	char image[2048] = {0};
	char* imageptr;
	imageptr = &image[0];
	char palette[768] = {0};
	char* palptr;
	palptr = &palette[0];
	int i;
	int offset = 0x36;

	file_read (imageptr, filename, 2048);
	offset += *(imageptr+0xA);
	offset += *(imageptr+0xB)*256;

	for(i=0; i<2048;i++){
		if(i%16 == 0)
			*(pixeldata+i)= *(imageptr+i);

	}

	for(i=0; i<256; i++){
		palette[3*i] = *(imageptr + 3*i);
		palette[3*i+1] = *(imageptr+3*i+1);
		palette[3*i+2] = *(imageptr+3*i+2);
	}

	return palptr;

}


