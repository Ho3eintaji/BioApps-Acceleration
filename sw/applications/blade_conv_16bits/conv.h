#ifndef BLADE_CONV
#define BLADE_CONV

void data_placement (int* ptr_blade, short image [1024], short filter [6][25]) ;
void convolution (int* ptr_blade) ;
void result_reading (int* ptr_blade, short result_filter [6][28][28]) ;

#endif
