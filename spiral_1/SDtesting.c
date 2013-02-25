/*/*
 * SD Interface Reference
 *
 * This file contains relevant code examples and comments to allow for use of the SD card. Ensure that the files within notes
 * one and two within exercise 3 are overwritten/changed.
 *
 */

#include <stdio.h>
#include <string.h> //this is not used but there is supposed to be support for strings
#include "interface_SD.h"
#include <altera_up_sd_card_avalon_interface.h>

//int main(void) {
//
//	//Creates a pointer so that the sd card can be used using the HAL functions
//	alt_up_sd_card_dev *device_reference = NULL;
//	device_reference = alt_up_sd_card_open_dev("/dev/Altera_UP_SD_Card_Avalon_Interface_0");
//
//	//All functions are usually run once when the SD card is opened, use flags(ints) with values 0/1 to toggle whether the
//	//function has run or not. Set to 0 upon SD card insertion, 1 upon SD card removal
//	int connected = 0;
//	int printed = 0;
//	int filecount = 0;
//	int file0opened = 0;
//	int returnval = 0;
//	int valid = 0;
//
//	//file handle. The first file opened will be assigned this number to refer to the file.
//	short int file0;
//
//	/*The following variables create an array of characters to store the filenames in. It may be possible to use strings
//	 *instead of arrays of char to store data. It is only necessary to initialize them once as they will be overwritten as
//	 *necessary
//	 */
//	char *fileptr;
//	char filenames [1024];
//	fileptr = &filenames[0];
//
//	/*
//	 * These are character buffers for reading data from text files. It will read up to 4kB of text data from a .txt into the
//	 * array. The number of characters in the array is kept track of by charcnt
//	 */
//
//	char *charptr;
//	char charbuffer [1024];
//	charptr = &charbuffer[0];
//
//
//
//	if (device_reference != NULL) {
//
//		//After initializing the HAL drivers, this will always be running.
//		while(1) {
//
//			//Test to see if an SD card is inserted and initializes variables.
//
//			if ((connected == 0) && (alt_up_sd_card_is_Present())) {
//				printf("Card connected.\n");
//
//				//Check if SD card is formatted to FAT16
//				if (alt_up_sd_card_is_FAT16()) {
//					printf("FAT16 file system detected.\n");
//
//					//reset all functions that we want running
//
//					printed = 0;
//					filecount = 0;
//					returnval = 0;
//					file0opened = 0;
//					valid = 1;
//					fileptr = &filenames[0];
//					charptr = &charbuffer[0];
//
//				} else {
//					printf("Unknown file system.\n");
//					valid = 0;
//				}
//
//				connected = 1; //Set connected to 1 so we do not re-initialize the variables
//
//				//Check for SD card removal
//
//
//				//Upon insertion of an appropriately formatted card,
//				//until all files have been printed to screen, run these operations
//
//				while (valid == 1 && alt_up_sd_card_is_Present() && returnval != -1){
//
//					//Add all filenames into an array. There is no way of determining how long the filename is therefore, a
//					//we check for the '.' in the filename and increment the pointer within the filename char array accordingly
//					//The alt_up_sd_card_find_first() function will return the first file in the directory and must be run before
//					//alt_up_sd_card_next() can be run.
//
//					returnval = alt_up_sd_card_find_first(0x00000000, fileptr);
//					//This function takes arguments (base address(can be a pointer or a number), pointer). It is unknown what
//					//the impact of changing the base address is. The pointer is to an address within an char array to write
//					//the filename to
//
//					if(returnval == 0){
//						filecount++;
//						while(*fileptr != '.')
//							fileptr++;
//						fileptr += 4;
//					}
//
//					//Until all filenames have been read into the array, use the alt_up_sd_card_find_next() function in order
//					//to read filenames into the filename array. As with the alt_up_sd_card_find_first() function, the pointer
//					//must be incremented for every character added into the array.
//					while (1){
//
//						returnval = alt_up_sd_card_find_next(fileptr);
//						//This function takes a single argument, a pointer which should be to somewhere within a char array
//						//This determines where the function begins writing the filename to
//
//						if (returnval == -1)
//							break;
//						filecount++;
//						while(*fileptr != '.')
//							fileptr++;
//						fileptr += 4;
//					}
//
//					//Print the names of all files, one file per line
//					//We use the fact that all filenames have a '.' and a three letter extension to print filenames.
//					//After the above runs, the array should have no empty spaces. It contains filecount filenames and therefore
//					//filecount '.''s
//					if (printed == 0){
//						int k = 0;
//						int d = 0;
//						for(k=0; k < filecount; k++){
//							while(filenames[d] != '.'){
//								printf("%c", filenames[d]);
//								d++;
//							}
//							int j = 0;
//							for(j=0; j<4; j++){
//								printf("%c", filenames[d]);
//								d++;
//							}
//							printf("\n");
//						}
//						printed = 1;//All filenames have been printed so set this value to 0
//					}
//
//
//					//Open a file
//					//This functionality has not been fully tested and may contain errors.
//					if (file0opened == 0 && (alt_up_sd_card_is_Present() == true)){
//
//
//						file0 = alt_up_sd_card_fopen("README.TXT", false);
//						//Takes two arguments "<filename>" and a boolean. If true, the file will be created if it does not exist
//						//and will return -1 if the file could not be opened. If the file is already opened it will return -2
//						//Otherwise it will return a short int with the handle used by other functions to address the file
//
//						if(file0 == -1){
//							printf("File could not be opened\n");
//						}
//						else if(file0 == -2){
//							printf("File is already opened\n");
//						}
//						else{
//							//If the file opened, successfully, read and operate on it.
//							printf("File opened successfully with file handle %d\n", file0);
//
//							file0opened = 1;//The file has opened successfully, set the flag
//
//							//-------------------------------------------------------------------------
//							//test code that attempts to implement a simple data structure
//
//
//							//Read the file into the character buffer and simultaneously print it to the screen
//							//Will read up to 4kB of data into a buffer. Should pass an argument in to retrieve
//							//prototype
//							//void read_SD(char &readbuffer, string "filename") filename format "level[name].txt]
//							//Retrieve the data and store it inside the readbuffer
//							//readbuffer should be at minimum 4kB.
//							short int k=0;
//							short int charcnt = 0;
//
//							for(k=0;k<4096;k++){
//								*(charptr+k) = alt_up_sd_card_read(file0);
//								printf("%c",*(charptr+k));
//
//								//If the end of file has been reached, stop reading. EOF is designated with a negative value
//								if (*(charptr+k) < 0){
//									printf("File has been read into the char buffer\n");
//									break;
//								}
//
//								charcnt = k;
//
//							}
//
//							printf("the number of characters in the file is %d\n", charcnt);
//
//
//
//							printf("\nThe end of the file has been reached\n");
//
//							//-----------------------------------------------------------------------
//							//End of testcode.
//
//							printf("file %d is now closing\n", file0);
//							if(alt_up_sd_card_fclose(file0)){
//								printf("The file has now been closed successfully\n");
//							}
//						}
//					}
//				}
//			} else if ((connected == 1) && (alt_up_sd_card_is_Present() == false)) {
//				printf("Card disconnected.\n");
//				connected = 0;
//				while(!alt_up_sd_card_is_Present());
//			}
//		}
//
//	}
//
//	return 0;
//}

int main (void){

	sd_init();

	char charbuffer[1768] = {0};
	int charbuffersize = 1768;
	char* charptr;
	charptr = &charbuffer[0];
	int charcount=0;
	int k;
	int i = 0;
	char* palette;


	palette = bmp_read("black.bmp", charptr);

	printf("read");
	for(i=0;i<16;i++){
		for(k=0;k<20;k++){
			printf("%02hhX ", (unsigned char)*(charptr+k +16*i ));
		}
		printf("\n");
	}
//
//	file_read(charptr, "Readme.txt", 1768);
//	for(i=0;i<100;i++)
//		printf("%c", (unsigned char)charbuffer[i]);
	//
	//	char leveldata[20][6];
	//	char *levelptr;
	//	levelptr = &leveldata[0][0];
	//	int lvlcount;
	//	lvlcount = levelnames (levelptr);
	//
	//	for(i=0;i<lvlcount;i++){
	//		for(k=0;k<6;k++){
	//			printf("%c", leveldata[i][k]);
	//		}
	//		printf("\n");
	//	}



//		charcount = filenames_read(charptr, charbuffersize);
//		printf("The number of files is %d\n", charcount);
//		for(k=0;k<charcount;){
//
//			printf("%c",charbuffer[i]);
//			if(charbuffer[i]== '\n')
//				k++;
//			i++;
//
//		}
	//
	//	charcount = file_read(charptr, "README.TXT", charbuffersize);
	//	for(k=0;k<charcount;k++){
	//
	//		printf("%c",charbuffer[k]);
	//	}
	//
	////	printf("\n");
	//	file_read(charptr, "README.txt", 2048);
	//	for(i=0; i<1200; i++)
	//		printf("%c", charbuffer[i]);

//		file_read(charptr, "blank.bmp", 2048);
//		int multi = *(charptr+0xB);
//		multi *= 256;
//		charptr += *(charptr+0xA);
//		charptr += multi;
//
//		for (i=0;i<120;i++){
//			for(k=0;k<16;k++){
//				printf("%02hhX ", (unsigned char)*(charptr+(i*16)+k));
//			}
//			printf("\n");
//		}
	////
	//	char levelbuffer[10][16];
	//	char objectbuffer [10][16];
	//	char* levelptr;
	//	char* objectptr;
	//	int j;
	//	levelptr = &levelbuffer[0][0];
	//	objectptr = &objectbuffer[0][0];
	//	level_data("level1.txt",levelptr, objectptr);
	//
	//	for(k=0; k<10;k++){
	//		for(j=0; j<16; j++){
	//			printf("%c", levelbuffer[k][j]);
	//		}
	//
	//	}
	//
	//	printf("\n\nBeginning of objectbuffer\n");
	//	for(k=0; k<10;k++){
	//			for(j=0; j<16; j++){
	//				printf("%c", objectbuffer[k][j]);
	//			}
	//
	//		}
	//	printf("\n");
	//
	//	level_save(levelptr,objectptr);



	return 0;

}*/