# Applications on HEEPocrates with CGRA Acceleration

This repository contains a suite of applications designed to run on the HEEPocrates board, an [X-HEEP](https://github.com/esl-epfl/x-heep) based platform. The primary goal of this project is to showcase and benchmark the performance of a Coarse-Grained Reconfigurable Array (CGRA) for accelerating various computational kernels compared to standard CPU execution.

The applications include:
* **Benchmark Kernels:** `fft`, `matmul`, and `conv2d`. Each of these can be compiled to run on either the main CPU or be offloaded to the CGRA for acceleration.

* **Seizure Detection Application:** A complete two-channel seizure detection application (`seiz_trans`) based on a lightweight Transformer model. This application demonstrates a real-world use case for CGRA acceleration. A CPU-only version (`seiz_trans_wo_cgra`) is also provided for direct comparison.


#### Repository Structure
The main applications are located in the sw/applications/ directory:

```text
.
└── sw/
    └── applications/
        ├── conv2d/
        ├── fft/
        ├── matmul/
        ├── seiz_trans/
        └── seiz_trans_wo_cgra/
```

#### Prerequisites
##### 1. Environment Setup
Before running any application, you must set up the required development environment.
For `bash` shell:
```bash
# Only required if you are running on eslsrv
scl enable devtoolset-10 bash
# Source the project environment
source env.sh
```
For `tcsh` shell:
```bash
# Only required if you are running on eslsrv
scl enable devtoolset-10 tcsh
# Source the project environment
source env.csh
```
##### 2. X-HEEP Prerequisites & Conda Environment
Ensure you have completed all the prerequisite steps outlined in the main [x-heep repository](https://github.com/esl-epfl/x-heep).
Activate the correct conda environment:
```bash
conda activate core-v-mini-mcu
```
#### HEEPocrates Hardware Setup
For proper operation, the HEEPocrates testing board requires the following hardware configuration:

**1. External Clock:**  An external clock source must be connected.
- Frequency: 32.768 KHz
- Amplitude: 1.8 V
- Offset: 900 mV
- Waveform: Square

**2. Board Switches:**  The applications are loaded via flash. Set the on-board switches to the following positions:
- BOOT_SEL: H
- EXEC_FLASH: L
- BYPASS: L

#### Building and Running Applications
##### 1. Generate HEEPocrates Libraries
Before compiling any application for the first time, you need to generate the necessary platform libraries. Run this command from the root of the repository:
```bash
make heepocrates-gen
```

##### 2. Running an Application
All applications can be compiled and run using the `run.sh` script.
```bash
./run.sh <application_name>
```
Where `<application_name>` is one of the directories in `sw/applications/`.
The default serial port is `/dev/ttyUSB2`. If you use a different port, you may need to modify the run script.

##### 3. Selecting Execution Target (CPU vs. CGRA)
For the benchmark kernels (`fft`, `matmul`, `conv2d`), you can choose whether to run the computation on the CPU or offload it to the CGRA. This is controlled by a preprocessor macro in the `main.c` file of each application.
To run on the **CGRA**, ensure the `#define RUN_CGRA` line is present and uncommented:
After changing the file, re-run the application with the `./run.sh` script to compile and execute the new version.

#### Example Applications

##### Seizure Detection Transformer (seiz_trans)
This is a lightweight, 2 channel version of TSD, Transformer-based model for seizure detection that processes two channels of data. It serves as the primary demonstrator for the benefits of CGRA acceleration.
`seiz_trans` offload matmul kernels on CGRA, and `seiz_trans_wo_cgra` is pure CPU version.