/*
*	@author - Tasman Mayers
*	@date - October 2017
*
*	This program takes a text file input and a 32 bit integer to encrypt:
*	This program can also decrypt text input using -d
*	Call encrypt using {-e filename key}
*	Call decrypt with {-d filename}
*
*	Depending on system the file extention may be required to operate correctly
*-------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *input;
FILE *output;

/*	Function to convert the text key to an int
*	@char arr[] - input text string
*/
int getkey(char arr[])
{
	char intarr[strlen(arr)];
	int i = 0;
	while (arr[i] != '\n')
	{
		intarr[i] = arr[i] + 15;
		i++;
	}
	intarr[i] = '\0';
	return (atol(intarr));
}

//read in encryption key and decrypt file
void decrypt(void)
{
	//exit if file cant open
	   if ((output == NULL) || (input == NULL))
	   {
		   perror("File failed to open\n");
		   exit(EXIT_FAILURE);
	   }
	   
	//call getkey to convert key to int
	char line[BUFSIZ];
	strcpy(line, fgets(line, sizeof line, input));
	long enc = getkey(line);
	printf("Encryption key is: %ld\n", enc);
	   
	   //pull encryption parameter then continue
	   while (fgets(line, sizeof line, input) != NULL)
	   {
		   for (int i = 0; i < BUFSIZ; ++i)
		   {
			   if ((line[i] != '\0') && (line[i] != '\n')) // range of printable chars
			   {
				   if (i % 2 == 0)
				   {
						line[i] = line[i] - (enc%1000) + 5*(enc%10);
				   }
				   else
				   {
					   
						line[i] = line[i] + (enc%10000) - 8*(enc%100) ;
				   }
			   }
		   }
		   
		   if(fputs(line, output) == EOF)	//exit if a write error occurs
            {
                perror("error copying file\n");
                exit(EXIT_FAILURE);
            }
	   }
	   if (input != NULL) fclose(input); // close file
	   if (output != NULL) fclose(output); // close file
	   printf("Operation Successful.\n");
	   exit(EXIT_SUCCESS);
	
}

/*	Encrypts the input file with the key provided	
*	@long x - encryption key input
*/
void encrypt(long x)
{
	   char line[BUFSIZ];
	   unsigned long encparam = x;
	   
	   //exit if file cant open
	   if ((output == NULL) || (input == NULL))
	   {
		   perror("File failed to open\n");
		   exit(EXIT_FAILURE);
	   }
	   
	   //copy in encryption parameter to file
	   sprintf(line, "%ld", encparam );
	   for (int j = 0; j < BUFSIZ; ++j)
	   {
		   if (line[j] >= '0' && line[j] <= '9')
		   {
			   line[j] = line[j] - 15;
		   }
		   else
		   {  
			   line[j] = '\n';
			   line[j+1] = '\0';
			   break;
		   }
	   }
	   fputs(line, output);
	   
	   while (fgets(line, sizeof line, input) != NULL)
	   {
		   for (int i = 0; i < BUFSIZ; ++i)
		   {
			   if ((line[i] != '\0') && (line[i] != '\n'))
			   {
				   if (i % 2 == 0)	// perform different encryption on every second char
				   {
						line[i] = line[i] + (encparam%1000) - 5*(encparam%10) ;
				   }
				   else
				   {
						line[i] = line[i] - (encparam%10000) + 8*(encparam%100) ;
				   }
			   }
		   }
		   
		   if(fputs(line, output) == EOF)		//quit when write error occurs
            {
                perror("error copying file\n");
                exit(EXIT_FAILURE);
            }
	   }
	   if (input != NULL) fclose(input); // close file
	   if (output != NULL) fclose(output); // close file
	   printf("Encrypted with key: %ld\n", encparam);
	   printf("Operation Successful.\n");
	   exit(EXIT_SUCCESS);
}


/* calls encrypt if -e, decrypt if -d*/
int main(int argc, char *argv[])
{
   if (argc < 3 || argc > 4)
   {
	   perror("Incorrect arg count\n");
	   exit(EXIT_FAILURE); //exit, wrong args
   }
   else if (strcmp("-e", argv[1]) == 0)		//call encrypt function
   {
	   input = fopen(argv[2], "r");
	   output = fopen("HashedOutput", "w");
	   if (atol(argv[3]) < 0)		//crash if key is negative
	   {
		   perror("please use a 32-bit positive integer as encryption key");
		   exit(EXIT_FAILURE);
	   }
	   encrypt(atol(argv[3]));
   }
   else if (strcmp("-d", argv[1]) == 0)		//call decrypt function
   {
	   input = fopen(argv[2], "r");
	   output = fopen("DecryptedOutput", "w");
	   decrypt();
   }
}