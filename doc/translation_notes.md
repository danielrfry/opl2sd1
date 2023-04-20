# Notes on the OPL to SD-1 translation

The SD-1 has a number of significant differences to the OPL series that prevent a direct 1:1 mapping of registers:

## 1. Voices

The OPL3 has up to 18 2-operator voices, or up to 6 4-operator voices. Two 2-op voices are combined to make each 4-op voice. Most tunes will use 16 voices or fewer, but may still play sounds on voices 17 and 18. Therefore, the translator allocates SD-1 voices dynamically to OPL3 voices as required. In the event that all 18 2-op voices are used, the least recently allocated voice is re-assigned and some notes may be cut off.

## 2. Register layout

The OPL3 registers contain a bank of FM parameters for each voice, and the sound playing on a particular voice can be modified simply by writing to the registers.

On the SD-1, FM parameters are not associated directly with any particular voice. Instead, there is an array of 16 banks of tone data that must be written to sequentially, and a tone within this array is selected when a note is played.

As the SD-1 has 16 voices and 16 tones, the translator simply reserves a tone for each voice. However, the SD-1 tone data must still be written sequentially starting at the first position in the array - therefore the OPL registers are cached in memory, and all tones converted and re-written up to the location of the changed tone, any time an OPL register changes.

## 3. Sample rate

The SD-1 uses a sample rate of 48000Hz, compared to the OPL3's sample rate of 49716Hz. As a result, by default the same block and frequency numbers produce a lower pitched note on the SD-1. The translator sets the global frequency multiplier register of the SD-1 to compensate for this difference.

## 4. Envelope generator

The OPL3 has an 'envelope type' parameter that determines whether a note is held at its sustain level while the key is on, or whether it transitions immediately from the decay to release phase. The SD-1 instead has a sustain rate parameter that determines the rate at which the level is reduced during the sustain phase. The translator sets the sustain rate either to the release rate or 0 according to the value of the envelope type parameter, to mimic the OPL behaviour.
