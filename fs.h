#ifndef FS_H
#define FS_H
/* Prevent multiple inclusion */

#include<stdint.h>
#include<stdio.h>
#include<string.h>
/* fs.h
 * Various definitions for OSP Practical Case Study E
 */

/* The bitmap */
extern uint8_t bitmap[20];
/* 160Kb disk with 1Kb blocks-> 160 bits for bitmap -> 20 bytes
 */

/* The directory entry */
struct entry
{
   	char  user;
	char  name[9];
	char  extension[4];
	short blockcount;
	short block[8];
};


/* The Directory */
extern struct entry directory[32];

/* extern means its defined in another
   file, prevents multiple definition 
   errors
*/

int toggle_bit(int block);
/* Toggles the value of the bit 'block', in
   the external array 'bitmap'.
   returns the current value of the bit
   Does NOT validate 'block'!!!
*/
int block_status(int block);
/* Returns the status of 'block',
   in the external array bitmap
   returns 0 if bitmap bit is 0, 
   not 0 if bitmap bit is 1
   Does NOT validate block!!!
*/


int getUserInput();
char* getFileName();
int findFile(char* fileName, char* extension);
struct entry* createFile(char* fileName, char* extension);

int nextAvailableEntry();
int nextAvailableBlock();




#endif

