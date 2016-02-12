# the_synth_due

This is a work in progress 8 voice synthesizer derived from dzlonline original the_synth code which utilizes the greater power of the arduino due.

There are some noticable differances from the original code developed by dzlonline which currently include:

* 8 voices instead of 4
* 32,000Hz instead of 20,000Hz sample rate
* Different modulation engine
* direct control over voice amplitude
* Use of an 8-bit parallel DAC on the first 8 bytes of the REG_PIOD_ODSR register.
