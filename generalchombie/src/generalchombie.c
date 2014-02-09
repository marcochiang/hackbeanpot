#include <pebble.h>
#include "common.h"

#define NUM_EVENTS  2
#define NUM_MENU_SECTIONS 1
#define NUM_FIRST_MENU_ITEMS 3

static Window *window;
static Window *appWindow;
static TextLayer *text_layer;

static SimpleMenuLayer *simple_menu_layer;
static SimpleMenuSection menu_sections[NUM_MENU_SECTIONS];
static SimpleMenuItem first_menu_items[NUM_FIRST_MENU_ITEMS];

//schedule window
static Window *window2;
static TextLayer *text_layer2;

static struct PblTm *curTime;
static int counter = 0;
static char strCnt[5];
static int timer_elapse = 0;

static AppTimer* update_timer = NULL;

typedef struct schedule{
    char* evt_name;  
    char* start_time;
    char* end_time;
    //these times are starting from 12:00 AM
    int start_time_sec;
    int elapsed_time_sec;
}schedule;

static schedule *appSched[NUM_EVENTS];


static void appWindow_load(Window *window) {
    for(int i = 0; i < NUM_EVENTS; i++) {
        first_menu_items[i] = (SimpleMenuItem){
            .title = appSched[i]->evt_name,
            .subtitle = concatStr(appSched[i]->start_time, appSched[i]->end_time),
          };
    }
  // Bind the menu items to the corresponding menu sections
  menu_sections[0] = (SimpleMenuSection){
    .num_items = NUM_EVENTS,
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
  text_layer_destroy(text_layer);
}

//SCHEUDLE WINDOW
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


//MAIN MENU
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "GC");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}


// static void window2_load(Window *window) {
//   Layer *window_layer = window_get_root_layer(window2);
//   GRect bounds = layer_get_bounds(window_layer);

// }

// static void window2_unload(Window *window) {
//   text_layer_destroy(text_layer2);
// }


void generate_events(void) {
  //malloc
  schedule *appSched = malloc(sizeof(*appSched)*NUM_EVENTS);
  //EVT1
  appSched[0].evt_name = "shower";
  appSched[0].start_time = "2012";
  appSched[0].end_time = "2022";
  appSched[0].start_time_sec = convertTime(appSched[0].start_time);
  //EVT2
  appSched[1].evt_name = "poop";
  appSched[1].start_time = "2100";
  appSched[1].end_time = "2110";
  appSched[1].start_time_sec = convertTime(appSched[1].start_time);
}

static void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
   //APP_LOG(APP_LOG_VERBOSE, "Time flies!");
  //counter
  //snprintf(strCnt, 5, "%d", counter++);

  //get_time(curTime); 

  //tick_time->SECOND_UNIT = 3;
  //text_layer_set_text(text_layer, strCnt);
  //display_time(tick_time);
}

void find_event_time(struct schedule *appSched) {
  //get current time
  struct tm *tm;
  time_t then;
  int curTime; //in seconds

  then = time(NULL);
  tm = localtime(&then);

  curTime = (tm->tm_hour)*3600 + (tm->tm_min)*60 + (tm->tm_sec); //in sec
  timer_elapse = curTime;
  //appSched->elapsed_time_sec = curTime;
}

void handle_timer(void* data) {

    snprintf(strCnt, 10, "%d", timer_elapse--);

    text_layer_set_text(text_layer, strCnt);
    update_timer = app_timer_register(1000, handle_timer, NULL);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  //tick_timer_service_subscribe(SECOND_UNIT, handle_second_tick);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;

  generate_events();

  update_timer = app_timer_register(1000, handle_timer, NULL);
  find_event_time(appSched[0]);

  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();

}
