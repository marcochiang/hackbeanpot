/*
 *  Leaf connect
 *  Erik Risinger
 *  11/2013
 */

#include <pebble.h>
#include "common.h"

#define NUM_EVENTS  3

#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

#define TRUE 1
#define FALSE 0

static Window *window;
static Window *appWindow;

static TextLayer *text_layer;
static TextLayer *first_name_layer;
static TextLayer *last_name_layer;

static SimpleMenuLayer *simple_menu_layer;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem first_menu_items[NUM_FIRST_MENU_ITEMS];

static bool snooze = TRUE;

static AppTimer* update_timer = NULL;

typedef struct schedule{
    char* evt_name;  
    char* start_time;
    char* end_time;
    //these times are starting from 12:00 AM
    int end_time_sec;
    int elapsed_time_sec;
}schedule;

static schedule appSched[NUM_EVENTS];
static char strCnt[5];
static int cur_index = 0;

void find_event_time(struct schedule appSched[NUM_EVENTS], int index) {
  //get current time
  struct tm *tm;
  time_t then;
  int curTime; //in seconds

  then = time(NULL);
  tm = localtime(&then);

  curTime = (tm->tm_hour)*3600 + (tm->tm_min)*60 + (tm->tm_sec); //in sec
  appSched[index].elapsed_time_sec = appSched[index].end_time_sec - curTime;
}

void handle_timer(void* data) {
    if(appSched[cur_index].elapsed_time_sec <= 0) {
      cur_index++;

      snooze=FALSE;
      
      find_event_time(appSched, cur_index);
      //vibrate
    }

    if(snooze){
       vibes_short_pulse();
    }

    appSched[cur_index].elapsed_time_sec = appSched[cur_index].elapsed_time_sec - 1;
    snprintf(strCnt, 5, "%d", appSched[cur_index].elapsed_time_sec);

    text_layer_set_text(first_name_layer, appSched[cur_index].evt_name);
    text_layer_set_text(last_name_layer, "");
    text_layer_set_text(text_layer, strCnt);

    update_timer = app_timer_register(1000, handle_timer, NULL);
}


void generate_events(void) {
  //malloc
  ///*appSched = malloc(sizeof(*appSched)*NUM_EVENTS);
  //EVT1
  appSched[0].evt_name = "shower";
  appSched[0].start_time = "0001";
  appSched[0].end_time = "0028";
  appSched[0].end_time_sec = convertTime(appSched[0].end_time);
  //EVT2
  appSched[1].evt_name = "poop";
  appSched[1].start_time = "0011";
  appSched[1].end_time = "0029";
  appSched[1].end_time_sec = convertTime(appSched[1].end_time);
  //EVT3
  appSched[2].evt_name = "eat";
  appSched[2].start_time = "0012";
  appSched[2].end_time = "0030";
  appSched[2].end_time_sec = convertTime(appSched[2].end_time);
}


static void appWindow_load(Window *window) {
  // int num_a_items = 0;
  //   first_menu_items[num_a_items++] = (SimpleMenuItem){
  //   // You should give each menu item a title and callback
  //   .title = "10m Shower",
  // };
  // // The menu items appear in the order saved in the menu items array
  // first_menu_items[num_a_items++] = (SimpleMenuItem){
  //   .title = "2m Brush Teeth",
  // };
  // first_menu_items[num_a_items++] = (SimpleMenuItem){
  //   .title = "5m Breakfast",
  // };
      for(int i = 0; i < NUM_EVENTS; i++) {
        first_menu_items[i] = (SimpleMenuItem){
            .title = appSched[i].evt_name,
            .subtitle = concatStr(appSched[i].start_time, appSched[i].end_time),
          };
    }


  // Bind the menu items to the corresponding menu sections
  menu_sections[0] = (SimpleMenuSection){
    .num_items = NUM_FIRST_MENU_ITEMS,
    .items = first_menu_items,
  };

  Layer *appWindow_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(appWindow_layer);

  // Initialize the simple menu layer
  simple_menu_layer = simple_menu_layer_create(bounds, window, menu_sections, NUM_MENU_SECTIONS, NULL);

  // Add it to the window for display
  layer_add_child(appWindow_layer, simple_menu_layer_get_layer(simple_menu_layer));

}

static void appWindow_unload(Window *window) {
  simple_menu_layer_destroy(simple_menu_layer);
}

/* handlers */

//dummy handlers
static void select_click_handler(ClickRecognizerRef recognizer, void *context) { 
  appWindow = window_create();
  //window_set_click_config_provider(appWindow, click_config_provider);
  window_set_window_handlers(appWindow, (WindowHandlers) {
    .load = appWindow_load,
    .unload = appWindow_unload,
  });
  const bool animated = true;
  window_stack_push(appWindow, animated);

 }
static void up_click_handler(ClickRecognizerRef recognizer, void *context) { }
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  snooze = TRUE;
}
static void handle_accel(AccelData *accel_data, uint32_t num_samples) { }


static void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);

    /* set up name text layers */
    int first_origin_x = 10;
    int first_width = bounds.size.w - first_origin_x;
    int first_height = 36;
    int first_origin_y = bounds.size.h / 2 - first_height;
    
    int last_width = bounds.size.w - 10;
    int last_origin_x = 0;
    int last_height = 32;
    int last_origin_y = bounds.size.h / 2 - 10;
  

    //init first name layer; set: background color, text color, text, alignment, font; add to window
    first_name_layer = text_layer_create((GRect){ .origin = { first_origin_x, first_origin_y }, .size = { first_width, first_height } });
    text_layer_set_background_color(first_name_layer, GColorClear);
    text_layer_set_text_color(first_name_layer, GColorWhite);
    text_layer_set_text(first_name_layer, "General");
    text_layer_set_text_alignment(first_name_layer, GTextAlignmentLeft);
    text_layer_set_font(first_name_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
    layer_add_child(window_layer, text_layer_get_layer(first_name_layer));
  
    //init last name layer; set: background color, text color, text, alignment, font; add to window
    last_name_layer = text_layer_create((GRect){ .origin = { last_origin_x, last_origin_y}, .size = { last_width, last_height } } );
    text_layer_set_background_color(last_name_layer, GColorClear);
    text_layer_set_text_color(last_name_layer, GColorWhite);
    text_layer_set_text(last_name_layer, "Chombie");
    text_layer_set_text_alignment(last_name_layer, GTextAlignmentRight);
    text_layer_set_font(last_name_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
    layer_add_child(window_layer, text_layer_get_layer(last_name_layer));

    text_layer = text_layer_create((GRect) { .origin = { 0, 120 }, .size = { bounds.size.w, 20 } });
    text_layer_set_text(text_layer, "00 : 00: 00");
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
    text_layer_destroy(text_layer);
    text_layer_destroy(first_name_layer);
    text_layer_destroy(last_name_layer);
}

static void init(void) {
    window = window_create();
    window_set_background_color(window, GColorBlack);
    window_set_click_config_provider(window, click_config_provider);
    window_set_window_handlers(window, (WindowHandlers) {
      .load = window_load,
      .unload = window_unload,
    });
    const bool animated = true;

    generate_events();

    find_event_time(appSched, cur_index);
    update_timer = app_timer_register(1000, handle_timer, NULL);


    window_stack_push(window, animated);

}



static void deinit(void) {
  app_timer_cancel(update_timer);
    window_destroy(window);
    // window_destroy(appWindow);
}

int main(void) {
    init();
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);
  
  app_event_loop();
  deinit();
}
