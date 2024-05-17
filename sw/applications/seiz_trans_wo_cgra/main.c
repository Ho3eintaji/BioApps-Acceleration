//
// Created by alireza on 10/6/23.
//

#include <stdio.h>
#include <math.h>
#include "main.h"
//#include "data_cpp/signal.cpp"
//#include "data_cpp/signal_fft.cpp"
#include "fft.h"
#include "weightsAndBiasesC.h"
#include "transformerBlockC.h"

#include "performance.h"

// FFT
//#include "stftVec.h"
#include "data_cpp/array_output.h"

#include <stdio.h>
#include <stdlib.h>
#include "core_v_mini_mcu.h"
#include "power_manager.h"
#include "gpio.h"
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
#include "csr.h"
#include "hart.h"
#include "handler.h"
#include "heepocrates.h"


// For FLL
#include "soc_ctrl.h"
#include "rv_timer.h"
#include "fll.h"
// For GPIO
#include "gpio.h"

// System frequency
const uint64_t SYS_FREQ = 160*1000000; //MHz
void fll_cfg(uint64_t freq);
// Gpio
#define PIN_TRIGGER     4  //used for trigering and checking on oscilloscope
void gpio_output_cfg(gpio_t *gpio, uint32_t pin);
gpio_t gpio;

float error_check(const quant_bit_width* groundTruth, const quant_bit_width* output, size_t length){
    long error = 0;
    for (int i=0; i<length; i++){
        error += MUL_HQ(groundTruth[i] - output[i], groundTruth[i] - output[i]);
    }
    error = (error >> NUM_FRACTION_BITS);
    return (float) error/ (float) length;
}

void prototype_distances(quant_bit_width* prototypeVec, const quant_bit_width* modelOutput, int32_t* distVec, size_t prototypeLength, int prototypeNums){
    for (int p=0; p< prototypeNums; p++){
        long dist = 0;
        quant_bit_width * prototypePtr = prototypeVec + (p * prototypeLength);
        for (int i=0; i<prototypeLength; i++){
            dist += MUL_HQ(prototypePtr[i] - modelOutput[i], prototypePtr[i] - modelOutput[i]);
        }
        dist = (dist >> NUM_FRACTION_BITS);
        distVec[p] = (int32_t) dist;
    }
}

void transformerInference(quant_bit_width * transformerInput, quant_bit_width * transformerOutput, quant_bit_width* input_normalized, quant_bit_width* qkv, quant_bit_width* intermediate, quant_bit_width * aux_padding,  void* kperf){
    quant_bit_width * weightVec[NUM_LAYERS*(3*NUM_HEAD+5)+5];
    quant_bit_width * biasVec[NUM_LAYERS*(3*NUM_HEAD+5)+5];
    getWeights(weightVec);
    getBiases(biasVec);
    quant_bit_width * clsTokenVector = getClassToken();
    quant_bit_width * posMatrix = getPosEmbedding();
    TransformerBlock* selfatten = createTransformerBlock(D_SEQ, D_MODEL, D_Q, NUM_HEAD, D_FF, weightVec, biasVec, clsTokenVector, posMatrix);
    computeFixedPoint(selfatten, D_SEQ, transformerInput, input_normalized, transformerOutput, intermediate, qkv, aux_padding, kperf);
}

quant_bit_width compute_log_amp(int32_t real, int32_t imag){
    real = MUL_HQ(real, 25) >> (NUM_FRACTION_BITS - 9);
    imag = MUL_HQ(imag, 25) >> (NUM_FRACTION_BITS - 9);
    int32_t real2 = MUL_LONG(real, real) >> NUM_FRACTION_BITS;
    int32_t imag2 = MUL_LONG(imag, imag) >> NUM_FRACTION_BITS;
    float pow2 = (float)(real2 + imag2) / (float) (1<< NUM_FRACTION_BITS);
    float amp = sqrtf(pow2);
    float stft = logf(amp+ 1e-10f);
    quant_bit_width stft_int = (quant_bit_width) (stft * (1<<NUM_FRACTION_BITS));
    return stft_int;
}

/*
void initialize_stft(fft_complex_t* data, const quant_bit_width * raw_input_signal){
    // Initialize each element of the data array
    for (int i = 0; i < 256; i++) {
        data[i].r = (MUL_HQ(raw_input_signal[i], hanning[i])) ;
        data[i].i = 0;
    }
    for (int i = 256; i < 512; i++) {
        data[i].r = 0;
        data[i].i = 0;
    }
}
*/

/*
void stft_rearrange(quant_bit_width* rawInputSignal, quant_bit_width* stftVec, size_t patchHeight, size_t patchWidth){
    fft_complex_t data[512];
    int overlap = 64;
    for (int ch=0; ch<20; ch++){
        for (int time_step=0; time_step<15; time_step++){
            quant_bit_width* rawSignalPtr = rawInputSignal + ch * 3072 + (256 - overlap) * time_step;
            initialize_stft(data, rawSignalPtr);
            fft_fft(data, 9);
            quant_bit_width * stftVecPtr = stftVec + ch * 15 * 160 + (time_step / patchWidth) * patchWidth * patchHeight + (time_step % patchWidth);
            for (int index =0 ; index < patchHeight; index++){
                int16_t stft_int = compute_log_amp(data[index].r, data[index].i);
                *stftVecPtr = (int32_t) stft_int;
                stftVecPtr += patchWidth;
            }
            stftVecPtr += patchHeight * patchWidth * 2;
            for (int index = patchHeight ; index < 2*patchHeight; index++){
                int16_t stft_int = compute_log_amp(data[index].r, data[index].i);
                *stftVecPtr = (int32_t) stft_int;
                stftVecPtr += patchWidth;
            }
        }
    }
}
*/


int main() {

    fll_cfg(SYS_FREQ); //Set frequency
	gpio_output_cfg(&gpio, PIN_TRIGGER); // GPIO configuration for toggling

    kcom_perf_t kperf;
    timerInit(); // Init timer


    // Transformer   
    // Every buffer is 16x16 = 256
    quant_bit_width* input = STFT_rand_out; // 1st
    quant_bit_width* output = STFT_rand_out + 4800; // Last

    quant_bit_width* input_normalized = input + 256; // 2nd
    quant_bit_width* qkv = input_normalized + 256; 
    quant_bit_width* intermediate = qkv + 256*4; // 4 = K, V, Q and Kt
    quant_bit_width* aux_padding = intermediate + 256;
    
    int32_t distances[2];
    //stft_rearrange(rawInputSignal, stftVec, 80, 5);
        
    kcom_perfRecordStart(&(kperf.time.infer));
    // while(1){
        gpio_write(&gpio, PIN_TRIGGER, true);
        transformerInference(input, output, input_normalized, qkv, intermediate, aux_padding, (void *) &kperf);
        gpio_write(&gpio, PIN_TRIGGER, false);
        //a delay
        // for (int i = 0; i < 1000000; i++) {asm("nop");}
        // for (int i = 0; i < 1; i++) {asm("nop");}
    // }
    kcom_perfRecordStop(&(kperf.time.infer));

    printf("\rCycles: %d\n", kperf.time.infer.spent_cy);
    
    prototype_distances(prototypes, output, distances, D_MODEL, 2);
    
    printf("\rDistances:\n");
    for (int i = 0; i< 2; i++)
        printf("Class %d = %d\n", i, distances[i]);
    
    printf("\rEND\n");
    return 0;
}


void fll_cfg(uint64_t freq) {
  uint32_t fll_freq_real;
  fll_t fll;
  soc_ctrl_t soc_ctrl;
  uint32_t fll_status;
  // 2.1 FLL peripheral structure to access the registers
  fll.base_addr = mmio_region_from_addr((uintptr_t)FLL_START_ADDRESS);
  fll_init(&fll);
  fll_status = fll_status_get(&fll);
  soc_ctrl.base_addr = mmio_region_from_addr((uintptr_t)SOC_CTRL_START_ADDRESS);
  //2.4 Set default app frequency
  fll_set_freq(&fll, freq);
  for (int j = 0; j < 10000; j++) {
    asm volatile("nop");
  }
  fll_freq_real = fll_get_freq(&fll);
  soc_ctrl_set_frequency(&soc_ctrl, fll_freq_real);
}

void gpio_output_cfg(gpio_t *gpio, uint32_t pin) {
	gpio_params_t gpio_params;
    gpio_params.base_addr = mmio_region_from_addr((uintptr_t)GPIO_AO_START_ADDRESS);
    gpio_init(gpio_params, gpio);
    gpio_output_set_enabled(gpio, pin, true);
    gpio_write(gpio, pin, false);
}

