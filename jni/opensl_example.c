/*
opensl_example.c:
OpenSL example module
Copyright (c) 2012, Victor Lazzarini
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <android/log.h>
#include <stdio.h>
#include "opensl_io.h"

// Include for encoder and decoder.
#include <typedef.h>
#include <basop.h>
#include <cst_lbc.h>
#include <tame.h>
#include <util_cng.h>
#include <util_lbc.h>
#include <vad.h>
#include <lbccodec.h>
#include <exc_lbc.h>
#include <lpc.h>
#include <lsp.h>
#include <cod_cng.h>
#include <dec_cng.h>
#include <tab_lbc.h>
#include <coder.h>
#include <decod.h>

#define BUFFERFRAMES 240
#define VECSAMPS_MONO 240
#define VECSAMPS_STEREO 480
#define SR 8000

static int on;
void start_process() {
  OPENSL_STREAM  *p;
  FILE *fp;
  char adpcmdata_elements[12];

  //Initialize Encoder and decoder
  Init_Coder();
  Init_Vad();
  Init_Cod_Cng();
  reset_max_time();
  Init_Decod();
  Init_Dec_Cng();

  int samps, i, j;
  short  inbuffer[VECSAMPS_MONO], outbuffer[VECSAMPS_MONO];
  p = android_OpenAudioDevice(SR,1,1,BUFFERFRAMES);
  if(p == NULL) return;
  on = 1;

  while(on) {
   samps = android_AudioIn(p,inbuffer,VECSAMPS_MONO);

   //fp = fopen("/sdcard/test.pcm", "ab");
   //fwrite(inbuffer, sizeof(short), sizeof(short)*VECSAMPS_MONO, fp);
   //fclose(fp);

   Coder(inbuffer, adpcmdata_elements);
   Decod(outbuffer, adpcmdata_elements, 0);

   android_AudioOut(p,outbuffer,samps);
  }

  android_CloseAudioDevice(p);
}

void stop_process(){
  on = 0;
}
