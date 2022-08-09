#ifndef BLADE_CONV
#define BLADE_CONV

void hardware_loop_reg (int * ptr_blade, int operand, int width, int stride, int length) ;
void convolution_reg (int * ptr_blade, int accumulation, int reset) ;
int read_32bits (int * ptr_blade, int word, int sa) ;
short read_16bits (short * ptr_blade, int word, int byte, int sa) ;
char read_8bits (char * ptr_blade, int word, int byte, int sa) ;
void write_32bits (int * ptr_blade, int word, int sa, int data) ;
void write_16bits (short * ptr_blade, int word, int byte, int sa, short data) ;
void write_8bits (char * ptr_blade, int word, int byte, int sa, char data) ;
void imc (int * ptr_blade, int operation, int opA, int opB, int res) ;


#endif
