
# Getting started #

It is assumed that the reader of this document is the local administrator of a machine running Linux. This document is intended as a guide only. Group policies and custom configurations are outside the scope of this document. This guide has been tested against the Ubnutu distribution, but should work for most flavours of Linux with minor adjustments.

# Requirements #
  * A computer running Ubuntu 10.04 or greater with an available USB port.
  * This guide will most likely work with any recent version, but has been tested with:
    * Ubuntu 10.04, 10.10, 11.04, 12.04 and 13.04
    * `BackTrack 5 R1` and `4 R2`.
  * A USB Mini-B lead.
  * A Proxmark III.
  * HF and / or LF antenna for the Proxmark.
A technical understanding of the Proxmark III is not required for the installation process.

# Development environment installation #
  * Open a terminal:

> `CTRL + ALT + T`

  * Download (and install) TortoiseSVN, p7zip, and components essential to build the Proxmark from soruce:

> `sudo apt-get install subversion p7zip build-essential libreadline5 libreadline-dev libusb-0.1-4 libusb-dev libqt4-dev perl pkg-config wget`

  * Check out the latest revision of the Proxmark project:

> `svn checkout http://proxmark3.googlecode.com/svn/trunk pm3`


  * Get devkitARM release 32 (4.5.1) from [SourceForge](http://sourceforge.net/projects/devkitpro/):

> `http://sourceforge.net/projects/devkitpro/files/devkitARM/devkitARM_r41-x86_64-linux.tar.bz2/download`

  * Extract the contents of the .tar.bz2:

> `tar jxvf devkitARM_r41-x86_64-linux.tar.bz2`

  * Create a directory for the arm dev kit:

> `sudo mkdir /opt/devkitpro/`

  * Move the ARM developer kit to the newly created directory:

> `sudo mv devkitARM /opt/devkitpro/`

  * Add the appropriate environment variable:

> `export PATH=${PATH}:/opt/devkitpro/devkitARM/bin/`

  * Add the environment variable to your profile:

> `echo 'PATH=${PATH}:/opt/devkitpro/devkitARM/bin/' >> ~/.bashrc`

  * Re-load your profile:

> `. ~/.bashrc`

# PROXMARK DRIVER INSTALLATION #
No drivers are required to use the Proxmark within Linux. You will however need to do a couple of things as root:

  * Add yourself to the dialout group:

> `sudo adduser <username> dialout`

> (You will need to logoff before the changes will take effect.)

  * Configure modem-manager to ignore the proxmark (this is important as it will block flashing):

> `sudo vi /etc/udev/rules.d/77-mm-usb-device-blacklist.rules`

  * Create this file if it doesn't already exist, and add the following:
```
# proxmark3 - http://www.proxmark.org/
ATTRS{idVendor}=="2d2d" ATTRS{idProduct}=="504d", ENV{ID_MM_DEVICE_IGNORE}="1"
```

  * restart udev hotplug control:

> `sudo udevadm control --reload-rules`

# Testing the Proxmark #
You are now at the stage where you should be able to communicate with your Proxmark.
  * Plug in and check which port is came up as:
> > `dmesg`

  * You should see something like:
```
[59751.565721] usb 2-1.2: new full-speed USB device number 68 using ehci-pci
[59751.659024] usb 2-1.2: New USB device found, idVendor=2d2d, idProduct=504d
[59751.659035] usb 2-1.2: New USB device strings: Mfr=1, Product=0, SerialNumber=0
[59751.659041] usb 2-1.2: Manufacturer: proxmark.org
[59751.659767] cdc_acm 2-1.2:1.0: This device cannot do calls on its own. It is not a modem.
[59751.659804] cdc_acm 2-1.2:1.0: ttyACM3: USB ACM device
```
  * Go in to your Proxmark project folder and run the Proxmark software:


> `cd client; ./proxmark3 /dev/ttyACM3`

  * You should get the proxmark command prompt:

> `proxmark3> `

  * Get the proxmark version info:

> `hw version`

  * You should see something like this:
```
#db# Prox/RFID mark3 RFID instrument                 
#db# bootrom: svn 715 2013-05-16 15:34:53                 
#db# os: svn 798-unclean 2013-10-04 20:02:30                 
#db# FPGA image built on 2012/ 1/ 6 at 15:27:56                 
uC: AT91SAM7S256 Rev A          
Embedded Processor: ARM7TDMI          
Nonvolatile Program Memory Size: 256K bytes          
Second Nonvolatile Program Memory Size: None          
Internal SRAM Size: 256K bytes          
Architecture Identifier: AT91SAM7Sxx Series          
Nonvolatile Program Memory Type: Embedded Flash Memory
```
  * Connect your antenna(s) to the Proxmark and type in:

> `hw tune`

  * You should see something like this:
```
#db# Measuring antenna characteristics, please wait.
# LF antenna: 33.17 V @ 125.00 kHz
# LF antenna: 41.89 V @ 134.00 kHz
# LF optimal: 41.76 V @ 127.66 kHz
# HF antenna: 7.28 V @ 13.56 MHz
```
  * Type "`quit`" to exit out of the program.

# Notes #
There are many commands available within the Proxmark client. Type "`help`" to list the commands available to you. You get a list of following subcommands by typing in the command you're interested followed by help (Eg. "`hf help`"). For detailed help please read the Proxmark User Manual.

Command shortcutting is permitted. For instance typing "`hf mf re`" will achieve the same results as typing "`hf mf restore1k`" because it is the only "`hf mf`" command available that begins with "`re`".

You might have noticed when you executed the command "`hw version`" there was a "`-unclean`" or "`-suspect`" in the "`bootrom`" or "`os`" version information. This information indicates that the code may have changes in the local code versus the subversion revision.

  * A clean build is a build that has no local changes versus the subversion revision.
  * Unclean builds have local changes versus the subversion revision.
  * Suspect builds cannot be compared against the subversion revision.

## Next steps ##
From here on in the rest is up to you. You might want to confirm that the firmware you're running is the latest. In addition to this you might want to begin reading through the Proxmark source code and making changes of your own.

We suggest you read the following documentation:
  * [Proxmark source and firmware upgrading](Compiling.md)
  * [Proxmark User Manual](RunningPM3.md)
  * [Antenna Construction Guide](Antennas.md)