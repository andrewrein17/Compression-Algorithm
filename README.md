This compression algorithm reads the input one symbol (i.e., byte) at a 
time and compares it with each of the 8 bytes previously seen.  It also 
checks to see if the following n characters are the same as the current 
symbol.  If the byte has been previously seen, it outputs the position of 
the previous byte relative to the current position, Otherwise, the symbol 
is output as is by prefixing it with a binary one. 
Achieves ~70% compression.
To compile czy: make czy
To execute: ./czy < somefile.txt > somefile.encoded
   
