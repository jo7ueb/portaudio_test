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
  PaHostApiIndex number_host;
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
  printf("Size of PaDeviceInfo: %lu\n\n", sizeof(PaDeviceInfo));
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
    printf("Default Sample Rates[Hz]: %lf\n", info_dev->defaultSampleRate);

    if(info_dev->maxInputChannels > 0){
      printf("Low Input Latency[ms]: %lf\n",
	     info_dev->defaultLowInputLatency * 1000.0);
      printf("High Input Latency[ms]: %lf\n",
	     info_dev->defaultHighInputLatency * 1000.0);
    }

    if(info_dev->maxOutputChannels > 0){
      printf("Low Output Latency[ms]: %lf\n",
	     info_dev->defaultLowOutputLatency * 1000.0);
      printf("High Output Latency[ms]: %lf\n",
	     info_dev->defaultHighOutputLatency * 1000.0);
    }

    printf("\n");
  }
 

  /* get host API info */
  number_host = Pa_GetHostApiCount();

  #ifndef NDEBUG
  printf("There're %d host API(s) available\n\n", number_host);
  #endif

  for(i=0; i<number_host; ++i){
    const PaHostApiInfo *info = Pa_GetHostApiInfo(i);
 
    printf("API #%d\n", i);
    printf("API Name: %s\n", info->name);
    printf("Device Count: %d\n", info->deviceCount);
    printf("Default Input Device: %d\n", info->defaultInputDevice);
    printf("Default Output Device: %d\n", info->defaultOutputDevice);

    printf("\n");
  }


  /* Terminate PortAudio */
  Pa_Terminate();


  return EXIT_SUCCESS;
}
