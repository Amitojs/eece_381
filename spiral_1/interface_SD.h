/*
 * SD_interface.h
 *
 *  Created on: Jan 29, 2013
 *      Author: Leslie
 *
 *      This header file defines the functions utilized within SD_Reader.c
 *      The functions currently available and working are file_read() and filenames_read()
 *
 */

#ifndef SD_INTERFACE_H_
#define SD_INTERFACE_H_

#include <stdio.h>


//prototype for initialization of the SD card
void sd_init(void);
//prototype for file reader.
int file_read(char*, char*, int);
//Prototype for the filename reader.
int filenames_read(char*, int);
//prototype for level data
void level_data(char*, char*, char*);
//prototype for saving the gamestate
void level_save(char*, char*);
//prototype for loading the gamestate
void level_load(char*, char*);
//prototype for getting level names
int levelnames(char* levelmatrix);
//prototype for reading a bitmap image
char* bmp_read(char*, char*);

#endif /* SD_INTERFACE_H_ */
