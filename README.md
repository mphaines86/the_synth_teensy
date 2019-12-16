# the_synth_due

This is a work in progress 8 voice synthesizer using referances from dzlonline the_synth code and referances from gligli's overcycler code.

### Video Demonstration
https://www.youtube.com/watch?v=YTVGzLrhA3o

## Technical Details
The code is built for the Teensy 3.6, thus it is built for the K66 family of processors. Current specifications of the synth include:

* 8 voices with two oscillators per voice (Changeable using SYNTH_VOICE_COUNT in synth.h)
* 8-bit Sample playback
* 32,000Hz sample rate (Changeable at FS in synth.h)
* 3 Four stage ADSR envelope
* 1 Ramp generator
* 2 LFOs with a choice of sine, square, saw and Triangle waveforms
* Analog filter and VCA using the shruthi filter designed by Olivier Gillet from mutable Instruments. Information can be found here: http://mutable-instruments.net/shruthi1/build/smr4mkII
* Midi support is currently done through the arduino_midi_library found here: https://github.com/FortySevenEffects/arduino_midi_library

This code is currently a work in progresses so it is not a gurentee that everything will be in working order.

## Referances
Below are all links to resources/codes that I have referanced through out the project.
  
<http://mutable-instruments.net/shruthi1/build/smr4mkII>

<https://github.com/FortySevenEffects/arduino_midi_library>

<https://github.com/gligli/overcycler>

<https://github.com/dzlonline/the_synth>

<https://github.com/michaelfolz/LCD_HD44780>

<https://github.com/nox771/i2c_t3>
