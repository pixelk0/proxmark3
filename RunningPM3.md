# Running the PM3 #




---


## Using the Proxmark3 ##
In this section we will discuss the various practical aspects of running the Proxmark3 in day to day operations.

## Is it working ? ##

In order to check that your Proxmark3 is working, plug it into a USB port using a mini-USB cable, and observe the LED boot sequence :

With the original firmware, the boot sequence is as follows :

  * All LEDs on
  * Yellow only
  * Yellow and green
  * Red only
  * All LEDs off

With the 20081211 version, here is what happens :

```
LED boot sequence now changed, C (red) lights up when boot code jumps from flash to RAM boot code, A (yellow) lights up after clocks have been initialized, B (green) lights up when jumping from boot code to main code, then D (red led away from the others) lights up while code is being downloaded to FPGA, then all leds turn off and board is ready for action.

With these changes the board now boots and is ready to use in about 3 seconds. Also since the USB bus is not initialized twice (once during boot, then again when the main code runs) unless the button is held down at boot, this seems to avoid the double USB connect and "USB device not recognized" when device is connected to the USB bus or software reset.
```

## Which firmware should I run ? ##

Basically : the original Proxmark3 firmware only supports 125kHz tags, ISO15693 and ISO14443-B modulations. The firmware written by Koning (20080514) and posted on proxmark.org adds ISO14443-A support to the Proxmark3. The latest versions add new commands related to HID tags, and also commands for supporting a LCD screen on the Proxmark3.

The biggest issue at the moment with the device, is that several people tend to work on it without a proper central source code management system : this means that some features make it into some versions, some don’t, with no exact overlap.

As of March 2009, there are several versions of the firmware which can be found on http://proxmark.org/ :

  * The original version available on the page of the Proxmark author. This version does not support 14443a
  * The updated version available on the Proxmark.org page, aka the 20080514 and 20081121 versions. This version supports 14443a. A binary version of the firmware is available on the proxmark.org files section. It does not touch the bootloader and includes both FPGA & OS images in the same .s19 file.
  * The updated version available on the same Proxmark.org page which introduces a new enhanced bootloader : 20081211. From this version on, bootloader, fpgaimage & OS can be flashed separately. Nevertheless, the linux port of the proxmark3 client will work on standard proxmark devices.
  * There is a Linux port, released as version 20090112. This version is not compatible with the Windows version because it changes the USB device descriptor. It introduces interesting UID cloning features in 14443-A in the source code
  * As or March 2009, a 20090301\_doob version was released, which includes several new ISO-15693 commands. A 20090306\_edo version was released which also includes the linux command line client but is otherwise identical to the 20090301 version.
  * In April 2009, a a new Google code project was created and is hosted on Google Code. You can check the source out on SVN, and download ready-made s19 files there too.

## How to upgrade the firmware ##

As described earlier, the Proxmark firmware comprises a bootloader, a fpga image and actual OS, i.e. the ARM code. This firmware can be upgraded through USB, this is a feature of the bootloader. Depending on the firmware version, you can either upgrade the bootloader or the fpga+os (original firmware), or upgrade bootloader, fpga image and os separately (20081211 version).

In the normal course of operations, there is no reason why you should need to use the JTAG connector that is installed on the Proxmark, unless you somehow manage to crash the bootloader itself when flashing it over USB.

Actual flashing of the proxmark involves issuing flash commands twice : the first attempt causes the device to reboot, and then a second time where actual flashing takes place :

```
#prox load osimage.s19
-- error, then the device reboots --
#prox load osimage.s19
-- actual flashing takes place
```

Note : this double command is not necessary for current versions of the firmware, as described below :

Note for 20081211 version and later : flashing can only be done with the proxmark button pressed down when the proxmark is plugged in, as described in the ’CHANGES.TXT’ file in the ’doc’ directory that comes in the 20081211 zipfile.

With this boot code, the device can now only be flashed if button is held down after power on or a software reset.

The flash procedure is this :

```
Hold down button. Either plug in USB or software reset it. _While_holding_down_button_ (red and yellow LEDs are lit) you can issue one or more of the "prox bootrom " "prox fpga " "prox load ", be sure to hold button down for the entire duration of the flash process. Only release the button when flashing is complete and you want to let the board boot.

This process may be less convenient but it’s safer and avoids "unintentional" flashing of the board.
```

### Native Linux flashing ###
As of July 2009, the SVN versions include a linux "flasher" command, which works like the windows "prox" command. Type ’flasher’ for the command line to get the list of flashing options.

### Linux & VMWare ###
Thanks to the new flashing mode (keeping the button pressed), it now is fairly simple & reliable to flash the Proxmark3 from a Windows XP guest on a Linux host using vmware : just make sure your virtual machine is configured to automatically grab USB devices when it has focus and don’t forget to keep the Proxmark3 button pressed when you connect it to the USB port.


---


## Supported tag types ##
Below is a table summarizing various tag types and operations supported on the latest SVN version of the Proxmark3 firmware : it is not limitating in any way, get back to me if you have tested other tag types

| **Manufacturer** | **Tag name** | **Frequency** | **Modulation** | **Read** | **Simulate** | **Snoop** | **Comment** |
|:-----------------|:-------------|:--------------|:---------------|:---------|:-------------|:----------|:------------|
| ST Micro         | SRI512       | 13.56MHz      | ISO14443-B     | Yes      | No           | Yes       | UID read, memory dump. |
| NXP and many others | Mifare       | 13.56MHz      | ISO14443-A     | Yes      | Yes          | Yes       | Very Limited simulation capabilities in current firmware |
| NXP              | icode SLI    | 13.56MHz      | ISO15693       | Yes      | No           | No        | Simulation code not working. Limited reading capabilities. |
| EM Microelectronics | EM Marin <br> 4200 series <table><thead><th> 125kHz        </th><th> 125kHz OOK     </th><th> Yes      </th><th> Yes          </th><th> No        </th><th> Simulation not tested, manchester encoded. <br> Exists in several variants, but I only encountered the Manchester type. </th></thead><tbody>
<tr><td> Nedap            </td><td> Nedap        </td><td> 125kHz        </td><td> 120kHz OOK     </td><td> Yes      </td><td> Yes          </td><td> No        </td><td> Simulation not tested, manchested encoded, works at 125kHz </td></tr>
<tr><td> HID (formerly Motorola) </td><td> Indala       </td><td> 125kHz        </td><td> 125kHz BPSK    </td><td> Yes      </td><td> Yes          </td><td> No        </td><td> UID is scrambled, but replay works. </td></tr>
<tr><td> HID              </td><td> HID Prox     </td><td> 125kHz        </td><td> 125kHz FSK     </td><td> Yes      </td><td> Yes          </td><td> No        </td><td> Replay works. Decoding either on the Proxmark, or in the client. </td></tr></tbody></table>

<hr />

<h2>Standalone Mode - HID Prox emulation</h2>
The HID Standalone mode is available in SVN <a href='https://code.google.com/p/proxmark3/source/detail?r=52'>revision 52</a> and above.<br>
<br>
To get into stand-alone mode (works with or without a PC), hold the button for a second. You’ll see the lights go into a synchronized little bit. When done, the red1 LED will be lit. When using a PC, debug output will be printed so you see what’s going on.<br>
<br>
When just red1 (next to the other two LEDs) is lit, that means slot 1 (red1) is selected.<br>
<br>
When just orange is lit, that means slot 2 (orange) is selected.<br>
<br>
When red2 is lit (and either red1/orange), that means the pm3 is recording and waiting for an HID tag to be detected. Once detected, the red2 light will turn off and the tag will be stored in the selected slot.<br>
<br>
When green is lit (and either red1/orange), that means that specific slot is simulating the HID tag stored on that slot.<br>
<br>
To record, hold down the button for 1 second until the red2 light comes on. This will record to the active slot (either red1 or orange).<br>
<br>
To play, just press the button and the green light comes on for the selected slot. To switch to either slot, press the button again. You may need to press twice (once to play the current slot, then to switch to the next slot).<br>
<br>
So pressing four times would do :<br>
<pre><code>-&gt; red1 (selected 1)<br>
-&gt; red1+green (playing 1)<br>
-&gt; orange (selected 2)<br>
-&gt; orange+green (playing 2)<br>
-&gt; red1 (selected 1) ...<br>
</code></pre>

<hr />

<h2>Examples</h2>

<h3>Identifying an unknown tag</h3>

This method was documented in this proxmark.org forum post.<br>
<pre><code>A really quick and easy way of determining if a card is HF or LF is to :<br>
<br>
1. Run the hw tune command a couple of times to get an idea of what your voltage readings are on LF and HF.<br>
2. place the unknown card/tag against the proxmark antennas (LF &amp; HF)<br>
3. Run the tune command a couple of times again to get an idea of what the values are wth the tag in the antenna fields<br>
4. Look for significant voltage drops in either HF or LF, the voltage drop indicates the tag operating frequency.<br>
<br>
Generally you will see a voltage drop (sometimes over 10 volts) on the corresponding frequency (e.g. LF 125kHz/134kHz, HF 13.56MHz)<br>
<br>
Some examples :<br>
<br>
Nominal Tune readings (no tags nearby)<br>
   # LF antenna @  26 mA / 33972 mV [1273 ohms] 125Khz<br>
   # LF antenna @  18 mA / 22290 mV [1187 ohms] 134Khz<br>
   # HF antenna @  60 mA / 14276 mV [235 ohms] 13.56Mhz<br>
A HID LF prox card :<br>
<br>
   # LF antenna @   8 mA / 10205 mV [1273 ohms] 125Khz      [~ 23.5 Volt drop]<br>
   # LF antenna @   9 mA / 11547 mV [1187 ohms] 134Khz      [~ 10.7 Volt drop]<br>
   # HF antenna @  60 mA / 14244 mV [235 ohms] 13.56Mhz<br>
An ISO15693 HF prox card :<br>
<br>
   # LF antenna @  26 mA / 33837 mV [1273 ohms] 125Khz<br>
   # LF antenna @  18 mA / 22290 mV [1187 ohms] 134Khz<br>
   # HF antenna @  50 mA / 11794 mV [235 ohms] 13.56Mhz    [~ 2.5 Volt drop]<br>
An ISO14443A HF tag :<br>
<br>
   # LF antenna @  26 mA / 33972 mV [1273 ohms] 125Khz<br>
   # LF antenna @  18 mA / 22155 mV [1187 ohms] 134Khz<br>
   # HF antenna @  44 mA / 10538 mV [235 ohms] 13.56Mhz   [~ 3.7 Volt drop]<br>
</code></pre>

<hr />

<h3>Cloning/replaying a HID Prox tag</h3>
From this post, you can see how you can use the HID-related commands to read & replay HID Prox tags using the commands introduced in the 20081211 firmware.<br>
<br>
In particular, this post gives a good example of a properly read tag waveform :<br>
<br>
<img src='http://proxmark3.googlecode.com/svn/wiki/waveform.jpg' />

<hr />

<h3>Get the UID of a Mifare card using ’snooping’ capabilities</h3>
Place a Mifare card on a Mifare reader (in this case, an Omnikey 5321), and put the antenna between the Mifare card and the reader.<br>
<br>
Issue the following command :<br>
<pre><code>proxmark3&gt; hf 14a snoop<br>
<br>
The Red, Yellow and Green LEDs blink with activity. After a while, once the Proxmark3 buffer is full, the "command finished" prompt is issued :<br>
#db# COMMAND FINISHED<br>
#db# 00000022, 00000000, 00000000<br>
<br>
#db# 00000020, 000007d6, 00000093<br>
<br>
#db# 00000022, 00000000, 00000000<br>
<br>
#db# 00000020, 000007d6, 00000093<br>
</code></pre>
You can then decode the ISO14443-A commands which were snooped :<br>
<br>
<ul><li>the reader sends "26" (REAQA) to check if there are tags in the field.<br>
</li><li>the tag answers "0400" to say "Hi"<br>
</li><li>the reader says "9320" to select the tag —or any tag in the field—<br>
</li><li>the tag answers with its UID<br>
</li><li>the reader retries "select" with the tag’s UID<br>
</li><li>the tag says "I’m a mifare 1K"<br>
</li><li>the reader attempts to read block zero (30 00 02 a8)<br>
</li><li>the tag says "Not allowed" (04)<br>
</li><li>the reader says "Halt" (50 00 57 cd)<br>
<pre><code>&gt; hf 14a list<br>
recorded activity:<br>
ETU     :rssi: who bytes<br>
---------+----+----+-----------<br>
+      0:    :     26<br>
+ 381383:    :     26<br>
+ 381375:    :     26<br>
+     64:   0: TAG 04  00<br>
+   3432:    :     93  20<br>
+     64:   0: TAG 84  76  81  dd  ae<br>
+   7345:    :     93  70  84  76  81  dd  ae  01  9a<br>
+     64:   0: TAG 08  b6  dd<br>
+  97771:    :     30  00  02  a8<br>
+     72:   0: TAG 04<br>
+   5368:    :     50  00  57  cd<br>
...<br>
</code></pre>
</li><li>the Mifare UID of the card is indeed 847681DD</li></ul>

<hr />

<h3>Reading a ISO15693 Tag</h3>
The image below shows the reading from a iso-15683 ski pass as used in the French alps<br>
<br>
<pre><code>proxmark3&gt; hf 15 read<br>
proxmark3&gt; data samples<br>
proxmark3&gt; data norm<br>
proxmark3&gt; hf 15 demod<br>
SOF at 10, correlation 387<br>
EOF at 410<br>
12 octets<br>
#  0: 00<br>
#  1: 01<br>
#  2: fd<br>
#  3: f1<br>
#  4: 82<br>
#  5: 12<br>
#  6: 00<br>
#  7: 00<br>
#  8: 07<br>
#  9: e0<br>
# 10: 7d<br>
# 11: da<br>
CRC=da7d<br>
proxmark3&gt;<br>
</code></pre>

<img src='http://proxmark3.googlecode.com/svn/wiki/15693-samples.png' />

<hr />

<h3>Getting the sector key of a mifare card</h3>
<h4>Snooping on Mifare communications</h4>

This is a working example of how the sector keys of mifare cards can be retrieved with a Proxmark3, using the "crapto-1" package found on Google Code.<br>
<br>
The trace below is taken from a hi14asnoop session followed by hf 14a list to get the beginning of the authentication & encryption protocol :<br>
<br>
<table><thead><th> <b>Commands</b> </th><th> <b>Comment</b> </th></thead><tbody>
<tr><td> + 561882 : 1 : 26 </td><td> REQA           </td></tr>
<tr><td> + 64 : 2 : TAG 04 00 </td><td> Answer reqa    </td></tr>
<tr><td> + 10217 : 2 : 93 20 </td><td> Select         </td></tr>
<tr><td> + 64 : 5 : TAG 9c 59 9b 32 6c </td><td> The card’s UID is therefore : 9c 59 9b 32 </td></tr>
<tr><td> + 12313 : 9 : 93 70 9c 59 9b 32 6c 6b 30 </td><td> Select with UID </td></tr>
<tr><td> + 64 : 3 : TAG 08 b6 dd </td><td> Tag type (Mifare 1K) </td></tr>
<tr><td> + 923318 : 4 : 60 00 f5 7b </td><td> AUTH (block 00) </td></tr>
<tr><td> + 112 : 4 : TAG 82 a4 16 6c </td><td> Tag challenge (nt, "Nonce Tag") </td></tr>
<tr><td> + 6985 : 8 : a1 e4 ! 58 ce ! 6e ea ! 41 e0 ! </td><td> nr XOR ks1 (Nonce Reader, encrypted, 4 bytes), <br> ar XOR ks2 (Answer Reader to Nonce Tag, encrypted) </td></tr>
<tr><td> + 64 : 4 : TAG 5c ! ad f4 39 ! </td><td> at XOR ks3 (Answer Tag, encrypted) </td></tr></tbody></table>

In order to extract the key for sector 0 from the exchange, we need the following elements :<br>
<br>
<ul><li>Tag UID<br>
</li><li>Tag challenge (nt)<br>
</li><li>Reader challenge, encrypted (nr xor ks1, aka <a href='nr.md'>nr</a>)<br>
</li><li>Reader response, encrypted (ar XOR ks2, aka <a href='ar.md'>ar</a>)<br>
</li><li>Tag response, encrypted (at XOR ks3, aka <a href='at.md'>at</a>)</li></ul>

In the example above :<br>
<br>
<ul><li>UID : 0x9c599b32<br>
</li><li>nt : 0x82a4166c<br>
</li><li><a href='nr.md'>nr</a> : 0xa1e458ce<br>
</li><li><a href='ar.md'>ar</a> : 0x6eea41e0<br>
</li><li><a href='at.md'>at</a> : 0x5cadf439</li></ul>

Those can then be used in the following "crapto1" test program :<br>
<pre><code>// Test-file: test2.c<br>
#include "crapto1.h"<br>
#include &lt;stdio.h&gt;<br>
<br>
int main (void)<br>
{<br>
 struct Crypto1State *revstate;<br>
 uint64_t lfsr;<br>
 unsigned char* plfsr = (unsigned char*)&amp;lfsr;<br>
<br>
<br>
 uint32_t uid                = 0x9c599b32;<br>
 uint32_t tag_challenge      = 0x82a4166c;<br>
 uint32_t nr_enc             = 0xa1e458ce;<br>
 uint32_t reader_response    = 0x6eea41e0;<br>
 uint32_t tag_response       = 0x5cadf439;<br>
<br>
 uint32_t ks2                = reader_response ^ prng_successor(tag_challenge, 64);<br>
 uint32_t ks3                = tag_response ^ prng_successor(tag_challenge, 96);<br>
<br>
 printf("nt': %08x\n",prng_successor(tag_challenge, 64));<br>
 printf("nt'': %08x\n",prng_successor(tag_challenge, 96));<br>
<br>
 printf("ks2: %08x\n",ks2);<br>
 printf("ks3: %08x\n",ks3);<br>
<br>
 revstate = lfsr_recovery(ks2, ks3);<br>
 lfsr_rollback(revstate, 0, 0);<br>
 lfsr_rollback(revstate, 0, 0);<br>
 lfsr_rollback(revstate, nr_enc, 1);<br>
 lfsr_rollback(revstate, uid ^ tag_challenge, 0);<br>
 crypto1_get_lfsr(revstate, &amp;lfsr);<br>
 printf("Found Key: [%02x %02x %02x %02x %02x %02x]\n\n",plfsr[0],plfsr[1],plfsr[2],plfsr[3],plfsr[4],plfsr[5]);<br>
<br>
 return 0;<br>
}<br>
</code></pre>
Then compiled with :<br>
<pre><code>#gcc -o test2 test2.c crapto1.c crypto1.c<br>
</code></pre>
And run like this :<br>
<pre><code>./test2<br>
nt': 8d65734b<br>
nt'': 9a427b20<br>
ks2: e38f32ab<br>
ks3: c6ef8f19<br>
Found Key: [ff ff ff ff ff ff]<br>
</code></pre>

<hr />

<h2>Troubleshooting</h2>
<h3>USB Connection issues</h3>

The Proxmark3 sometimes has trouble with USB connections, especially on Linux (although as of July 2009 the usb subsystem should automatically detach the Linux kernel driver, so make sure you are running the latest versions before resorting to the following fixes).<br>
<pre><code>   For a short term fix : Plug the proxmark in an USB 1.1 hub or directly in a port on the computer, so ehci_hcd does not handle it, but the old USB 1.1 driver (uhci_hcd or ohci_hcd ; unloading ehci_hcd should do the trick, too).<br>
</code></pre>

On Ubuntu Jaunty, ehci_hcd support is built into the kernel. In this case, you can disable ehci support from the /sys/ interface :<br>
<pre><code># ls /sys/bus/pci/drivers/ehci_hcd<br>
</code></pre>
You should see an entry for your USB 2.0 host device, something like 0000:00:xx.x<br>
<br>
Then do<br>
<pre><code># echo -n 0000:00:xx.x &gt; unbind<br>
</code></pre>
This will unbind your device from ehci_hcd.<br>
<br>
To be tested :<br>
<br>
You can disable this on boot by creating a /etc/udev/rules.d/disable-ehci.rules file containing :<br>
<pre><code>ACTION=="add", SUBSYSTEM=="pci", DRIVER=="ehci_hcd", RUN+="/bin/sh -c 'echo -n %k &gt; %S%p/driver/unbind'"<br>
</code></pre>

<h3>Making the proxmark3 work without root access</h3>

Just create the right rules file in udev<br>
<pre><code>~$ cat /etc/udev/rules.d/026-proxmark.rules<br>
#Proxmark3<br>
SUBSYSTEM=="usb", ATTR{idVendor}=="9ac4", ATTR{idProduct}=="4b8f", MODE="0660", GROUP="adm"<br>
</code></pre>
This one gives access to the proxmark for users in the adm group...