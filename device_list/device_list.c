/**************************************************
 * PortAudio API practice
 *   Show all available devices
 *
 * Wakiyama lab. of SNCT
 *   Yasunori Endo
 *************************************************/

/* If you don't want to generate debug codes,
 * uncomment NDEBUG macro below.
 */
//#define NDEBUG

#include<stdio.h>
#include<stdlib.h>
#include<portaudio.h>


int main(int argc, char **argv){
  PaError error_code;
  PaDeviceIndex number_dev;
  PaDeviceInfo *devices;
  int i;

  printf("PortAudio practice - show devices\n");


  /* Initialize PortAudio */
  error_code = Pa_Initialize();

  
  /* count how many devices available */
  printf("Inspecting how many devices available.\n");
  number_dev = Pa_GetDeviceCount();

  #ifndef NDEBUG
  printf("There're %d device(s) available\n", number_dev);
  #endif


  /* Show devices info */
  for(i=0; i<number_dev; ++i){
    devices = Pa_GetDeviceInfo( (PaDeviceIndex)i );
    printf("#%d:\t%s\n", i, devices->name);
  }


  /* Terminate PortAudio */
  Pa_Terminate();


  return EXIT_SUCCESS;
}
