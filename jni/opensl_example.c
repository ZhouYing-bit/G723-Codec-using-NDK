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
