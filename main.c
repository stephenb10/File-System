#include<stdio.h>
/* stdio.h will be found in the system path */
#include"fs.h"
/* fs.h will be found in the local path */

/*
• With the name and extension fields being C strings. This structure is 32 bytes in size.
• The disk size is 160 kbyte. (This is roughly one side of a 5 1/4" inch disk.)
• The smallest unit of allocation is 1 kbyte.
• The main directory occupies the first block of the disk (block 0), and its size is fixed at 1 block, so there can only be 32 files in this file system.
• As the directory always occupies only the first block, therefore no control information about it needs to be stored in the directory (i.e. no . entry).
• the only user is user 1
• user −1 is not a valid user, and could be used to mark free directory entries.
• alongside the directory you also need a bitmap that is capable of representing all of the blocks available on the disk, this can be a free space bitmap or
	an allocation bitmap, this is your choice. This structure is not stored on the disk but would be computed by the operating system when the disk was inserted.
	You are not supposed to implement the actual storage, only the control structures of the file system. When implementing the free bitmap you must use a bitmap,
	i.e. it should be an array, but each element of the array should represent several blocks.
*/


int main(int ac, char** av)
{
	struct entry* currentFile = malloc(sizeof(struct entry));
	currentFile = NULL;

	while (1)
	{
		int input = getUserInput();

		switch (input) {
			/* initialise disk control structures, setting the first block of the disk to used in the bitmap, and marking all directory entries as being available. */
		case 1:
			toggle_bit(0);
			for (int i = 0; i < 32; i++)
				directory[i].user = -1;
			break;

			/* List the names, extensions and block counts of all the valid files in the directory */
		case 2:

			if (block_status(0) == 0)
			{
				printf("Disk has not been initialised!\n\n");
				break;
			}
			printf("Current Directory:\n");

			for (int i = 0; i < 32; i++)
				if (directory[i].user == 1)
					printf("File: %s.%s\nBlock Count: %d\n\n", directory[i].name, directory[i].extension, directory[i].blockcount);
			
			break;

			/* print the value of each of the bits in the bitmap. This need not be pretty, just a long list of 1’s and 0’s is sufficient */
		case 3:
			for (int i = 0; i < 160; i++)
			{
				int bit = 0;
				if (block_status(i) > 0)
					bit = 1;
				printf("Bit %d: %d\n", i, bit);
			}
			break;

			/* scans the directory and if the name provided doesn’t exist then adds that file to the directory.
			This file will be used in all subsequent operations until a new file is opened or it is deleted. */
		case 4:
		{
			if (block_status(0) == 0)
			{
				printf("Disk has not been initialised!\n\n");
				break;
			}

			char* fileName = getFileName();
			//printf("got file name\n");
			//printf("Using fuleName for user %s", fileName);
			if (fileName == NULL)
			{
				printf("Failed to create file\n\n");
				break;
			}

			char name[9], extension[4];

			char* pos = strstr(fileName, ".");
			int index = 0;
			if (pos)
			{
				// string up until .
				index = pos - fileName;
				// terminate string just before the .
				fileName[index] = '\0';

				// file name half of string
				strcpy(name, fileName);
				fileName[index] = '.';
				// extension half of the string excluding .
				strcpy(extension, fileName + index + 1);
			}

			int f = findFile(name, extension);

			if (f == -1)
			{
				printf("Creating file...\n\n");
				currentFile = createFile(name, extension);
			}
			else
			{
				printf("Oppening file...\n\n");
				currentFile = &directory[f];
			}

			break;
		}
			/* list the blocks occupied by the currently open file (not the content of these blocks as you don’t store information) */
		case 5:

			if (currentFile == NULL)
			{
				printf("No file is currently opened!\n\n");
				break;
			}

			printf("Blocks for %s\n", currentFile->name);
			for (int i = 0; i < currentFile->blockcount; i++)
				printf("Block: %d\n", currentFile->block[i]);
			break;

			/* allocate another block to the currently open file. You should not preallocate blocks for the file,
			you allocate the first available block, by scanning the bitmap for the first block that is available.
			Each shall add another block to the file until there are no more slots to allocate blocks to, or the disk out of blocks.
			(There are only 8 slots available for each file.) */
		case 6:

			if (currentFile == NULL)
			{
				printf("No file is currently opened!\n\n");
				break;
			}

			if (currentFile->blockcount >= 8)
			{
				fprintf(stderr, "File has max 8 blocks already!\n");
				break;
			}

			int block = nextAvailableBlock();
			if (block == -1)
			{
				fprintf(stderr, "No blocks available!\n\n");
				break;
			}

			currentFile->block[currentFile->blockcount++] = block;
			toggle_bit(block);
			break;

			/* deallocate all blocks for the current file in the bitmap, and marks as free the directory entry for that */
		case 7:
		{

			if (currentFile == NULL)
			{
				printf("No file is currently opened!\n\n");
				break;
			}

			printf("Deleting file %s:\n", currentFile->name);
			currentFile->user = -1;
			for (int i = 0; i < currentFile->blockcount; i++)
				toggle_bit(currentFile->block[i]);
			currentFile->blockcount = 0;
			break;
		}
		case 8:
			printf("Terminating...\n");
			exit(0);
		default:
			break;
		}

	}
	return 0;

}


