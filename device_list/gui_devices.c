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




/* Structure store information
 * about devices
 */
typedef struct{
  PaDeviceIndex id_dev;
  PaHostApiIndex id_api;

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
static char g_labelInfo[2048];
static int g_num_dev;
static Devinfo *g_devices = NULL;
