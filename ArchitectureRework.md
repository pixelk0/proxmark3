# Introduction #

The current Proxmark3 architecture is basically at end-of-life. Adding new features that are slightly different of existing features has become a copy-paste-change exercise. In most cases, adding something to the FPGA is entirely impossible because it would break existing code at a hundred different (sometimes very similar) places. Using existing code paths outside of the very specific case for which they were created is impossible most of the time.

Changing this in a minimally intrusive and backwards-compatible way seems infeasible. A major, deliberately backwards-incompatible, but future-proof, revamp appears necessary. The end result should be a code base with much the same functionality, but for which it is radically easier to develop new functionality.

This should be guided by the following principles:
  * Re-usable functional blocks that are applicable to more than one code path
  * Future-proof interfaces wherever possible

Most of the actual code from the existing code base can be reused. For this to happen, we must first identify and document the current level of functionality in both the ARM and the FPGA code, identify the responsible routines and their interface and interaction with other code and the ARM-FPGA interface. Then the routines need to be broken down into independent building blocks, redundancies must be eliminated and the new blocks reassembled (with some new glue code and interfaces) for the same (and enhanced) outward functionality.

# Existing functionality #

## User visible ##

```
data
    amp
    askdemod
    autocorr
    bitsamples
    bitstream
    buffclear
    dec
    detectclock
    fskdemod
    grid
    hexsamples
    hide
    hpf
    load
    ltrim
    mandemod
    manmod
    norm
    plot
    samples
    save
    scale
    threshold
    zerocrossings

hf
    14a
        list
        mifare
        reader
        sim
        snoop
    14b
        demod
        list
        read
        sim
        simlisten
        snoop
        sri512read
        srix4kread
    15
        demod
        read
        reader
        sim
    legic
        decode
        reader
        save
        load
        sim
        write
        fill
    tune
hw
    detectreader
    fpgaoff
    lcd
    lcdreset
    readmem
    reset
    setlfdivisor
    setmux
    tune
    version
lf
    cmdread
    em4x
    flexdemod
    hid
    indalademod
    read
    sim
    simbidir
    simman
    ti
    vchdemod
```

### data amp ###
"Amplify peaks"
#client-internal #graphbuffer-extrema

Goes through graph buffer, finds global minimum and maximum values, sets points where the sign of the slope changes (e.g. local minima/maxima) to global minimum/maximum.

### data askdemod ###
"Attempt to demodulate simple ASK tags"
#client-internal #graphbuffer-extrema

Goes through graph buffer, finds global minimum and maximum values, sets extrema to either 0 or 1 (mapping configurable, either maximum == 1 and minimum == 0 or vice versa), sets points between extrema to 0 or 1 based on the last extremum.

### data autocorr ###
"Autocorrelation over window"
#client-internal

Replaces graph buffer with the #autocorrelation result (scaled by 1/256) over a window size. The size of the resulting graph buffer is len(original graph buffer) - window size.

#warning Fixed buffer on stack of maximal graph buffer size. Should be converted to dynamic buffer on heap, or even better: perform replacement in-place.

### data bitsamples ###
"Get raw samples as bitstring"

Sends #CMD\_DOWNLOAD\_RAW\_ADC\_SAMPLES\_125K, waits for #CMD\_DOWNLOADED\_RAW\_ADC\_SAMPLES\_125K, sets graph buffer to 1-bit waveform represented by the received data contents (MSBit first). Downloads 3072 4-byte words in 12 4-byte words each for a total of 12288 bytes or 98304 values in the resulting graph buffer.

### data bitstream ###
"Convert waveform into a bitstream"
#client-internal #graphbuffer-extrema

Calls #GetClock which might call #DetectClock to get a clock rate. Separates the graph buffer into sections of length len(original graph buffer) / clock length. In each section, checks for samples that match the global maxima and minima (except for the first section). The graph buffer is replaced with a new buffer (inplace) that consists of either 0 or 1 values, one for each section. The value is carried over from the result for the last section, if both maximum and minimum were matched, otherwise it is the inverse of the result for the last section.

This appears to be a (very strange) way to decode #manchester encoding, when given the exact clock rate and with no clock drift.

### data buffclear ###
"Clear sample buffer and graph window"

Sends #CMD\_BUFF\_CLEAR and clears graph buffer.

### data dec ###
"Decimate samples"
#client-internal

Replaces the graph buffer with a version that only contains every second sample (inline).

### data detectclock ###
"Detect clock rate"
#client-internal

Calls #DetectClock and prints the result.

### data fskdemod ###
"Demodulate graph window as a HID FSK"
#client-internal

Replaces the graph buffer with the output of a complex calculation that seems to contain #crosscorrelation and #manchester decoding.

### data grid ###
"overlay grid on graph window, use zero value to turn off either"
#client-internal

Changes the values of the global variables that contain the grid step parameters.

### data hexsamples ###
"Dump big buffer as hex bytes"

Sends #CMD\_DOWNLOAD\_RAW\_ADC\_SAMPLES\_125K and waits for #CMD\_DOWNLOADED\_RAW\_ADC\_SAMPLES\_125K and then prints a #hexdump of the received response.

### data hide ###
"Hide graph window"

Call the function to hide the graph window.

### data hpf ###
"Remove DC offset from trace"
#client-internal

Calculates the #mean of the graph buffer (except for the first 10 samples) and then subtracts the result from all values in the graph buffer.

#warning The mean calculation magically excludes the first 10 samples (and will do funny things when there are less than 10 samples in the buffer). Also the intermediate value is stored in an int which might overflow.

### data load ###
"Load trace (to graph window"
#client-internal

Opens a file, interprets each line as an integer, and replaces the graph buffer contents with the results.

### data ltrim ###
"Trim samples from left of trace"
#client-internal

Accepts an offset parameter and replaces graph buffer content at position i with position i+offset.

### data mandemod ###
"Manchester demodulate binary stream (option 'i' to invert output)"
#client-internal #graphbuffer-extrema

Calculates graph buffer extrema and possibly #GetClock to calculate clock rate. Iterates through graph buffer to find the first value that matches the global minimum which appears after the first value that matches the global maximum.

#copypaste "data bitstream" If the global maximum does not equal 1, a virtually identical copy of the code from the "data bitstream" command is executed.

If the global maximum does equal 1, a more generic #manchester demodulation scheme is used which allows for a slight fuzzyness in edge timings.

In either case, the local temporary buffer now contains a straight 1-bit waveform, with 2 samples per bit period. This is then collapsed into a bit string with 1 sample per bit period (with some allowance for resynchronization). The result is printed as a #bindump on the screen (might consider adding a #hexdump in the future?).

#warning Fixed buffer on stack of maximal graph buffer size.

### data manmod ###
"Manchester modulate a binary stream"
#client-internal

Calls #GetClock and then separates the graph buffer into sections of clock length samples. For each section a value is calculated that is based on the first value in that section. Then the entire section is replaced with a square wave with 50% duty cycle whose first half is the value for that section xor some values from the last section, and the second half is the value for that section xor some values from the last section xor 1.

This appears to be supposed to do #manchester encoding, but the code flow (and meaning of variable wave) is not clear.

#warning Behaviour when the graph buffer contains values other than 0 or 1 is probably not what is expected.

### data norm ###
"Normalize max/min to +/-500"
#client-internal #graphbuffer-extrema

Calculates the minimum and maximum values in the graph buffer (except for the first 10 samples), and if these are not equal, replaces the graph buffer contents with scaled values so that the minimum is -500 and the maximum is +500.

### data plot ###
"Show graph window"
#client-internal

Calls ShowGraphWindow.

### data samples ###
"Get raw samples for graph window"

If the argument is 0, it is set to 128. Additionally it's clamped to 16000. For every 12 in the argument, sends one #CMD\_DOWNLOAD\_RAW\_ADC\_SAMPLES\_125K and waits for #CMD\_DOWNLOADED\_RAW\_ADC\_SAMPLES\_125K and then appends 48 values from the received packet to the graph buffer (which is truncated beforehand).

In effect this downloads 4 **argument samples (e.g. argument 32-bit words) from the device in packets of 12 words each and replaces the graph buffer with this data.**

### data save ###
"Save trace (from graph window)"
#client-internal

Iterates over the graph buffer and prints each value as an integer string representation to a file.

### data scale ###
"Set cursor display scale"
#client-internal

Sets the global variable for the cursor scale factor, whatever that may be.

### data threshold ###
"Maximize/minimize every value in the graph window depending on threshold"
#client-internal

Iterates over the graph buffer and sets every value that is greater than or equal to the argument to 1, and all other values to -1.

### data zerocrossings ###
"Count time between zero-crossings"
#client-internal

Iterates over the graph buffer and finds points where the sign changes. The values between two sign change points are replaced with the number of samples between the first sign change point of the two and the sign change point before it.

### hf 14a list ###
### hf 14a mifare ###
### hf 14a reader ###
### hf 14a sim ###
### hf 14a snoop ###
### hf 14b demod ###
### hf 14b list ###
### hf 14b read ###
### hf 14b sim ###
### hf 14b simlisten ###
### hf 14b snoop ###
### hf 14b sri512read ###
### hf 14b srix4kread ###
### hf 15 demod ###
### hf 15 read ###
### hf 15 reader ###
### hf 15 sim ###
### hf legic decode ###
### hf legic reader ###
### hf legic save ###
### hf legic load ###
### hf legic sim ###
### hf legic write ###
### hf legic fill ###
### hf tune ###
### hw detectreader ###
### hw fpgaoff ###
### hw lcd ###
### hw lcdreset ###
### hw readmem ###
### hw reset ###
### hw setlfdivisor ###
### hw setmux ###
### hw tune ###
### hw version ###
### lf cmdread ###
### lf em4x ###
### lf flexdemod ###
### lf hid ###
### lf indalademod ###
### lf read ###
### lf sim ###
### lf simbidir ###
### lf simman ###
### lf ti ###
### lf vchdemod ###


## Internal service routines ##

## Provided by the FPGA ##

# Interdependencies #

How do the existing blocks connect to each other?

# New functional blocks #

What independent functional blocks are going to be necessary. (Optional: How much of the implementation exists already and can be carried over.)

## FPGA ##
Basic FPGA functionality:
  * Passtrough (LF, HF)
  * Carrier generation (LF, HF)
  * RX: Hysteresis, configurable sampling intervals (LF, HF)
  * RX: I/Q-Correlator with configurable subcarrier frequencies, configurable sampling intervals (HF) (FIXME: LF?)
  * TX: AM from bitbanging or SSC data on configurable clock (LF, HF)
  * TX: FIXME, what else? PSK? FSK?

It should be possible to arbitrarily combine all of these as far as possible. Remember: We don't want a ISO-14443A-Thingamajig. We want a "13.56MHz carrier generator with bit-banged 100% AM on the transmission side and an I/Q-correlator on 848kHz with 106k complex samples per second on the RX side". Which can turn into "no TX side, I/Q-correlator on 848kHz with 106k complex samples per second on the RX side and hysteresis on the RX side" while sharing most of the code paths.

## Internal service routines ##

  * Generic CRC calculation
  * Manipulation of radio frames in memory with methods such as append\_bit or append\_byte
  * Generic encoding and decoding of most codes:
    * modified Miller
    * Manchester