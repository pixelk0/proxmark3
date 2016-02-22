
# Getting started #

It is assumed that the reader of this document is the local administrator of a machine running Windows 7. This document is intended as a guide only. Group policies and custom configurations are outside the scope of this document. This guide has not been tested against the "Home" or "Starter" editions of Windows 7.

# Requirements #
  * A computer running Windows 7 with an available USB port.
  * A USB Mini-B lead.
  * A Proxmark III.
  * HF and / or LF antenna for the Proxmark.
A technical understanding of the Proxmark III is not required for the installation process.

# Development environment installation #
  1. Download (and install) 7-ZIP from here: http://www.7-zip.org/download.html
  1. Download (and install) TortoiseSVN from here: http://tortoisesvn.net/downloads.html
  1. Download "`ProxSpace-20100226-r390.7z`" from: http://code.google.com/p/proxmark3/downloads/list
  1. "`ProxSpace-20100226-r390.7z`" should be extracted wherever you want to keep the Proxmark project. Additionally, the location should not include spaces. For instance - "`C:\Projects\Proxmark`" would be acceptable whereas "`C:\Users\Administrator\My Projects\Proxmark`" would not.
  1. Assuming you installed 7-ZIP using all of the default options, you should have 7-ZIP context menu items.
  1. Right-click "`ProxSpace-20100226-r390.7z`" and select 7-Zip > Extract Here.
  1. Assuming you installed TortoiseSVN using all of the default options, you should have subversion context menu items.
  1. Go in to the newly created ProxSpace folder and right-click the "pm3" folder. Select SVN Update.
  1. In the ProxSpace folder there should be a file called "runme.bat". Open this file with a suitable text editor. (N++ is recommended)
    1. You will need to modify the variable "`MYPATH`".
> > > EXAMPLE
> > > If your ProxSpace folder path is "`C:\ProxMark\ProxSpace`" you would need to change the "`MYPATH`" variable to "`set MYPATH=%~d0\ProxMark\ProxSpace`".
    1. The last line of the batch file might have a forward slash - "`msys/msys.bat`". You will need to change this to "`msys\msys.bat`".
    1. Save and close the file.
  1. Test the configuration by running "`runme.bat`". You should see a Minimalist GNU terminal window.
  1. Type "`exit`" to close the window.

# PROXMARK DRIVER INSTALLATION #
  1. Download the Proxmark drivers ("`ProxSpace-Driver-Current.7z`") from here: http://code.google.com/p/proxmark3/downloads/list
  1. Assuming you installed 7-ZIP using all of the default options (Development Environment Installation Step 1.), you should have 7-ZIP context menu items.
  1. Right-click "`ProxMark-Driver.7z`" and select 7-Zip > Extract Here.
  1. Attach the Proxmark to the computer. A notification in the tray should appear indicating the ProxMark has been detected.

> > ![http://proxmark3.googlecode.com/svn/wiki/Windows-Installingdevicedriversoftware.jpg](http://proxmark3.googlecode.com/svn/wiki/Windows-Installingdevicedriversoftware.jpg)
  1. Open the Device Manager.
> > Start > Control Panel > System > Hardware > Device Manager.
> > - or -

> Win + Pause / Break > Hardware > Device Manager.
    1. Expand out the "Human Interface Devices" and double-click the last "USB Input Device" in the tree.
    1. In the window that appears, verify the device selected is the Proxmark by checking the "Bus reported device description" value under the "Details" tab.
      1. If the "Bus reported device description" value does not contain "ProxMark-3 RFID Instrument", close the window and try another "USB Input Device" in the tree.
    1. Click Driver > Update Driver... > Browse my computer for driver software > Let me pick from a list of device drivers on my computer > Have Disk... > Browse.
    1. In the Locate File window, navigate to the Proxmark Driver folder created at step 3. Select "ProxMark-3\_RFID\_Instrument.inf" then click Open > Ok > Next > Close.
      1. Installing this driver may require a restart.
      1. If you go back in to the Device Manager you should see "ProxMark-3 RFID Instrument" under the "LibUSB-Win32 Devices" node.

# Testing the Proxmark #
You are now at the stage where you should be able to communicate with your Proxmark.
  1. Go in to your Proxmark project folder and run the "`runme.bat`" file.
    1. If for whatever reason "`runme.bat`" does not open a Minimalist GNU terminal window, open a command prompt and run "`runme.bat`" from there. By doing so you should see any errors and diagnose the problem.
  1. Run the Proxmark software - "`./client/proxmark3.exe`".
    1. You should see something like this:
```
Connected units:
1. SN: ChangeMe [bus-0/\\.\libusb0-0001--0x9ac4-0x4b8f]
proxmark3>
```
  1. Next, check what firmware you are running - "`hw version`".
    1. You should see something like this:
```
#db# Prox/RFID mark3 RFID instrument
#db# bootrom: svn 486-suspect 2011-07-18 12:48:52
#db# os: svn 486-suspect 2011-07-18 12:48:57
#db# FPGA image built on 2009/12/ 8 at 8: 3:54
```
  1. Connect your antenna(s) to the Proxmark and type in "`hw tune`".
    1. You should see something like this:
```
#db# Measuring antenna characteristics, please wait.
# LF antenna: 33.17 V @ 125.00 kHz
# LF antenna: 41.89 V @ 134.00 kHz
# LF optimal: 41.76 V @ 127.66 kHz
# HF antenna: 7.28 V @ 13.56 MHz
```
  1. Type "`quit`" to exit out of the program.

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