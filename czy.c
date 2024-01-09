#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include "bitsy.h" 

char bytes[8];

void loadByte(){ //Keeps track of last 8 bytes seen in input stream

   for(int i = 7; i > 0; i--){

	bytes[i] = bytes[i-1];

   }

} 

int check(char c){ //Returns position on success or -1 on failure

   for(int i = 0; i < 8; i++){

	if(bytes[i] == c){
	   return i;
	}

   }

   return -1;

}

/*
 * This compression algorithm reads the input one symbol (i.e., byte) at a 
 * time and compares it with each of the 8 bytes previously seen.  It also 
 * checks to see if the following n characters are the same as the current 
 * symbol.  If the byte has been previously seen, it outputs the position of 
 * the previous byte relative to the current position, Otherwise, the symbol 
 * is output as is by prefixing it with a binary one.
 *
 * To compile czy: make czy
 * To execute: ./czy < somefile.txt > somefile.encoded
 */
int main(int argc, char *argv[]){
	
	ssize_t num;
	unsigned char buf[1];
	num = read(0, buf, 1);
	int cmp;
	unsigned char bit;

	if(num == -1){

	   return -1; 

	}

	while(num != 0){

	   cmp = check(buf[0]); //Checks if element frequent or not

	   if(cmp == -1){ //If not frequent

		write_bit(1);
		write_byte(buf[0]);

	   }else{ //If frequent

		write_bit(0);
		for(int i = 0; i < 3; i++){ //Write the three-bit position

		   bit = (cmp >> i) & 1U;
		   write_bit(bit);

		}

	   }

	   loadByte();
	   bytes[0] = buf[0];

	   num = read(0, buf, 1); //Reads byte from stdin

	}

	bit = 1;
	write_bit(bit);
	
	return 0;
	
}

