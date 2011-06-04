/***********************************************
 * PortAudio & GTK+ API practice
 *   Show all available devices with GTK+
 *
 * Wakiyama lab. of SNCT
 *   Yasunori Endo
 **********************************************/

/* If you don't want to generate debug codes,
 * uncomment NDEBUG macro below,
 * or compile with -DNDEBUG option.
 */
//#define NDEBUG

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<portaudio.h>
#include<gtk/gtk.h>




/* Important macros */
#define APP_NAME "GTK Show Audio Devices"
#define APP_VER "1.0"




/* Structure store information
 * about devices
 */
typedef struct{
  PaDeviceIndex id_dev;

  char *name_dev;
  char *name_api;

  int max_inputs;
  int max_outputs;

  double low_input_latency;
  double high_input_latency;
  double low_output_latency;
  double high_output_latency;

  double sample_rate;
} DevInfo;
#define SIZE_DEVINFO (sizeof(DevInfo))
#define SIZE_DEVINFO_P (sizeof(DevInfo *))




/* Global variables */
static char g_label_info[2048];
static int g_num_dev;
static DevInfo *g_devices = NULL;




/* Prepare list of devices */
static void get_dev_info(){
  int i;


  /* allocate g_devices */
  g_num_dev = Pa_GetDeviceCount();
  g_devices = (DevInfo *)malloc(SIZE_DEVINFO * g_num_dev);

  #ifndef NDEBUG
  printf("Total devices: %d\n", g_num_dev);
  #endif


  /* Store info into g_devices */
  for(i=0; i<g_num_dev; ++i){
    const PaDeviceInfo *info_dev = Pa_GetDeviceInfo(i);
    const PaHostApiInfo *info_api = Pa_GetHostApiInfo(info_dev->hostApi);
    int len_dev_name, len_api_name;

    len_dev_name = strlen(info_dev->name);
    g_devices[i].name_dev = (char *)malloc(len_dev_name + 1);
    #ifndef NDEBUG
    printf("device: %s len: %d\n", info_dev->name, len_dev_name);
    #endif

    len_api_name = strlen(info_api->name);
    g_devices[i].name_api = (char *)malloc(len_api_name + 1);
    #ifndef NDEBUG
    printf("api: %s len: %d\n", info_api->name, len_api_name);
    #endif

    g_devices[i].id_dev = i;

    g_devices[i].max_inputs = info_dev->maxInputChannels;
    g_devices[i].max_outputs = info_dev->maxOutputChannels;

    g_devices[i].high_input_latency = info_dev->defaultHighInputLatency * 1000.0;
    g_devices[i].low_input_latency = info_dev->defaultLowInputLatency * 1000.0;
    g_devices[i].high_output_latency = info_dev->defaultHighOutputLatency * 1000.0;
    g_devices[i].low_output_latency = info_dev->defaultLowOutputLatency * 1000.0;

    g_devices[i].sample_rate = info_dev->defaultSampleRate;

    strcpy(g_devices[i].name_dev, info_dev->name);
    strcpy(g_devices[i].name_api, info_api->name);
    #ifndef NDEBUG
    printf("\ncopied device:%s\ncopied api:%s\n",
	   g_devices[i].name_dev, g_devices[i].name_api);
    #endif
  }
}




/* Destroy info of devices */
static void destroy_dev_info(){
  int i;

  for(i=0; i<g_num_dev; ++i){
    free(g_devices[i].name_dev);
    free(g_devices[i].name_api);
  }

  free(g_devices);
}
