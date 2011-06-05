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




/* Generate label text for g_label_info */
static void set_label_text(int id_dev){
  #ifndef NDEBUG
  printf("set_label_text() called with id_dev = %d\n", id_dev);
  #endif

  snprintf(g_label_info, 2047,
	   "Device ID: %d\n\
Device Name: %s\n\
Host API Name: %s\n\
Max Input Channels: %d\n\
Max OutputChannels: %d\n\
Default Sample Rate: %lf [Hz]\n\
Default High Input Latency: %lf [ms]\n\
Default Low Input Latency: %lf [ms]\n\
Default High Output Latency: %lf [ms]\n\
Default Low Output Latency: %lf [ms]\n",
	   id_dev,
	   g_devices[id_dev].name_dev,
	   g_devices[id_dev].name_api,
	   g_devices[id_dev].max_inputs,
	   g_devices[id_dev].max_outputs,
	   g_devices[id_dev].sample_rate,
	   g_devices[id_dev].high_input_latency,
	   g_devices[id_dev].low_input_latency,
	   g_devices[id_dev].high_output_latency,
	   g_devices[id_dev].low_output_latency);
}




/* callback for combo_select */
static void cb_combo_changed(GtkComboBox *cbox, GtkLabel *label){
  int nr = gtk_combo_box_get_active(cbox);

  #ifndef NDEBUG
  printf("cb_combo_changed() received %d.\n\n", nr);
  #endif

  set_label_text(nr);
  gtk_label_set_text(label, g_label_info);
}




/* callback for button_about */
static void cb_about_dialog(GtkButton *button, gpointer data){
  GtkWidget *dialog_about;

  dialog_about = gtk_about_dialog_new();
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(dialog_about), APP_NAME);
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog_about), APP_VER);
  gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog_about),
			       "http://twitter.com/jo7ueb");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog_about),
				"This program lists all available audio devices.");
  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog_about),
				 "Copyright (C) 2011 Yasunori Endo \
All Rights Reserved");

  gtk_widget_show_all(dialog_about);
}




/* main function */
int main(int argc, char **argv){
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *hbox_upper, *hbox_lower;
  GtkWidget *label_info, *label_prompt;
  GtkWidget *combo_select;
  GtkWidget *button_about, *button_close;

  int i;
  PaDeviceIndex default_input;


  /* Initialize API*/
  gtk_init(&argc, &argv);
  Pa_Initialize();

  get_dev_info();
  default_input = Pa_GetDefaultInputDevice();
  set_label_text(default_input);


  /* setting up for GtkWidgets */
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request(window, 500, 400);
  gtk_window_set_title(GTK_WINDOW(window), APP_NAME);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);

  vbox = gtk_vbox_new(FALSE, 5);
  hbox_upper = gtk_hbox_new(FALSE, 0);
  hbox_lower = gtk_hbox_new(TRUE, 0);

  label_info = gtk_label_new(g_label_info);
  label_prompt = gtk_label_new("Choose device ID:");

  button_about = gtk_button_new_with_label("About");
  button_close = gtk_button_new_with_label("Quit");

  combo_select = gtk_combo_box_new_text();
  for(i=0; i<g_num_dev; ++i){
    char number[3];
    snprintf(number, 2, "%d", i);

    gtk_combo_box_append_text(GTK_COMBO_BOX(combo_select), number);
  }
  gtk_combo_box_set_active(GTK_COMBO_BOX(combo_select), default_input);


  /* packing widgets into window */
  gtk_container_add(GTK_CONTAINER(window), vbox);

  gtk_box_pack_start(GTK_BOX(hbox_upper), label_prompt, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox_upper), combo_select, FALSE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(hbox_lower), button_about, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox_lower), button_close, TRUE, TRUE, 0);

  gtk_box_pack_start(GTK_BOX(vbox), hbox_upper, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), label_info, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), hbox_lower, FALSE, TRUE, 0);


  /* connecting to signals */
  g_signal_connect(G_OBJECT(window), "delete-event",
		   G_CALLBACK(gtk_main_quit), NULL);

  g_signal_connect(G_OBJECT(combo_select), "changed",
		   G_CALLBACK(cb_combo_changed), label_info);

  g_signal_connect(G_OBJECT(button_about), "clicked",
		   G_CALLBACK(cb_about_dialog), NULL);

  g_signal_connect(G_OBJECT(button_close), "clicked",
		   G_CALLBACK(gtk_main_quit), NULL);

  
  #ifndef NDEBUG
  printf("%s\n", g_label_info);
  #endif

  gtk_widget_show_all(window);
  gtk_main();


  /* Terminate this program */
  destroy_dev_info();
  Pa_Terminate();
  return EXIT_SUCCESS;
}
