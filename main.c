#include <pebble.h>

static Window *window;
static TextLayer *hello_layer;
static char msg[100];
static int counter = 0;


void out_sent_handler(DictionaryIterator *sent, void *context) {
   // outgoing message was delivered -- do nothing
 }


 void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
   // outgoing message failed
   text_layer_set_text(hello_layer, "Error out!");
 }


 void in_received_handler(DictionaryIterator *received, void *context) {
   // incoming message received 
   // looks for key #0 in the incoming message
   int key = 0;
   Tuple *text_tuple = dict_find(received, key);
   //int key1 = 1;
   //Tuple *text_tuple1 = dict_find(received, key1);
   if (text_tuple) {
     if (text_tuple->value) {
       // put it in this global variable
       strcpy(msg, text_tuple->value->cstring);
     }
     else strcpy(msg, "no value!");
     
     text_layer_set_text(hello_layer, msg);
   }
//    else if(text_tuple1){
//      if (text_tuple1->value) {
//        // put it in this global variable
//        strcpy(msg, text_tuple1->value->cstring);
//      }
//      else strcpy(msg, "no value11111111!!");
     
//      text_layer_set_text(hello_layer, msg);
//    }
   else {
     text_layer_set_text(hello_layer, "no message");
   }
   
 }

 void in_dropped_handler(AppMessageResult reason, void *context) {
   // incoming message dropped
   text_layer_set_text(hello_layer, "Error in!");
 }





/* This is called when the up button is clicked */
void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (counter == 1){
      text_layer_set_text(hello_layer, "Change degree!");
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "a");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send();   
    }
  else if(counter == 2){
    text_layer_set_text(hello_layer, "Can't do anything!");
    DictionaryIterator *iter1;
    app_message_outbox_begin(&iter1);
    int key = 0;
    // send the message "hello?" to the phone, using key #1
    Tuplet value1 = TupletCString(key, "d");
    dict_write_tuplet(iter1, &value1);
    app_message_outbox_send();   
  }
  else if(counter == 3){
    text_layer_set_text(hello_layer, "Flash color Green!");
    DictionaryIterator *iter1;
    app_message_outbox_begin(&iter1);
    int key = 0;
    // send the message "hello?" to the phone, using key #1
    Tuplet value1 = TupletCString(key, "g");
    dict_write_tuplet(iter1, &value1);
    app_message_outbox_send();   
  } 
  else if(counter == 4){
    text_layer_set_text(hello_layer, "Threshold value up!");
    DictionaryIterator *iter1;
    app_message_outbox_begin(&iter1);
    int key = 0;
    // send the message "hello?" to the phone, using key #1
    Tuplet value1 = TupletCString(key, "j");
    dict_write_tuplet(iter1, &value1);
    app_message_outbox_send();   
  }
  else if(counter == 5){
    text_layer_set_text(hello_layer, "Display avg temp");
    DictionaryIterator *iter1;
    app_message_outbox_begin(&iter1);
    int key = 0;
    // send the message "hello?" to the phone, using key #1
    Tuplet value1 = TupletCString(key, "m");
    dict_write_tuplet(iter1, &value1);
    app_message_outbox_send();   
  }  
  else if(counter == 6){
    text_layer_set_text(hello_layer, "Quit standby mode");
    DictionaryIterator *iter1;
    app_message_outbox_begin(&iter1);
    int key = 0;
    // send the message "hello?" to the phone, using key #1
    Tuplet value1 = TupletCString(key, "q");
    dict_write_tuplet(iter1, &value1);
    app_message_outbox_send();   
  }  
}



/* This is called when the select button is clicked */
void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (counter == 1){
    text_layer_set_text(hello_layer, "print temperature!");
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    int key = 0;
    // send the message "hello?" to the phone, using key #0
    Tuplet value = TupletCString(key, "b");
    dict_write_tuplet(iter, &value);
    app_message_outbox_send();     
  }
  else if (counter == 2){
    text_layer_set_text(hello_layer, "Display PM 2.5 info!");
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    int key = 0;
    // send the message "hello?" to the phone, using key #0
    Tuplet value = TupletCString(key, "e");
    dict_write_tuplet(iter, &value);
    app_message_outbox_send(); 
  }
  else if (counter == 3){
    text_layer_set_text(hello_layer, "Flash color Purple!");
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    int key = 0;
    // send the message "hello?" to the phone, using key #0
    Tuplet value = TupletCString(key, "h");
    dict_write_tuplet(iter, &value);
    app_message_outbox_send(); 
  }
  else if (counter == 4){
    text_layer_set_text(hello_layer, "Threashold value down!");
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    int key = 0;
    // send the message "hello?" to the phone, using key #0
    Tuplet value = TupletCString(key, "k");
    dict_write_tuplet(iter, &value);
    app_message_outbox_send(); 
  }
  else if (counter == 5){
    text_layer_set_text(hello_layer, "Display high/low temp!");
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    int key = 0;
    // send the message "hello?" to the phone, using key #0
    Tuplet value = TupletCString(key, "n");
    dict_write_tuplet(iter, &value);
    app_message_outbox_send(); 
  }
  else if(counter == 6) {
    text_layer_set_text(hello_layer, "enter standby mode!");
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    int key = 0;
    // send the message "hello?" to the phone, using key #0
    Tuplet value = TupletCString(key, "p");
    dict_write_tuplet(iter, &value);
    app_message_outbox_send(); 
  }
}

/* This is called when the down button is clicked */
void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  counter++;
    if (counter > 7){
      counter = 1;
    }
    if (counter == 1){
      text_layer_set_text(hello_layer, "Display Temperature");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "r");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send(); 
    }
  else if (counter == 2){
      text_layer_set_text(hello_layer, "Display PM 2.5 info");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "c");
      //dict_write_tuplet(iter1, &value1);
      //app_message_outbox_send(); 
    }
  else if (counter == 3){
      text_layer_set_text(hello_layer, "Next Page.");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "f");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send(); 
    }
  else if (counter == 4){
      text_layer_set_text(hello_layer, "Next Page.");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "i");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send(); 
    }
  else if (counter == 5){
      text_layer_set_text(hello_layer, "Next Page.");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "l");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send(); 
    }
  else if(counter == 6){
      text_layer_set_text(hello_layer, "Next Page.");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "o");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send(); 
    }
  else if (counter == 7){
      text_layer_set_text(hello_layer, "Next Page.");  
      DictionaryIterator *iter1;
      app_message_outbox_begin(&iter1);
      int key = 0;
      // send the message "hello?" to the phone, using key #1
      Tuplet value1 = TupletCString(key, "s");
      dict_write_tuplet(iter1, &value1);
      app_message_outbox_send(); 
  }
}




/* this registers the appropriate function to the appropriate button */
void config_provider(void *context) {
   window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
   window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
   window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  hello_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(hello_layer, "Welcome!");
  text_layer_set_text_alignment(hello_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(hello_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(hello_layer);
}

static void init(void) {
  window = window_create();
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });

  // need this for adding the listener
  window_set_click_config_provider(window, config_provider);
  
  // for registering AppMessage handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_sent(out_sent_handler);
  app_message_register_outbox_failed(out_failed_handler);
  const uint32_t inbound_size = 64;
  const uint32_t outbound_size = 64;
  app_message_open(inbound_size, outbound_size);
  
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


