# the_synth_due

This is a work in progress 8 voice synthesizer using referances from dzlonline the_synth code and referances from gligli's overcycler code. 

The code is built off of the Arduino due, thus it is built for the Atmel SAM3X line of Arm processors. Current specifications of the synth include:

* 8 voices
* 8-bit Sample playback
* 32,000Hz sample rate
* Four stage ADSR envelope

Currently the TLC7528 8-bit parallel DAC is being used to transfer the data into an analog voltage, but any equivelant DAC will work. The data is written to the first 8 bytes of the REG_PIOD_ODSR register.

This code is currently a work in progresses so it is not a gurentee that everything will be in working order.
