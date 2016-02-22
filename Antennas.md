# Antennas #




---


## Connecting the Proxmark3 to an antenna ##
The proxmark supports two loop antennas that are each used to transmit and receive. One is used for LF (125kHz and 134kHz frequencies) and another for HF (13.56MHz). A homegrown antenna does not have to connect through the Hirose connector and can be soldered directly to the test points TP2 through TP5.

## Hirose pinout ##

The figure below shows which Hirose connector pins connect to which test point.

![http://proxmark3.googlecode.com/svn/wiki/hirose-pins-2.jpg](http://proxmark3.googlecode.com/svn/wiki/hirose-pins-2.jpg)

## High frequency antenna ##

High frequency coils should be connected between TP3 and TP4 either on the Hirose connector or soldered direct to the test points. A standard 13.56MHz coil is usually made out of about 4 turns of enamel wire and the coil diameter is about 5cm. The coil can be circular but deforming it into a rectangular shape can help tune it closer to the desired frequency.

The tune command can be used to verify that the antenna is tuned properly. For a High frequency antenna, it should return a value over 10 Volts in the HF coil.

## Low frequency antenna ##
Low frequency coils should be connected between TP2 and TP5 either on the Hirose connector or soldered direct to the test points.. A standard 125kHz coil is made of around a hundred turns of thin enamel wire.

The tune command should return a value of over 20 Volts for a properly tuned antenna but it should not exceed 45 Volts.


---


# Antenna examples #

## Roel’s Hirose antenna ##

This antenna design uses a standard Hirose mini USB cable to make a neat and cheap High Frequency (HF) antenna for all 13.56MHz modulations (MIFARE, Felica,etc.).

The antenna forms a LC circuit with proxmark3's C35 capacitor. This design works best when C35 is a 47pF capacitor, but some proxmark3 boards are known to have 100pF capacitors. If you have a 100pF capacitor, the inductance of L will need to be lower than the antenna of this design creates. The formula for calculating the proper value of L given a known frequency (F) and capacitor value (C) is F=1/(2\*pi\*sqrt(L\*C)).

On the left you see a Hirose connector, there are multiple mini-USB cables available on the market, so make sure you buy the correct one.

![http://proxmark3.googlecode.com/svn/wiki/hirose_usb_cable_new.jpg](http://proxmark3.googlecode.com/svn/wiki/hirose_usb_cable_new.jpg)

First you have to strip the cable. You can cut of the USB side ; we do not need this part. Make sure the total size of the cable is bigger than 80cm. After cutting the cable you make an incision at 6.5cm and remove the insulation. Do the same to the shielding that is underneath the plastic insulation. You will see 4 wires appear in different colors.

![http://proxmark3.googlecode.com/svn/wiki/hirose_cable_no_shielding.jpg](http://proxmark3.googlecode.com/svn/wiki/hirose_cable_no_shielding.jpg)
![http://proxmark3.googlecode.com/svn/wiki/hirose_cable_stripped.jpg](http://proxmark3.googlecode.com/svn/wiki/hirose_cable_stripped.jpg)


We do not need the red and white wire for a HF antenna. Those are for connecting a LF antenna. So cut away the shielding, red and white wire. We have left, the connector and 2 wires, black and green, which are at least 80cm. Cut the green wire at 19cm from the connector and strip it a bit. Do the same to the black wire, but use the length 76cm.

![http://proxmark3.googlecode.com/svn/wiki/black_length.jpg](http://proxmark3.googlecode.com/svn/wiki/black_length.jpg)

Make an antenna coil of 3 windings using the green cable. Connect the green cable with the black one and tape them together so they won’t unwind.

Plug it into the Proxmark and you will get an antenna with 13V. You can even optimize this value by adjusting the length of the wires. Note that small changes (1cm) can already have a big impact. If you found out better values than we describe, please drop a note so we update this manual.

![http://proxmark3.googlecode.com/svn/wiki/antenne_in_proxmark.jpg](http://proxmark3.googlecode.com/svn/wiki/antenne_in_proxmark.jpg)

Check this website for more tunes and tweaks for your antenna:
http://blog.madspace.nl/2010/01/playing-with-the-proxmark3/


---


# Antenna Building tips #

Those various tips were taken from forum posts :

From http://www.proxmark.org/forum/viewtopic.php?id=122
```
Glad to see someone else is still working with the antennas ! I’ve had decent results with my 50 ohm antennas without much additional matching, only tuning of parallel RLC circuits. One that seems to work particularly well and is small was a RR-IDISC-ANT4-3-A from Digi-key (though kind of pricey). It gave read ranges of about 3 cm.

I also made up some antennas using copper tape and a few trimmer caps. This seemed to work almost as well and was a lot cheaper. With larger loops I could get 3 - 4 cm. I made a fixture for the PM3 that attaches an SMA connector to the test points. That way I could attach my homemade antennas (which also have SMA connectors) via coax. This avoids the TL effects and allows the antenna to be located pretty far away from the device.

Either way you’ll probably have to tune the antennas slightly (or tune the PM3 by changing C35/C36), but you should get it to work. The RR antenna has a trimmer cap so that is nice.
```
From http://www.proxmark.org/forum/viewtopic.php?id=90 : very good description of how to build a home-made antenna.

# Antenna/tag orientation #

From http://www.proxmark.org/forum/viewtopic.php?id=119
```
What works for me is the plane of the PM3 antenna is right next to the plane of the RFID card but the long side of the RFID card is rotated about 45 degrees relative to the long side of the antenna. If you look here at the first two pictures, where the green wire rectangle is the PM3 antenna you get an idea of the setup.
```