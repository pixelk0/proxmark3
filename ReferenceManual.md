# Reference Manual #

This reference manual is valid for the current SVN version of the firmware.




---


## Utility Commands ##

### tune ###

Measure antenna tuning

This command can be issued to verify that the antennas work properly. Run tune and wait about 8 seconds for the sweep to complete and the output values to be printed. The typical output of the 'tune' command should return values in this region:

```
# LF antenna @  33.97 V @   125.00Khz
# LF antenna @  22.29 V @   134.00Khz
# LF optimal @  33.97 V @   125.00Khz
# HF antenna @  14.27 V @    13.56Mhz
```

Read the section on antenna building for more details on how to use this command.

For a graphical view of the results issue the plot command. The output shows the voltage measured on the antennas at the most commonly used frequencies 125/134Khz and 13.56Mhz. It also shows where the LF antenna peak voltage occured (LF optimal value) which is where the LF antenna peak resonance is. This command can be used to check whether the LF antenna is properly tuned to the frequency you expect, typically a LF antenna should be tuned to 129Khz for best results with both 125Khz and 134Khz tags.

### detectreader `[l|h]` ###

Detect external reader field (option 'l' or 'h' to limit to LF or HF)

This just listens for an external reader field and lights up green for HF and/or red for LF. Button press exits. Output columns are :

PREVIOUS, CURRENT, LOOPS

Where PREVIOUS is the ADC value we just changed from, CURRENT is the value we changed to, and LOOPS is the number of times we went around the loop between changes. It would obviously be more helpful to have an actual time, but since the resolution proved not to be high enough to be able to measure reader commands (which is why I started on this in the first place !) I didn't bother...

However, it proved to be a useful little feature when you're wondering what that reader on the wall is actually listening for...

Here it is in action :

```
proxmark3> detectreader
> detectreader
#db# LF 125/134 Baseline:
#db# 00000000, 00000000, 00000000

#db# HF 13.56 Baseline:
#db# 00000000, 00000000, 00000000

#db# HF 13.56 Field Change:
#db# 00000000, 0000000b, 0000c899

#db# HF 13.56 Field Change:
#db# 0000000b, 00000000, 00000001

#db# HF 13.56 Field Change:
#db# 00000000, 00000037, 00000001

#db# HF 13.56 Field Change:
#db# 00000037, 00000053, 00000001

#db# LF 125/134 Field Change:
#db# 00000000, 00000018, 000198f9

#db# LF 125/134 Field Change:
#db# 00000018, 0000000d, 00000003

#db# LF 125/134 Field Change:
#db# 0000000d, 00000018, 0000775f

#db# LF 125/134 Field Change:
#db# 00000018, 00000024, 00000001

#db# LF 125/134 Field Change:
#db# 00000024, 0000003e, 00000001

#db# LF 125/134 Field Change:
#db# 0000003e, 00000082, 00000001

#db# LF 125/134 Field Change:
#db# 00000082, 0000003f, 00000001

#db# LF 125/134 Field Change:
#db# 0000003f, 0000000e, 00000001
```
It's also great for seeing how far out the reader field extends (or how sensitive your antenna is)...

### fpgaoff ###

Set FPGA off, meaning all signal lines are driven low. In this mode the FPGA does not modulate the antenna or route any inbound signals. This does not turn off power to the FPGA.

### lcdreset ###

Hardware reset LCD by toggling it's RESET line.

### lcd ###

Send command/data to LCD

### reset ###

Resets the board

### quit ###

Quit program


---


## Low Frequency Commands ##

### losim ###

Simulate a LF tag

This command should only be issued after acquiring, downloading and interpreting LF samples : it simulates a LF tag using the samples previously acquired.

More precisely, the trace buffer should contain only zeroes and ones, which will be interpreted by the Proxmark3 as field modulation commands ("open" and "short" coil). This means you need to interpret the raw bitstream using commands such as indalademod, askdemod and others to create a meaningful modulation bitstream.

### loread `[h]` ###

Read (125/134 kHz) LF ID-only tag

loread reads by default on 125kHz. In order to tune the Proxmark to 134kHz, issue loread with the "h" argument : loread h

### tidemod ###

Demod raw bits for TI-type LF tag

Demodulate the data stream retrieved by hisampless following a tiread. Performs some checks on the recovered data and prints relevant info. Use the plot command to see a graphical output of the demodulated waveform and the detected bit boundaries.

### tiread ###

Read a TI-type 134 kHz tag and store the raw waveform in the buffer.

This tag sends a stream of 128 bits as an FM (frequency modulated) waveform. A zero consists of 16 cycles at 123.2Khz and a one consists of 16 cycles at 134.2Khz meaning the duration of ones and zeroes is slightly different therefore the bit boundaries are not at regular intervals.

Once the tag is read,  the waveform stored in the buffer is demodulated into a set of three hex numbers (high 32 bits, low 32 bits and crc). Some checks are performed on the recovered data and relevant info printed. The TI tag stores 64 bits of data and a 16 bit crc. On the rewriteable TI tags, the crc written to the tag doesn't have to be valid, therefore 64+16=80 bits of data can be written.

At the completion of this command, the raw waveform is available in the buffer and can be retrieved by hisampless 40000.

### tiwrite hi lo `[crc]` ###

Writes data to a TI tag

Takes two hex parameters that form the 64 bit data to be written. Optionally a hex crc can be provided. If no crc is provided, one will be computed before writing. If a crc is provided, it will be written verbatim, even if it's wrong. This allows one to write up to 80 arbitrary bits to the tag (64 data + 16 crc). Once the write is finished, the tag immediately transmits the data written back, so this command reads and stores the raw waveform in the buffer.

Example:

```
> tiwrite 0xfeedbeef 0xdeadfade
#db# Writing the following data to tag:
#db# feedbeef, deadfade, 00004bc8
#db# Now use tiread to check
> tiread
#db# Info: TI tag is rewriteable
#db# Info: TI tag ident is valid
#db# Info: Tag data_hi, data_lo, crc = 
#db# feedbeef, deadfade, 00004bc8

#db# Info: CRC is good
```

Alternatively if you want to decode the tag on the client side (PC):

```
> tiwrite 0xfeedbeef 0xdeadfade
#db# Writing the following data to tag:
#db# feedbeef, deadfade, 00004bc8
#db# Now use tiread to check
> hisampless 40000
> tidemod
actual data bits start at sample 5851
length 260/238
Info: raw tag bits = 1.1111.11.1.111111.11.1.1.1111.111111.111.11111.11.11.111.1111111...1..1111.1..1..1111111.1111.11.1.1111
Info: Rewriteable TI tag detected.
Info: Tag data = FEEDBEEFDEADFADE
Info: CRC 4BC8 is good
```

### vchdemod ###

Demod samples for VeriChip

### flexdemod ###

Demod samples for FlexPass

### hiddemod ###

HID Prox Card II (not optimal)

### hidfskdemod ###

HID FSK demodulator, it loops continuously (or until the button is pressed) and if a HID tag enters the field it will capture and do the FSK demodulation and manchester decoding of the waveform to return the tag ID as a hex number. It seems the TAG ID is a 44 bit number and the number printed on the tag is usually bits 16 though 1 (bit 0 is possibly a parity bit)

### hidsimtag ###

HID tag simulator, it will take a hex number (tag ID) as a parameter and create the proper FSK manchester encoded waveform and continuously broadcast it. This could in principle be modified to run a loop and broadcast several IDs in sequence if one wanted to search for a hit in a certain range.

### indalademod ###

Indala card demodulator

indalademod was written to demodulate Indala LF cards. It should be issued after a "loread" and "losamples" command. Issue losamples with enough samples in argument, like 4000 or 6000. This command takes as argument either "64" or "224" depending on the number of bits expected in the UID.

After demodulation, the prox buffer is left in a state where the losim command can be issued to simulate the tag.

```
proxmark3> loread
> loread
#db# 0000008f, 0000005c, 00000000
proxmark3> losamples 6000
> losamples 6000
proxmark3> indalademod 64
> indalademod 64
Expecting a bit less than 750 raw bits
Recovered 749 raw bits
worst metric (0=best..7=worst): 6 at pos 44
UID=0000000000000000000000000000010100001000000011000000000010010101
Occurences: 11 (expected 11)
proxmark3>
```

### em410xread `[clock]` ###

Reads the ID from an EM410x tag (the plot should contain the raw tag).  Clock is optional, usually has a value of 64.

Example :

```
> loread
> losamples 2000
> em410xread
Auto-detected clock rate: 64
EM410x Tag ID: 1a0041375d
```

### em410xsim `[tag id]` ###

Simulates an EM410x tag with the specified ID. Example :

```
> em410xsim 1a0041375d
(The orange light will turn on. Press the button on the PM3 to stop emulating the tag.)
```

### em410xwatch ###

Watches for an EM410x tag until it detects one. Essentially "loread + losamples 2000 + em410xread" until a tag is detected. Example :

```
> em410xwatch
Auto-detected clock rate: 64
EM410x Tag ID: 1a0041375d
```


### em4x50read ###

Reads the transmitted data of an EM4x50 tag, the format is slightly different from EM410x tags. To be executed after a "loread + losamples 4000" (or losamples 6000 etc...).


### manmod `[clock]` ###

This will Manchester modulate the graphed bitstream. It's a helper function for em410xsim but can be re-used.

### detectclock ###

This function will auto-detect the clock rate.

Example :

```
> detectclock
Auto-detected clock rate: 64
```

### bitstream `[clock]` ###

Converts a waveform into a bitstream. The number of "samples" is still the same but it's easier to read a stream. I've only tested this using EM4102 tags.


---


## LF Signal analysis commands ##

### fskdemod ###

This is another way to demodulate HID tags inspired by tidemod. It acts on a raw FSK waveform as captured by loread or loaded from a saved capture. It uses the same principles of signal processing as tibits to demodulate the FSK waveform. It then goes on to detect the start condition and inserts markers around the detected bits to visually aid the user in detecting bit boundaries. Finally it manchester decodes (using a method different from the one implemented in hidfskdemod) the bitstream and prints out the output to the text window as binary and hex.

### askdemod ###

Generic ASK demodulation routine

askdemod takes one argument : the modulation convention (high mod is 1 or high mod is zero).

This command demodulates the stream into a binary stream into the trace buffer (0's and 1's).

askdemod can be used for analysis of simple LF tags which just modulate the field without any phase or frequency shifts. A complete example of using this command can be found at the page on LF tag operations.

### mandemod ###

Generic Manchester demodulation routine

mandemod takes a binary stream from the trace buffer (see askdemod) and attempts to do manchester decoding to it. One argument : clock rate. Outputs the bitstream to the scrollback buffer. If the clock rate is not given, it is autodetected.

See the page on LF tag operations for a practical example.

The mandemod was recently updated and supports two algorithms :

> You can either use a raw capture buffer (issued straight from loread then losamples), and the mandemod then accepts another argument, "i", in order to decode using a positive or negative convention.
> Or you can first demodulate the feed using askdemod (or another demodulation function the day other functions become available), and a slower but more robust algorithm decodes the tag. You should not need the "i" argument in that case, because the routine automatically resyncs if it detects it is using the wrong convention.


---


## ISO-15693 commands ##

### hi15read ###

Read HF tag (ISO 15693)

This command starts to read an ISO 15693 tag. It sends an "identify" request, then waits for the response. The response is not demodulated, just left in the buffer so that it can be downloaded to a PC and processed there, using the hisamples command.

### hi15reader ###

New in 20090301

Act like an ISO15693 reader

This command sends a few 15693 commands to a tag and displays the answers. More a proof of concept than an actual reader implementation, but interesting nonetheless : it is actually very close to the hi15read command, but all demodulation is done on the Proxmark3 itself.

In the example below, a ski pass is read using this command. The same ski pass is used in the example of the hi15demod command and the results are the same, of course !

```
proxmark3> hi15reader
> hi15reader
#db# 12 octets read from IDENTIFY request
#db# 00000000, 00000001, 000000fd

#db# 000000f1, 00000082, 00000012

#db# 00000000, 00000000, 00000007

#db# 000000e0, 0000007d, 000000da

#db# 0 octets read from SELECT request
#db# 0 octets read from XXX request
proxmark3>
```

### hi15sim ###

New in 20090301

ISO15693 tag simulator. Tagged as 'not working too well', and indeed I was not able to make my Omnikey reader react to it at all...

### hi15demod ###

Demod ISO15693 from tag

This command should be issued after downloading HF samples through the hisamples command. It tries to demodulate ISO15693 frames found in those samples, as shown in the example below :

Example :

```
proxmark3> hi15read
> hi15read
proxmark3> hisamples
> hisamples
proxmark3> hi15demod
> hi15demod
SOF at 7, correlation 52
EOF at 407
12 octets
#  0: 00
#  1: 01
#  2: fd
#  3: f1
#  4: 82
#  5: 12
#  6: 00
#  7: 00
#  8: 07
#  9: e0
# 10: 7d
# 11: da
CRC=da7d
proxmark3>
```


---


## ISO-14443-B commands ##

### hi14read ###

Reads ISO 14443-B tag

This command sends the following ATQB command : 0x05, 0x00, 0x08, 0x39, 0x73 , in ISO14443-B mode to a tag, and records the tag's answer.

The second value of the output can be either 0x00000000 or 0x00000001 : if it is "1", this means the reply of the tag was received properly. If it is zero, it means not all bytes (or none) were received. In both cases, the answer should be read using the hexsamples command.

### hi14bdemod ###

Demod ISO14443 Type B from tag

### hisimlisten ###

Get HF samples as fake tag

### hi14sim ###

Fake ISO 14443 tag

### hi14snoop ###

Eavesdrop ISO14443-B

This command is equivalent to hi14asnoop, but in B mode : it listens to traffic between the tag and the reader until canceled with the button. In current firmware versions, the Proxmark3 does have a tendancy to crash if acquisition is run for too long, unfortunately.

LED Behaviour : when the command is issued, the yellow LED turns on. As soon as a ISO14443-B modulated field is detected, the yellow LED turns off, and the green LED turns on.

The example below shows an old ePassport chip being detected and read in ISO14443-B mode, using an Omnikey 5321 reader : you can see the reader issuing an "ATQB" command (05 00 00 71 FF) and the tag answering :

```
proxmark3> hi14snoop
> hi14snoop
#db# cancelled
proxmark3> hi14list
> hi14list
recorded activity:
time        :rssi: who bytes
---------+----+----+-----------
+      0:  77: TAG 50 92 08 05 0f 91 f3 5e 11 b3 81 a1 37 2f
+   1166:    :     1d 92 08 05 0f 00 a5 01 01 f8 46
+   6418:  85: TAG 01 f1 e1
+ 677656:    :     05 00 00 71 ff
+    824:  93: TAG 50 92 08 05 0f 91 f3 5e 11 b3 81 a1 37 2f
+   1188:    :     1d 92 08 05 0f 00 a5 01 01 f8 46
+   6408:  86: TAG 01 f1 e1
+ 178038:    :     05 00 00 71 ff
+  77124:    :     06 00 97 5b
+ 113552:    :     05 00 00 71 ff
+  76286:    :     06 00 97 5b
+ 114412:    :     05 00 00 71 ff
+  76282:    :     06 00 97 5b
+ 109716:    :     05 00 00 71 ff
+  76696:    :     06 00 97 5b
+ 114028:    :     05 00 00 71 ff
+    822: 156: TAG 50 92 08 05 0f 91 f3 5e 11 b3 81 a1 37 2f
+   1166:    :     1d 92 08 05 0f 00 a5 01 01 f8 46
+   6422: 181: TAG 01 f1 e1
+ 814936:    :     05 00 00 71 ff
+  74604:    :     06 00 97 5b
+ 114392:    :     05 00 00 71 ff
+  76706:    :     06 00 97 5b
+ 113988:    :     05 00 00 71 ff
+  76244:    :     06 00 97 5b
+ 114436:    :     05 00 00 71 ff
+    822: 136: TAG 50 92 08 05 0f 91 f3 5e 11 b3 81 a1 37 2f
+   1178:    :     1d 92 08 05 0f 00 a5 01 01 f8 46
+   6366: 160: TAG 01 f1 e1
+1140613746:    :     44 44 44 44 44 44 44 44 44 44 44 44 44 44 44 44
                      44 44 44 44 44 44 44 44 44 44 44 44 44 44 44 44
                      44 44 44 44 44 44 44 44 44 44 44 44 44 44 44 44
                      44 44 44 44 44 44 44 44 44 44 44 44 44 44 44 44
                      44 44 44 44   **FAIL CRC**
proxmark3>
```

### hi14list ###

List ISO 14443 history

See the command above for an example !

### sri512read ###

Read the contents of ST Micro SRI memory tags

Those tags are simple memory tags from ST, which use the -B modulation. This command gets the tag's UID and the 16 memory blocks + system block.

Below is an example : the proxmark actually gets the CRC wrong at the last command, though the contents are right.

```
proxmark3> tune
> tune
# LF antenna @   0 mA /   134 mV [1273 ohms] 125Khz
# LF antenna @   0 mA /   134 mV [1187 ohms] 134Khz
# HF antenna @  23 mA /  5607 mV [235 ohms] 13.56Mhz
proxmark3> sri512read
> sri512read
#db# Randomly generated UID from tag (+ 2 byte CRC):
#db# 000000e1, 000000ff, 00000006

#db# Now SELECT tag:
#db# Tag UID (64 bits):
#db# d00218bf, 9c561b37, 00000000

#db# Tag memory dump, block 0 to 15
#db# Address , Contents, CRC
#db# 00000000, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000001, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000002, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000003, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000004, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000005, fffffffe, 0000fc13

#db# Address , Contents, CRC
#db# 00000006, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000007, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000008, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 00000009, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 0000000a, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 0000000b, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 0000000c, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 0000000d, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 0000000e, ffffffff, 0000470f

#db# Address , Contents, CRC
#db# 0000000f, ffffffff, 0000470f

#db# System area block (0xff):
#db# CRC Error reading block! - Below: expected, got
#db# 0000ab03, 0000eb03, 00000000

#db# Address , Contents, CRC
#db# 000000ff, ffff7fff, 0000eb03
```


---


## ISO-14443-A commands ##

### hi14areader ###

Act like a ISO-14443-A reader. The Proxmark generates a field and uses 100% ASK and Modified Miller encoding to communicate with a card or tag. The answers of the card are stored in a buffer (BigBuf) on the Proxmark and can be downloaded by hi14alist.

Technically, the Proxmark drives the antenna coils to generate a field. The field is removed with certain intervals of 2�s to communicate with a card. The Proxmark switches between sending and listening. Only the anticollision phase is executed and only the messages from the card (TAG) are stored.

New in 20090301 : hi14areader now supports Cascade L1 and L2 for double and triple length UIDs.

hi14asim : fake ISO 14443a tag

The command hi14asim sets the Proxmark in emulation mode. Until the button is pressed the device will respond as programmed in the firmware. We programmed the Proxmark to act like a mifare Classic 4k card. Contactless readers (such as the Omnikey 5121) run the anticollision and get convinced that they communicate with an mifare Standard 4k card. They also detect the (simulated) card's UID.

### hi14asnoop ###

Sniff the communication between a reader and a tag. The communication from both directions is captured and stored in a buffer (BigBuf) on the Proxmark and can be downloaded by hi14alist.

By default, the Proxmark waits for communication to occur which means that it will 'hang' forever if nothing happens. You can press the button on the device to stop acquisition and get back to the command line.

The command hi14asnoop starts sniffing the communication between a reader and card until the buffer (BigBuf) is full. Below is the result of the sniffing captured by executing hi14alist. The result shows the repeated anticollision loop. This indicates that the reader is just polling if the card is still there. This anticollision loop walks through the following states :

> Reader : 26 -> Card : 04 00
> The reader sends an REQA (Request for type A) message on which the card responded with its type.

> Reader : 93 20 <- Card : 2a 69 8c 43 8c
> The reader starts the anticollision to select a card and requests its UID. The card responded with its UID. The last byte is the so-called BCC18 and is the result of XOR-ing the first four UID-bytes.

> Reader : 93 70 2a 69 8c 43 8c -> Card : 08 b6 dd
> The reader selects the desired card by sending 93 70 followed by the UID of the card. If the card is successfully selected it will response with a SAK19 which means that the card is now ready to handle commands of the higher layer protocol. The SAK consists of one byte (08) that indicates the card type and is followed by two CRC bytes.

### hi14alist ###

List ISO 14443a history

With this command any data captured by hi14areader or hi14asnoop can be downloaded and displayed in the Windows client.

The listing indicates time units in terms of "Proxmark ticks", i.e. the internal unit used in the FPGA to measure timing. In the case of iso14443A, this is 1/8th of a bit period, the bit period length being 9.44�s.


---


## Raw LF/HF commands ##

### losamples ###

Get raw samples for LF tag

By default, only download the first 128 samples. Issue "losamples" with the number of samples in argument for a different value, for example losamples 16000. The HID USB interface being slow, this takes a long time. The number of samples can be any value between 0 and 16000.

### hisamples ###

Get raw samples for HF tag

### hisampless ###

Get signed raw samples, HF tag

### hisamplest ###

Get samples HF, for testing

### higet ###

Get samples HF, 'analog'

### bitsamples ###

Get raw samples as bitstring

### hexsamples ###

Dump big buffer as hex bytes

Used in particular for ISO14443-B reading commands.


---


## Graph commands ##

### plot ###

Shows the graph window. The graph window is automatically updated when new samples are downloaded or operations are done on the trace.

When in the graph window, the following commands are available :

  * Up arrow : zoom out
  * Down arrow : zoom in
  * Left/Right arrow : move trace left/right
  * Left click : set yellow cursor
  * Right click : set purple cursor

### hide ###

Hides the graph window

### grid x y ###

This overlays a faint grid on the plot window with the x and y spacing specified. Use positive values. You can turn either or both grids back off by specifiyng a zero value.

### autocorr ###

Autocorrelation over window

### norm ###

Normalize max/min to +/-500

### dec ###

Decimate

This command removes every other sample from the trace buffer : in practice, this means it divides the sampling rate by two.

dec exists partly for historical purposes : commands such as flexdemod and vchdemod were written for the Prox II, which had a sampling rate of half of the Prox3 sampling rate. For this reason, in order to get actual meaningful results from those commands, you should "decimate" the trace before issuing them.

The effect of dec can clearly be seen on the example below, with an Indala tag :

Indala "loread" acquisition

![http://proxmark3.googlecode.com/svn/wiki/Indala-initial.png](http://proxmark3.googlecode.com/svn/wiki/Indala-initial.png)

Same trace, decimated once

![http://proxmark3.googlecode.com/svn/wiki/Indala-dec.png](http://proxmark3.googlecode.com/svn/wiki/Indala-dec.png)

### hpf ###

Remove DC offset from trace

### zerocrossings ###

Count time between zero-crossings

### ltrim ###

Trim from left of trace

### threshold val ###

The number val can be positive or negative. The command runs through the graph buffer and replaces every sample value >= val with 1 and every other value with -1. This might be useful sometimes if you want to take an analog waveform and turn it into a clean set of highs and lows.

### scale ###

Set cursor display scale. Setting the scale makes the differential ("dt") reading between the yellow and purple markers meaningful. Once the scale is set, the differential reading between brackets is the time duration in seconds.

The argument to the scale command is the carrier frequency, expressed in kHz. For example, if acquiring in 125kHz, use scale 125.

The example below shows for example that one of the FSK frequencies of a HID Prox tag is 15.625kHz (i.e. a 0.064s period).

![http://proxmark3.googlecode.com/svn/wiki/scale-1.png](http://proxmark3.googlecode.com/svn/wiki/scale-1.png)

### save ###

Save trace (from graph window)

### load ###

Load trace (to graph window)