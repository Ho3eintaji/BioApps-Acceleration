# Application: Transformer
Running a full transformer model on HEEPocrates board

# Running applications on HEEPocrates


## Prerequisite

0. Only if you are running on eslsrv


On BASH shell:

```
scl enable devtoolset-10 bash
source env.sh
```

On TCSH shell:

```
scl enable devtoolset-10 tcsh
source env.csh
```

1. You should do all the prerequisite explained in [x-heep](https://github.com/esl-epfl/x-heep)

Don't forget to activate the conda enviroment

```
conda activate core-v-mini-mcu
```

## HEEPocrates testing board
- A clock input with the frequency of 32.768 KHz should be given to the testing board.
    - Frequency: 32.768 KHz
    - Amp: 1.8 V
    - Offset: 900 mV
    - Func: square
- For running the application, we are using flash load mode (more info on HEEPocrates main repo), so be carefull below configuration is set on the board:
    - BOOT SEL: H
    - EXEC FLASH: L
    - BYPASS: L

## Running transformer application

0. first run below command for creating required heepocrates librares
```
make heepocrates-gen
```

1. then run the application (`hello_world` or `seiz_trans`)

```
./run.sh seiz_trans
```

- `/dev/ttyUSB2` is considered as default serial port. 