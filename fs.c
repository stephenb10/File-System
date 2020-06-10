/* fs.c
   Some useful functions for OSP Practical Case Study E
*/
#include"fs.h"

uint8_t bitmap[20];
struct entry directory[32];

/* Reads in input from user and validates */
int getUserInput()
{
	int choice;
	do
	{
		printf("1. Initalise Disk\n");
		printf("2. List Files\n");
		printf("3. Display Bitmap\n");
		printf("4. Open/Create File\n");
		printf("5. Read File\n");
		printf("6. Write File\n");
		printf("7. Delete File\n");
		printf("8. Exit\n");
		scanf("%d", &choice);
		printf("\n\n");
	} while (choice < 1 || choice > 8);
	return choice;
}

/* Gets the file name and extension from input.
Ensures the name is no longer than 9 characters.
Ensures the file extension is no longer than 4 characters.*/
char* getFileName()
{
	char buffer[100], fileName[14];
	int length;

	printf("Enter the file name to open or create: ");
	scanf("%s", buffer);
	length = strlen(buffer);

	if (length > 13)
	{
		printf("File name must be no longer than 13 characters including extension!\n");
		return NULL;
	}

	strcpy(fileName, buffer);


	// Check to see if user entered the file extension
	char* pos = strstr(buffer, ".");
	if (!pos)
		if (length > 9)
		{
			printf("File name is longer than 9 characters, failed to create file\n");
			return NULL;
		}
		else
		{
			printf("Enter the file extension in the format of \".AAAA\"\n");
			scanf("%s", buffer);
			length = strlen(buffer);
			if (length > 5)
			{
				printf("\nExtension can only be 4 characters long aftet the .\n");
				return NULL;
			}
			else
				strcat(fileName, buffer);
		}

	char* file = fileName;

	return file;
}

/* Search through all available files and compare
the file and extension.
Returns the index in the directory array if found
ele returns -1 if not found. */
int findFile(char* fileName, char* extension)
{
	for (int i = 0; i < 32; i++)
		if (directory[i].user == 1)
		{
			if (strcmp(fileName, directory[i].name) == 0
				&& strcmp(extension, directory[i].extension) == 0)
					return i;
		}
	return -1;
}

/* Create a file with passed name and extension.
Assigns file to next available directory.
Assigns the next available block and toggles it.
Returns a pointer to the file in the directory */
struct entry* createFile(char* fileName, char* extension)
{
	int element = nextAvailableEntry();
	if (element == -1)
	{
		fprintf(stderr, "Max files reached!\n");
		return NULL;
	}

	int block = nextAvailableBlock();
	if (block == -1){
		fprintf(stderr, "No available blocks!\n");
		return NULL;
	}

	struct entry* file = &directory[element];

	strcpy(file->name, fileName);
	strcpy(file->extension, extension);
	file->user = 1;
	file->blockcount = 1;
	file->block[0] = block;
	toggle_bit(block);

	return file;
}

/* Finds the next avaialble position in the directory array 
Returns the index of position if available
else return -1 if the directory is full. */
int nextAvailableEntry()
{
	for (int i = 0; i < 32; i++)
		if (directory[i].user == -1)
			return i;

	return -1;

}

/* Finds the next avaialble block in the bitmap.
Returns the index of position if available
else return -1 if no blocks available. */
int nextAvailableBlock()
{
	for (int i = 1; i < 160; i++)
		if (block_status(i) == 0)
			return i;
	
	return -1;
}


int toggle_bit(int block)
{

	int elem=block/8;
	int pos=block%8;
	int mask=1<<pos;

	bitmap[elem]^=mask;

	return bitmap[elem]&mask;
}


int block_status(int block)
{
	int elem=block/8;
	int pos=block%8;
	int mask=1<<pos;

	return bitmap[elem]&mask;
}