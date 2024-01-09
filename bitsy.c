#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include "bitsy.h"

unsigned char byte;
int w_pos = 0; // Bit position for write funtion
int r_pos = 0; // Bit position for read function
char curr_byte;
char bit_buf[5];

/* read_byte
 * Abstraction to read a byte.
 * Relys on readBit.
 */
unsigned short read_byte()
{
	/* If we are buffering data in read_bit, we dont want to skip over
	 * that data by calling read again. Instead, call read_bit and
	 * construct a byte one bit at a type. This also allows us to read
	 * 8 bits that are not necessarily byte alligned. */

	unsigned short whole_byte = 0;
	unsigned short bit;

	for (int i = 0; i < 8; i++)
	{

		bit = read_bit();
		if (bit == 300)
		{ // Checks for EOF

			return 300;
		}
		whole_byte ^= (-bit ^ whole_byte) & (1UL << i); // Sets current bit in current position in whole_byte
	}

	return whole_byte;
}

/* read_bit
 * Abstraction to read a bit.
 * */
unsigned short read_bit()
{
	/* This function is responsible for reading the next bit on the
	 * input stream from stdin. Each time read bit is called, use bitwise
	 * operations to extract the next bit and return it to the caller.
	 * Once the entire buffered byte has been read the next byte from
	 * the file. Once eof is reached, return a unique value > 255
	 */

	unsigned short bit;
	ssize_t num;

	if (r_pos == 0) // Checks if new byte needs to be loaded
	{ 

		num = read(0, bit_buf, 1);
		curr_byte = bit_buf[0]; // Sets curr_byte
	}

	if (num == 0) // Checks for end of file
	{

		return 300; // Returns num larger than byte_max to show EOF
	}

	bit = (curr_byte >> r_pos) & 1U; // Gets the nth bit from the current byte

	if (r_pos == 7) // Checks if last bit of byte has been set
	{

		r_pos = -1; // Resets bit position tracker
	}

	r_pos++; // Increments bit position tracker

	return bit;
}

/* write_byte
 * Abstraction to write a byte.
 */
void write_byte(unsigned char byt)
{
	/* Use writeBit to write each bit of byt one at a time. Using writeBit
	 * abstracts away byte boundaries in the output.*/
	unsigned char bit;

	for (int i = 0; i < 8; i++)
	{
		bit = (byt >> i) & 1U; // Pulls i-th bit from the current byte
		write_bit(bit);		   // Sends bit to be added to output byte
		bit = 0;
	}
}

/* write_bit
 * Abstraction to write a single bit.
 */
void write_bit(unsigned char bit)
{
	/* Keeps a byte sized buffer. Each time this function is called, inserts the
	 * new bit into the buffer. Once 8 bits are buffered, writes the full byte
	 * to stdout.
	 */
	char buf[10];

	byte ^= (-bit ^ byte) & (1UL << w_pos); // Sets bit in byte at position w_pos

	if (w_pos == 7) // Checks if new byte is needed and write out old byte
	{				
		w_pos = -1; // Reset position tracker
		buf[0] = byte;
		write(1, buf, 1); // Writes complete byte
		byte = 0;
	}

	w_pos++; // Increments bit position tracker
}
