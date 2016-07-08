# the_synth_due

This is a work in progress 8 voice synthesizer using referances from dzlonline the_synth code and referances from gligli's overcycler code.

###Video Demonstration
https://www.youtube.com/watch?v=YTVGzLrhA3o

The code is built for the Arduino due, thus it is built for the Atmel SAM3X line of Arm processors. Current specifications of the synth include:

* 8 voices with two oscillators per voice
* 8-bit Sample playback
* 32,000Hz sample rate
* Four stage ADSR envelope
* Lfo with a choice of sine, square, saw and Triangle waveforms
* Analog filter and VCA using the shruthi filter designed by Olivier Gillet from mutable Instruments. Information can be found here: http://mutable-instruments.net/shruthi1/build/smr4mkII
* Midi support is currently done through the arduino_midi_library found here: https://github.com/FortySevenEffects/arduino_midi_library

Currently the TLC7528 8-bit parallel DAC is being used to transfer data into analog voltages, but any equivelant DAC will work. The Wave data is written to the first 8 bytes of the REG_PIOD_ODSR register while analog voltages for the filter and VCA are written to bytes 1-9 of register REG_PIOC_ODSR.

This code is currently a work in progresses so it is not a gurentee that everything will be in working order.
