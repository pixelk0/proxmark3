
## Before you start ##
This document has been created assuming that you have read the relevant getting started guide and configured your development environment accordingly.

For Windows users - Everything in this document is done from the Minimalist GNU terminal window. Start by running "`runme.bat`".

## Flash sections ##
Proxmark firmware is comprised of three logical sections: bootrom, fpga and operating system. The bootrom is a relatively small bit of code that performs some basic hardware initialization, supports reflashing the device over USB and knows how to transfer execution to the operating system. Due to the limited number of features exposed by the bootrom, it is not frequently updated and so you should only rarely need to update it when there is a compatibility conflict.

![http://proxmark3.googlecode.com/svn/wiki/Firmware.png](http://proxmark3.googlecode.com/svn/wiki/Firmware.png)

The FPGA code processes analogue signals coming from the antennas and makes those signals available to the ARM. Like the bootrom code, the FPGA code is not frequently updated. Presently, the operating system is the most frequently updated portion of Proxmark code. It is responsible for receiving and executing most of the commands advertised in the client user interface. Upgrading the bootrom of your Proxmark can brick the device. Please exercise caution when upgrading the bootloader. If the bootloader is corrupted, the only way to restore your Proxmark to working order will be through the use of a JTAG programmer.

An S19 (Motorola S-record) and ELF (Executable and Linkable Format) file is produced for each logical section.
  * ELF files are used with the flasher program for upgrading the Proxmark over USB.
  * S19 files are used with a JTAG programmer for upgrading the Proxmark.

### Bootloader ###
  * File name: `bootrom/obj/bootrom.s19`
  * File name: `bootrom/obj/bootrom.elf`

The **bootloader** (sometimes also called bootrom) is a small piece of code that enables writing to the flash over USB in the first place. It's updated very infrequently and, due to its importance, should be left alone most of the time. As long as your bootloader is intact you can recover any firmware error of your proxmark3 with just an USB connection and the provided flash tools. When you destroy your bootloader, you will need a JTAG connection and associated tools to recover the device.

### FPGA image ###
  * File name: `armsrc/obj/fpgaimage.s19`
  * File name: `armsrc/obj/fpgaimage.elf`

The **FPGA image** is the configuration/code that runs on the FPGA that sits on the proxmark3. Since the FPGA doesn't have flash memory of its own, this configuration is stored in the flash memory of the ARM processor and downloaded into the FPGA on each boot of the device. The FPGA code (called bit stream) is updated fairly infrequently, and special tools are needed to compile the bit stream from the verilog sources. The proxmark3 firmware source releases and SVN checkouts always contain a compiled bit stream (in fpga/fpga.bit) which is then simply wrapped into an image file for you to flash onto the proxmark3.

### OS image ###
  * File name: `armsrc/obj/osimage.s19`
  * File name: `armsrc/obj/osimage.elf`

The **OS image** is the main firmware code that runs on your proxmark3. It is developed and regularly updated in the SVN, but if you don't need the latest features you are well off using the provided binary releases. (Note also: since the SVN is considered eternal work-in-progess, a current SVN version might not work or even build.) The OS image and FPGA image work in tandem, so you should always flash them from the same SVN revision.

## Revision history and flashing procedures ##
As mentioned above, the Proxmark3 is largely a constant work in progress. Its firmware started off as a big update from the older ProxmarkII project, and was mainly aimed at demonstrating a few key concepts rather than provide a polished end-user interface.

Over time, and thanks to the many contributions of all the developers on the project, the firmware has evolved in many ways: new commands and features, more refined firmware architecture, etc.

### Original firmware ###
The original firmware which is the one shipped on some pre-built Proxmark3 devices is now generally considered as outdated. If your Proxmark3 is loaded with the original firmware, you should consider upgrading to the most recent binary release.

## Flashing procedure ##
Ensure that you have read the prior section before proceeding. In order to upgrade to the latest version of firmware, you will need to first upgrade the Proxmark�s bootloader.

The steps below will upgrade the Proxmark bootloader to the version you checked out previously using the procedure from the getting started guide.

  1. Optional � Update your working copy to the latest revision. (Refer to the getting started documentation).
  1. If you have not already done so, open up a terminal and go to the "pm3/client" directory.
  1. Press and hold the button on the Proxmark while connecting it to your computer. Continue to hold the button until the yellow and red LEDs stay lit.
  1. Upgrade the Proxmark bootrom by executing the following command:
> > sudo ./flasher -b ../bootrom/obj/bootrom.elf

At this point the bootrom has been updated and the Proxmark is now in a position to have its OS upgraded. The following steps will upgrade the Proxmark Operating System and FPGA code to your checked revision.

  1. Ensure that the Proxmark is not connected to the PC.
  1. Hold down the Proxmark�s button and connect it to the PC. After the yellow and red LEDs are lit, execute the command below:
```
sudo ./flasher ../armsrc/obj/fpgaimage.elf
```
  1. If the previous step is successful, disconnect the Proxmark.
  1. While holding the button, connect the Proxmark to the PC and wait for the yellow and red LEDs to stay lit. Execute the command below:
```
sudo ./flasher ../armsrc/obj/osimage.elf
```
  1. Disconnect the Proxmark from the PC and then reconnect it.
  1. Launch the client software by executing "./proxmark3.exe". The client should successfully connect to the Proxmark. You should see something like this:
```
Connected units:
1. SN: ChangeMe [bus-0/\\.\libusb0-0001--0x9ac4-0x4b8f]
proxmark3>
```

## JTAG Recovery Procedure ##
If for whatever reason the USB upgrade procedure (section above) failed and the Proxmark will no longer boot, you will need to load the bootrom on to the Proxmark using the JTAG interface. This procedure assumes that you have a Segger J-LINK for the recovery process and J-link commander installed.

Plug both the Proxmark and the Segger J-LINK in to the computer.
Attach the J-LINK to the Proxmark JTAG port.

Run J-link commander. Then:
```
exec device = AT91SAM7S256
exec EnableFlashDL
h
loadbin "C:\proxmark3\recovery\proxmark3_recovery.bin" 0x100000
```

that is it!

for flashing either the bootrom or firmware, these are the files and addresses:
```
bootrom.bin = 0x100000
fullimage.bin = 0x102000
```

Now un-plug the proxmark USB and JTAG and re-plug the Proxmark USB to the computer.
