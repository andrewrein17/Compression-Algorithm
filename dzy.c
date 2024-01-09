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

void loadByte(){ //Keeps track of the last 8 bytes seen

   for(int i = 7; i > 0; i--){

        bytes[i] = bytes[i-1];

   }

}

int flag = 0;
int total_bytes = 0;
/* This program decompresses a compressed stream directed at its standard input 
 * and writes decompressed data to its standard output.
 * To compile dzy: make dzy
 * To execute: ./dzy < somefile.encoded > somefile_decoded.txt
 */
int main(int argc, char *argv[]){

	unsigned char bit = read_bit();
	unsigned char pos = 0;
	unsigned short byte;

	while(1){

	   if(bit == 1){

		byte = read_byte();	

		if(byte == 300){ //Check for end of input stream
		   flag = 1;
		   break;
		}

		write_byte(byte);
		total_bytes++;
		loadByte();
		bytes[0] = byte;

		bit = read_bit();


	   }else if(bit == 0){ //If a frequent bit is seen, read next three bits to get position

		bit = read_bit();
		pos ^= (-bit ^ pos) & (1UL << 0);
	
		bit = read_bit();
                pos ^= (-bit ^ pos) & (1UL << 1);
		
		bit = read_bit();
                pos ^= (-bit ^ pos) & (1UL << 2);

		byte = bytes[pos];
		write_byte(byte);
		total_bytes++;

		loadByte();
		bytes[0] = byte;

		bit = read_bit();

	   }else{

		return -1;

	   }

	}

	if(flag == 0 && total_bytes > 2){
	
	   write(1, "\n", 1);

	}

	write(1, "\n", 1);
	
	return 0; //exit status. success=0, error=-1
}
