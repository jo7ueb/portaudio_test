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
#include<string.h>
#include<portaudio.h>


int main(int argc, char **argv){
  PaError error_code;
  PaDeviceIndex number_dev;
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

  #ifndef NDEBUG
  printf("Size of PaDeviceInfo: %d\n", sizeof(PaDeviceInfo));
  #endif


  /* get devices info */
  for(i=0; i<number_dev; ++i){
    const PaDeviceInfo *info_dev  = Pa_GetDeviceInfo( (PaDeviceIndex)i );
    const PaHostApiInfo *info_api = Pa_GetHostApiInfo(info_dev->hostApi);

    printf("Device #%d\n", i);
    printf("Struct Version: %d\n", info_dev->structVersion);
    printf("Device Name: %s\n", info_dev->name);
    printf("Host API Name: %s\n", info_api->name);
    printf("Max Inputs: %d\n", info_dev->maxInputChannels);
    printf("Max Outputs: %d\n", info_dev->maxOutputChannels);
    printf("Default Sample Rates: %lf\n", info_dev->defaultSampleRate);

    printf("\n");
  }
 

  /* Terminate PortAudio */
  Pa_Terminate();


  return EXIT_SUCCESS;
}
