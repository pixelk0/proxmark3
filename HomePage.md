# NOTICE #
===(2014-03-31)===<br />
THIS REPOSITORY HAS MOVED TO GITHUB! PLEASE DO NOT COMMIT YOUR CHANGES HERE!<br />
Please visit https://github.com/Proxmark/proxmark3 for the latest version of the repository.<br />
<br />
For those unfamiliar with git:<br />
http://try.github.io/levels/1/challenges/1<br />
http://readwrite.com/2013/09/30/understanding-github-a-journey-for-beginners-part-1<br />
http://lifehacker.com/5983680/how-the-heck-do-i-use-github<br />
<br />
===(2014-03-17)===<br />
Moving this repository to GitHub is up for discussion! Please check out the following thread and post your comments...<br />
[Move repo to Github](http://www.proxmark.org/forum/viewtopic.php?id=1902)<br />
Discussions will close on March 31st.<br />
<br /><br />
<font color='grey'>
<h1>Introduction</h1>

This is the home page of a group of volunteer enthusiasts committed to further enhancing the capabilities of the already awesome Proxmark3, originally developed by Jonathan Westhues and release under the terms of GPL. You are encouraged to <a href='http://www.cq.cx/proxmark3.pl'>visit his site</a> to read his original write up on the build of the proxmark3 as well as take a look at the historical progression from the basic <a href='http://www.cq.cx/prox.pl'>prox</a> to the <a href='http://www.cq.cx/proxmarkii.pl'>markII</a> to the final <a href='http://www.cq.cx/proxmark3.pl'>proxmark3</a>. Please bear in mind that any statements on Jonathan's web site relating to the abilities of the board may be little dated now, and the capabilities of the proxmark3 have been (and continue to be) further enhanced, by great enthusiasts, such as <a href='http://gerhard.dekoninggans.nl/proxmark/software'>Gerhard de Koning Gans</a> who added ISO-14443a support and many others who continue to add features in their own time.<br>
<br>
To further help those new to the proxmark3, we have created a <a href='ReferenceManual.md'>User's Manual</a> where we attempt to document all the commands and their usage. You are welcome to join us on the <a href='http://proxmark.org/forum'>proxmark.org</a> forum, to find out the latest news, ask for help, or to discuss new ideas.<br>
<br>
<h1>What is it</h1>
The proxmark3 is a powerful general purpose RFID tool, the size of a deck of cards,  designed to snoop, listen and emulate everything from Low Frequency (125kHz) to High Frequency (13.56MHz) tags.<br>
<br>
<img src='http://proxmark3.googlecode.com/svn/wiki/prox3-straight.jpg' />

From the <a href='http://www.cq.cx/proxmark3.pl'>original website</a> :<br>
<br>
<blockquote>This device can do almost anything involving almost any kind of low ( 125 kHz) or high ( 13.56 MHz) frequency RFID tag. It can act as a reader. It can eavesdrop on a transaction between another reader and a tag. It can analyse the signal received over the air more closely, for example to perform an attack in which we derive information from the tag’s instantaneous power consumption. It can pretend to be a tag itself. It is also capable of some less obviously useful operations that might come in handy for development work.</blockquote>

<h1>Is it for me ?</h1>

It should be pointed out quite early that the proxmark3 is not really for beginners. If you are not already fairly familiar with electronics, embedded programming, some RF design and ISO standards, this device will probably bring you more frustration than anything else ! Users that do not understand the basic principles behind RFID may have difficulty using the device.<br>
<br>
If you are reasonably hard core, you could build one yourself. The Eagle PCB designs, gerber files, etc are freely available. There is an updated component list which can help you source the components from any of the major suppliers. Alternatively you could buy a ready-made board from somewhere like <a href='http://www.proxmark3.com'>proxmark3.com</a> or <a href='http://hackable-devices.org/products/product/proxmark3/'>hackable-devices.org</a>

The Proxmark III is a RFID development tool. Typically, an "out of the box" proxmark3 with the latest firmware can run acquisitions in LF and HF mode and output traces, quite useful already, decode a number of different tag types, do some operations in ISO 15693 and ISO 14443 a and b modes, but if you really want to get the most out of this device, you will need to start enhancing the firmware yourself to suit your needs.<br>
<br>
As long as you only want to add higher level functions over the RF layers that are already coded into the FPGA, you should be fine without VHDL skills, but if you want to actually go deeper and have the proxmark3 support new modulations, this will require modifying the FPGA verilog code, an interesting exercise if you’re motivated or are already a pro in that area, but very time consuming if you’re getting started !<br>
<br>
Now you should have a pretty good idea on whether this tool is for you or not !<br>
</font>