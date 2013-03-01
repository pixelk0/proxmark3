//-----------------------------------------------------------------------------
// Copyright (C) 2010 iZsh <izsh at fail0verflow.com>
//
// This code is licensed to you under the terms of the GNU GPL, version 2 or,
// at your option, any later version. See the LICENSE.txt file for the text of
// the license.
//-----------------------------------------------------------------------------
// Main command parser entry point
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sleep.h"
#include "cmdparser.h"
#include "proxmark3.h"
#include "data.h"
#include "usb_cmd.h"
#include "ui.h"
#include "cmdhf.h"
#include "cmddata.h"
#include "cmdhw.h"
#include "cmdlf.h"
#include "cmdmain.h"
#include "util.h"

unsigned int current_command = CMD_UNKNOWN;
unsigned int received_command = CMD_UNKNOWN;
UsbCommand current_response;
UsbCommand current_response_user;

static int CmdHelp(const char *Cmd);
static int CmdQuit(const char *Cmd);

static command_t CommandTable[] = 
{
  {"help",  CmdHelp,  1, "This help. Use '<command> help' for details of the following commands:\n"},
  {"data",  CmdData,  1, "{ Plot window / data buffer manipulation... }"},
  {"exit",  CmdQuit,  1, "Exit program"},
  {"hf",    CmdHF,    1, "{ HF commands... }"},
  {"hw",    CmdHW,    1, "{ Hardware commands... }"},
  {"lf",    CmdLF,    1, "{ LF commands... }"},
  {"quit",  CmdQuit,  1, "Quit program"},
  {NULL, NULL, 0, NULL}
};

int CmdHelp(const char *Cmd)
{
  CmdsHelp(CommandTable);
  return 0;
}

int CmdQuit(const char *Cmd)
{
  exit(0);
  return 0;
}

bool WaitForResponseTimeout(uint32_t cmd, UsbCommand* response, size_t ms_timeout) {

	// Wait until the command is received
  for(size_t i=0; received_command != cmd && i < ms_timeout/10; i++) {
		msleep(10); // XXX ugh
    if (i == 200) { // Two seconds elapsed
      PrintAndLog("Waiting for a response from the proxmark...");
      PrintAndLog("Don't forget to cancel its operation first by pressing on the button");
    }
	}
	
  // Check if timeout occured
  if(received_command != cmd) return false;

	// Copy the received response (if supplied)
  if (response) {
    memcpy(response, &current_response, sizeof(UsbCommand));
  }

	// Reset the received command
  received_command = CMD_UNKNOWN;

	return true;
}

bool WaitForResponse(uint32_t cmd, UsbCommand* response) {
	return WaitForResponseTimeout(cmd,response,-1);
}

//-----------------------------------------------------------------------------
// Entry point into our code: called whenever the user types a command and
// then presses Enter, which the full command line that they typed.
//-----------------------------------------------------------------------------
void CommandReceived(char *Cmd) {
  CmdsParse(CommandTable, Cmd);
}

//-----------------------------------------------------------------------------
// Entry point into our code: called whenever we received a packet over USB
// that we weren't necessarily expecting, for example a debug print.
//-----------------------------------------------------------------------------
void UsbCommandReceived(UsbCommand *UC)
{
  /*
  //  Debug
  printf("UsbCommand length[len=%zd]\n",sizeof(UsbCommand));
  printf("  cmd[len=%zd]: %"llx"\n",sizeof(UC->cmd),UC->cmd);
  printf(" arg0[len=%zd]: %"llx"\n",sizeof(UC->arg[0]),UC->arg[0]);
  printf(" arg1[len=%zd]: %"llx"\n",sizeof(UC->arg[1]),UC->arg[1]);
  printf(" arg2[len=%zd]: %"llx"\n",sizeof(UC->arg[2]),UC->arg[2]);
  printf(" data[len=%zd]: %02x%02x%02x...\n",sizeof(UC->d.asBytes),UC->d.asBytes[0],UC->d.asBytes[1],UC->d.asBytes[2]);
  */

  //	printf("%s(%x) current cmd = %x\n", __FUNCTION__, c->cmd, current_command);
  // If we recognize a response, return to avoid further processing
  switch(UC->cmd) {
      // First check if we are handling a debug message
    case CMD_DEBUG_PRINT_STRING: {
      char s[USB_CMD_DATA_SIZE+1];
      size_t len = MIN(UC->arg[0],USB_CMD_DATA_SIZE);
      memcpy(s,UC->d.asBytes,len);
      s[len] = 0x00;
      PrintAndLog("#db# %s       ", s);
      return;
    } break;

    case CMD_DEBUG_PRINT_INTEGERS: {
      PrintAndLog("#db# %08x, %08x, %08x       \r\n", UC->arg[0], UC->arg[1], UC->arg[2]);
      return;
    } break;

    case CMD_MEASURED_ANTENNA_TUNING: {
      int peakv, peakf;
      int vLf125, vLf134, vHf;
      vLf125 = UC->arg[0] & 0xffff;
      vLf134 = UC->arg[0] >> 16;
      vHf = UC->arg[1] & 0xffff;;
      peakf = UC->arg[2] & 0xffff;
      peakv = UC->arg[2] >> 16;
      PrintAndLog("");
      PrintAndLog("# LF antenna: %5.2f V @   125.00 kHz", vLf125/1000.0);
      PrintAndLog("# LF antenna: %5.2f V @   134.00 kHz", vLf134/1000.0);
      PrintAndLog("# LF optimal: %5.2f V @%9.2f kHz", peakv/1000.0, 12000.0/(peakf+1));
      PrintAndLog("# HF antenna: %5.2f V @    13.56 MHz", vHf/1000.0);
      if (peakv<2000)
        PrintAndLog("# Your LF antenna is unusable.");
      else if (peakv<10000)
        PrintAndLog("# Your LF antenna is marginal.");
      if (vHf<2000)
        PrintAndLog("# Your HF antenna is unusable.");
      else if (vHf<5000)
        PrintAndLog("# Your HF antenna is marginal.");
    } break;
      
    case CMD_DOWNLOADED_RAW_ADC_SAMPLES_125K: {
//      printf("received samples: ");
//      print_hex(UC->d.asBytes,512);
      sample_buf_len += UC->arg[1];
//      printf("samples: %zd offset: %d\n",sample_buf_len,UC->arg[0]);
      memcpy(sample_buf+(UC->arg[0]),UC->d.asBytes,UC->arg[1]);
    } break;


//    case CMD_ACK: {
//      PrintAndLog("Receive ACK\n");
//    } break;

    default: {
      // Maybe it's a response
      switch(current_command) {
        case CMD_DOWNLOAD_RAW_ADC_SAMPLES_125K: {
          if (UC->cmd != CMD_DOWNLOADED_RAW_ADC_SAMPLES_125K) {
            PrintAndLog("unrecognized command %08x\n", UC->cmd);
            break;
          }
//          int i;
          PrintAndLog("received samples %d\n",UC->arg[0]);
          memcpy(sample_buf+UC->arg[0],UC->d.asBytes,48);
          sample_buf_len += 48;
//          for(i=0; i<48; i++) sample_buf[i] = UC->d.asBytes[i];
          received_command = UC->cmd;
        } break;

        default: {
        } break;
      }
//      // Store the last received command
//      memcpy(&current_response, UC, sizeof(UsbCommand));
//      received_command = UC->cmd;
    } break;
  }
  // Store the last received command
  memcpy(&current_response, UC, sizeof(UsbCommand));
  received_command = UC->cmd;
/*
  // Maybe it's a response:
  switch(current_command) {
    case CMD_DOWNLOAD_RAW_ADC_SAMPLES_125K:
      if (UC->cmd != CMD_DOWNLOADED_RAW_ADC_SAMPLES_125K) goto unexpected_response;
      int i;
      for(i=0; i<48; i++) sample_buf[i] = UC->d.asBytes[i];
      received_command = UC->cmd;
      return;
    case CMD_ACQUIRE_RAW_ADC_SAMPLES_125K:
    case CMD_DOWNLOADED_SIM_SAMPLES_125K:
      if (UC->cmd != CMD_ACK) goto unexpected_response;
      // got ACK
      received_command = UC->cmd;
      return;
    default:
    unexpected_response:

	if(UC->cmd != CMD_ACK)
		PrintAndLog("unrecognized command %08x       \n", UC->cmd);
	else
		memcpy(&current_response, UC, sizeof(UsbCommand));
	received_command = UC->cmd;
  }
 */
}