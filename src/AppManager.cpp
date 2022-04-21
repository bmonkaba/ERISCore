/**
 * @file AppManager.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */

//#define SERIAL_PRINT_APP_LOOP_TIME
#include <Arduino.h>

#include "HSI.h"
#include "Touch.h"
#include "AnalogInputs.h"
#include "ILI9341_t3_ERIS.h"
#include "ili9341_t3n_font_Arial.h"
#include "AppManager.h"
#include "Eris.h"

#include <pgmspace.h>
#include "FreeStack.h"

extern "C" uint32_t set_arm_clock(uint32_t frequency);

extern AudioDirector _ad;
extern SvcSerialCommandInterface sci;
extern SvcErisAudioParameterController apc;


Touch touch(CS_TOUCH);
ILI9341_t3_ERIS draw(TFT_CS, TFT_DC,TFT_RESET,TFT_MOSI,TFT_SCLK,TFT_MISO);
#ifdef USE_EXTMEM
uint16_t DMAMEM FB1[SCREEN_WIDTH * SCREEN_HEIGHT];
uint16_t EXTMEM imgCache[AM_IMG_CACHE_SIZE];
#else
uint16_t DMAMEM FB1[SCREEN_WIDTH * SCREEN_HEIGHT] __attribute__ ((aligned (16)));
uint16_t DMAMEM imgCache[AM_IMG_CACHE_SIZE] __attribute__ ((aligned (16)));
#endif
SvcDataDictionary _data; 

/**
 * @brief Construct a new App Manager:: App Manager object using a private constructor (lazy singleton pattern)
 * 
 */
FLASHMEM AppManager:: AppManager(){
  ad = 0;
  data = &_data;
  root = 0;
  active_app_id = 0;
  p_active_app = 0;
  next_id_assignment = 1; //id 0 is reserved
  monitor_dd_update_timer = 0;
  state = redraw_objects;
  //attach the global imgCache buffer to a surface
  memset(imgCache,0,sizeof(imgCache));
  p_fast_img_cache_surface = new Surface(&imgCache[0],AM_IMG_CACHE_SIZE);
  p_display_surface = new Surface(&FB1[0],SCREEN_WIDTH,SCREEN_HEIGHT);

  //init the app focus and app stacks
  memset(&app_focus_stack,0,sizeof(app_focus_stack));
  memset(&app_popup_stack,0,sizeof(app_popup_stack));
  app_focus_stack_index = 0;
  app_popup_stack_index = 0;
  exclusive_app_render = false;
  cycle_time_max = 0;
  //init the sd card
  if (!sd.begin(SdioConfig(FIFO_SDIO))){
    Serial.println(F("M AppManager: FATAL! SD Card Not Found "));
    sd.initErrorHalt(&Serial);
  } else {Serial.println(F("AppManager: SD Card FOUND"));}
  //init the display
  Serial.println(F("M AppManager: Config display"));
  draw.setPWMPin(TFT_LED_PWM); //library will control the backlight
  draw.setSD(&sd); //provide a cd pointer to the sd class
  draw.setFrameBuffer(FB1);
  draw.useFrameBuffer(true);
  
  //render.setPWMPin(TFT_LED_PWM); //library will control the backlight
  //render.setSD(&sd); //provide a cd pointer to the sd class
  //render.setFrameBuffer(FB1);
  //render.useFrameBuffer(true);
  animated_wallpaper.setSD(&sd);
  animated_wallpaper.setPath(UI_ANIMATED_WALLPAPER_PATH);
  Serial.println(F("M AppManager: Init display"));
  draw.begin();
  //render.begin();
  Serial.println(F("M AppManager: Init touch controller"));
  //init the touch screen
  touch.setCalibrationInputs(452,374,3830,3800); //inital cal values; app manager will monitor and update
  touch.setRotation(3);
  touch.begin();
  touch_state = 0;
  touched_elapsed =0;
  //enable cpu temp monitoring
  tempmon_init();
  //set the default template colors for the gui
  data->create(UI_BUTTON_FILL_COLOR,(int32_t)76);
  data->create(UI_BUTTON_SHADE_COLOR,(int32_t)37);
  data->create(UI_BUTTON_ACTIVE_BORDER_COLOR,(int32_t)23254);
  data->create(UI_BUTTON_INACTIVE_BORDER_COLOR,(int32_t)23244);
  data->create(UI_BUTTON_TEXT_COLOR,(int32_t)50713);
  data->create(UI_SLIDER_BORDER_COLOR,(int32_t)44374);
  data->create(UI_SLIDER_SHADE_COLOR,(int32_t)44344);
  data->create(UI_SLIDER_FILL_COLOR,(int32_t)1530);
  data->create(UI_SLIDER_TEXT_COLOR,(int32_t)39222);

  Serial.println(F("M AppManager: Contructor complete"));
  #ifdef ENABLE_ASYNC_SCREEN_UPDATES
  //render.updateScreenAsync(true);
  display_refresh_time = 0;
  #endif
  //log any crash reports
  Serial.println(F("M AppManager: Checking for crash report"));
  if (CrashReport){
      //Serial.print(CrashReport);
      FsFile f = sd.open("/crashreport.txt",O_CREAT|O_WRITE);
      f.print(CrashReport);
      f.flush();
      f.close();
  }
};

void FLASHMEM AppManager::setup(){
  //////////////////////////////////////////////////////////////////////////////////////
  //reset the i2c bus and config the external ADC
  Serial.println(F("M AppManager::setup Configuring Audio Hardware"));
  _ad.initAudioHardware();

  //I2CBusScan();  
  //Reset PSRAM clock to 132 Mhz

  /*
    CCM_CCGR7 |= CCM_CCGR7_FLEXSPI2(CCM_CCGR_OFF);
    CCM_CBCMR = (CCM_CBCMR & ~(CCM_CBCMR_FLEXSPI2_PODF_MASK | CCM_CBCMR_FLEXSPI2_CLK_SEL_MASK))
      | CCM_CBCMR_FLEXSPI2_PODF(4) | CCM_CBCMR_FLEXSPI2_CLK_SEL(2); // 528/5 = 132 MHz
    CCM_CCGR7 |= CCM_CCGR7_FLEXSPI2(CCM_CCGR_ON);

  */
}

/**
 * @brief AppManager update executes single update of the state machine
 * 
 */
void FLASHMEM AppManager::update(){
    elapsedMicros app_time;
    uint32_t heapTop;
    uint32_t drt;
    bool touch_updated;
    bool update_analog;
    AppBaseClass *node;
    bool monitor_update;
    int32_t te; 
    
    te = touched_elapsed;
    
    monitor_update = (monitor_dd_update_timer > APPMANAGER_MONITOR_DD_UPDATE_RATE_MSEC);
    cycle_time=0;
    drt = display_refresh_time; 
    touch_updated = false;
    update_analog = false;
    node = root;

    //requestArmSetClock(CPU_BASE_FREQ);

    if (node == 0){
      Serial.println(F("AppManager::update called without an application initalized"));
      return;
    }

    //update loop is directed by the state variable 
    switch(state){
      case redraw_background:
        app_time=0;
        if (!draw.busy() || 1){
            data->update(RENDER,(int32_t)0);
          if (animated_wallpaper.getNextFrameChunk()){
              requestArmSetClock(CPU_BOOST_MAX_FREQ);
              draw.bltSDAnimationFullScreen(&animated_wallpaper);
              data->update(AM_REDRAW_BG,(int32_t)app_time);
              requestArmSetClock(CPU_BASE_FREQ);
          } else Serial.println(F("M bad chunk"));
        }
        if ((data->read(RENDER) == 0) && animated_wallpaper.isFrameComplete()){
          data->update(RENDER,(int32_t)1);
          state = redraw_objects;
        }
        break;
        
      case redraw_wait://idle render time to give the screen refresh a head start
        if(exclusive_app_render && (draw.busy()==false || drt > 40)) state = redraw_objects;
        else if(!exclusive_app_render && (draw.busy()==false || drt > 24)) state = redraw_background;
        //execute on next state transition
        if (state != redraw_wait){
          //update analog inputs
          update_analog = analog.update();
          touch.update();
          touch_updated = true;    
          //note: this is  a good place for application manager housekeeping tasks where screen access is not required
          data->update(AM_AUDIO_CPU_MAX,(float32_t)AudioProcessorUsageMax());
          float32_t cpu;
          cpu = AudioProcessorUsage();
          data->update(AM_AUDIO_CPU,(float32_t)cpu);
          data->update(AM_AUDIO_MEM_MAX,(int32_t)AudioMemoryUsageMax());
          data->update(AM_AUDIO_MEM,(int32_t)AudioMemoryUsage());
        }
        break;

      case redraw_render:
        PXP_finish(); //make sure any PXP operations are complete
        //draw.updateScreenAsync(false);
        if(draw.busy()==false){
          draw.updateScreenAsyncFrom(&draw,false);
          data->update(RENDER_PERIOD,(int32_t)drt);
          data->update(RENDER,(int32_t)4);
          data->increment(RENDER_FRAME);
          app_time=0;
          state = redraw_wait;
          display_refresh_time = 0;
        }
        break;

      case redraw_objects:      
        node = root;
        if (exclusive_app_render == false){
          do{
              app_time=0;
              if (node->et_render_period > node->render_period_max) node->render_period_max = node->et_render_period;
              node->render(); //update active window
              node->render_cpu_time = app_time;
              if (node->render_cpu_time > node->render_cpu_time_max){
                  node->render_cpu_time_max = node->render_cpu_time;
              }
              //update the data dictionary
              node->et_render_period =0;
              node=node->next_app_node;//check next node
            }while(node !=NULL);
        };
        data->update(RENDER,(int32_t)2);
        data->increment(UPDATE_CALLS);
        state = redraw_popup;
        break;

      case redraw_popup:
        app_time=0;
        //draw any popups
        if (!app_popup_stack_index==0 && app_popup_stack[app_popup_stack_index-1] != 0){
          node = getApp(app_popup_stack[app_popup_stack_index-1]);
          if (node != 0){
            //Serial.println(F("AppManager::redraw_popup"));
            node->render();
          }
        }
        data->update(RENDER,(int32_t)3);
        state = redraw_render;     
        break;
    }
    //always call the apps updateRT function on every loop
    
    data->increment(RT_CALLS);
    node = root;
    bool isactive_child;
    do{
      app_time=0;
      if (node->update_priority > node->update_priority_counter){
        node->update_priority_counter++;
      }else{
        node->update_priority_counter = 0;
        if (node->et_update_period > node->update_period_max) node->update_period_max = node->et_update_period;
        node->update(); //real time update (always called)
        node->update_cpu_time = app_time;
        if (node->update_cpu_time > node->update_cpu_time_max) node->update_cpu_time_max = node->update_cpu_time;
        node->et_update_period =0;
      }
      isactive_child = false;
      if (node->id == active_app_id && !draw.busy()) {
          if (p_active_app != node){
            if (p_active_app != 0) p_active_app->onFocusLost();
            node->onFocus();
            p_active_app = node;
          }
      }
      if (node->parent_node!=NULL){if(node->parent_node->id == active_app_id){isactive_child = true;}}; //send event triggers to any child apps
      if (node->id == active_app_id || isactive_child) {
          //Serial.print("AppManager::updating active application");Serial.println(active_app_id);
          //trigger any events and then call the update function for this 'active' app or child node
          if (update_analog){
            data->update(AM_AN1,(int32_t)analog.readAN1());
            data->update(AM_AN2,(int32_t)analog.readAN2());
            data->update(AM_AN3,(int32_t)analog.readAN3());
            data->update(AM_AN4,(int32_t)analog.readAN4());
            
            node->onAnalog1(analog.readAN1(),analog.freadAN1());
            node->onAnalog2(analog.readAN2(),analog.freadAN2());
            node->onAnalog3(analog.readAN3(),analog.freadAN3());
            node->onAnalog4(analog.readAN4(),analog.freadAN4());
          }
          data->update("TOUCHED_ELAPSED",(int32_t)te);
          if (touch_updated == true && touch.touched()) {
            p = touch.getPoint();
            data->update("TOUCH_Z",(int32_t)p.z);
            
            if ((node->touch_state == 0) && (te > 250) ){
                node->onTouch(p.x, p.y);
                node->touch_state=1;
                touched_elapsed = 0;
            } else{
                node->onTouchDrag(p.x, p.y);
            }
          } else if (node->touch_state==1){
            node->touch_state=0;
            node->onTouchRelease(p.x, p.y);
          }
      }
      node->cycle_time = cycle_time;
      if (node->cycle_time > node->cycle_time_max){
          node->cycle_time_max = node->cycle_time;
      }
      node=node->next_app_node;//check next node
    }while(node !=NULL);

    if (cycle_time > cycle_time_max){
      cycle_time_max = cycle_time;
    } else{
      //delayMicroseconds((cycle_time_max - cycle_time)/100);
    }
    
    if(monitor_update){
      monitor_dd_update_timer = 0;
      data->update(AM_LOOP_TIME,(int32_t)cycle_time_max);
      data->update(AM_SERIAL_AVAIL,(int32_t)Serial.availableForWrite());
      //data->update("FRAME_PTR1",(int32_t)draw.getFrameAddress());
      //data->update("FRAME_PTR2",(int32_t)render.getFrameAddress());
      int32_t free_mem;
      void* htop;
      free_mem = 1000;
      cycle_time_max*=0.999;
      htop = malloc(1000);
      while (htop){
        //memset(htop,0x5A,1000);
        heapTop = (uint32_t) htop;
        free(htop);
        free_mem += 1000;
        htop = malloc(free_mem);
      }
      
      data->update(FREE_MEM,(int32_t)(free_mem - 1000));
      data->update(FREE_HEAP,(int32_t)(0x20280000 - heapTop));
      data->update(FREE_LOCAL,(int32_t)(0x2007F000 - (uint32_t)(&heapTop)));
      data->update(FREE_STACK,(int32_t)FreeStack());
       
      heapTop = 0;
      data->update(CPU_TEMP,(float32_t)tempmonGetTemp());
      AudioProcessorUsageMaxReset();
      AudioMemoryUsageMaxReset();    
    }
};
/**
 * @brief provides an interface for apps to request the SdFs object
 * 
 * @return SdFs* 
 */
SdFs* AppManager::getSD(){
  return &sd;
}

/**
 * @brief provides an interface for apps to request another app object by id
 * 
 * @param id 
 * @return AppBaseClass* 
 */
AppBaseClass* AppManager::getApp(uint16_t id){
    AppBaseClass *node = root;
    do{
      if (node->id == id){
        return node;
      }
      node=node->next_app_node;
    }while(node !=NULL);
    return NULL;
}
/**
 * @brief provides an interface for apps to request pop up focus with or without exclusive screen access
 * 
 * @param id 
 * @param exclusive 
 * @return true 
 * @return false 
 */
bool AppManager::requestPopUp(uint16_t id,bool exclusive){
  if (app_popup_stack_index == 8) return false;
  app_popup_stack[app_popup_stack_index] = id;
  app_popup_stack_index+=1;
  exclusive_app_render = exclusive;
  return true;
}
/**
 * @brief provides an interface for apps to release pop up focus
 * 
 * @return true 
 * @return false 
 */
bool AppManager::releasePopUp(){
  if (app_focus_stack_index == 0)return false;
  app_popup_stack_index-=1;
  app_popup_stack[app_popup_stack_index] = 0;
  exclusive_app_render = false;
  return true;
}
/**
 * @brief provides an interface for apps to request focus
 * 
 * @param id 
 * @return true 
 * @return false 
 */
bool AppManager::getFocus(uint16_t id){
  if (app_focus_stack_index == 8) return false;
  app_focus_stack[app_focus_stack_index++] = active_app_id;
  if (p_active_app!=NULL){
    p_active_app->onFocusLost();
  }
  p_active_app = getApp(id);
  p_active_app->onFocus();
  active_app_id=id;
  return true;
} 
/**
 * @brief provides an interface for apps to release focus
 * 
 * @return true 
 * @return false 
 */
bool AppManager::returnFocus(){
  if (app_focus_stack_index == 0) return false;
  getApp(active_app_id)->onFocusLost();
  active_app_id = app_focus_stack[--app_focus_stack_index];//load from the stack
  p_active_app = getApp(active_app_id);
  if(p_active_app!=NULL) p_active_app->onFocus();
  app_focus_stack[app_focus_stack_index] = 0;
  return true;
} 
/**
 * @brief provides an interface for apps to request the id of the active app
 * 
 * @return uint16_t 
 */
uint16_t AppManager::peekAppFocus(){return active_app_id;}
/**
 * @brief provides an interface for apps to request the active app object
 * 
 * @return AppBaseClass* 
 */
AppBaseClass* AppManager::getActiveApp(){ return p_active_app;}

/**
 * @brief provides an interface for apps to send messages to other apps
 * 
 * @param sender 
 * @param to_app 
 * @param message 
 * @return true 
 * @return false 
 */
bool AppManager::sendMessage(AppBaseClass *sender, const char *to_app, const char *message){
  // sends a message to the requested app
  AppBaseClass *node = root;
  do{
    if (node->isName(to_app)){
      node->messageHandler(sender,message);
      return true;
    }
    node=node->next_app_node;//check next node
  }while(node !=NULL);
  return false;
}


/**
 * @brief Get the App pointer By Name. Returns NULL if not found.
 * 
 * @param appName 
 * @return AppBaseClass* 
 */
AppBaseClass* AppManager::getAppByName(const char *appName){
  AppBaseClass *node = root;
  do{
    if (node->isName(appName)){
      return node;
    }
    node=node->next_app_node;//check next node
  }while(node !=NULL);
  return NULL;
}

/**
 * @brief prints out some stats in JSON format to the serial port
 * 
 */
void FASTRUN AppManager::printStats (){
  SvcSerialCommandInterface* sci;
  sci = (SvcSerialCommandInterface*)getAppByName("SCI"); //request the serial command interface
  if(sci!=NULL){
    AppBaseClass *node = root;
    if (node == NULL) return;
    do{
      if(sci->requestStartLZ4Message()==true){
        sci->print(F("STATS {\"APPS\":{\""));
        sci->print(node->name);
        sci->print(F("\":{"));
        sci->print(F("\"render_period_max\":"));sci->print(node->render_period_max);sci->print(",");
        sci->print(F("\"render_cpu_time_max\":"));sci->print(node->render_cpu_time_max);sci->print(",");
        sci->print(F("\"update_period_max\":"));sci->print(node->update_period_max);sci->print(",");
        sci->print(F("\"update_cpu_time_max\":"));sci->print(node->update_cpu_time_max);
        sci->println("}}}");
        sci->sendLZ4Message();
      }
      //clear the stats
      node->render_cpu_time_max = 0;
      node->update_cpu_time_max = 0;
      node->cycle_time_max = 0;
      node->render_period_max = 0;
      node->update_period_max = 0;
      node=node->next_app_node;//check next node
    }while(node !=NULL);
    if(sci->requestStartLZ4Message()==true){
      sci->print(F("STATS {\"APPMANAGER\":{"));
      sci->print(F("\"cycle_time\":"));sci->print(cycle_time);sci->print(F(","));
      sci->print(F("\"cycle_time_max\":"));sci->print(cycle_time_max);sci->print(F(","));
      sci->print(F("\"touch_state\":"));sci->print(touch_state);sci->print(F(","));
      sci->print(F("\"active_app_id\":"));sci->print(active_app_id);sci->print(F(","));
      sci->print(F("\"exclusive_app_render\":"));sci->print(exclusive_app_render);
      sci->println(F("}}"));
      sci->sendLZ4Message();
    }
    //clear the app manager stats
    cycle_time_max = 0;
  }
  return;
}
/**
 * @brief provides an interface for apps to register themselves with the AppManager \n
 * once registered the AppManager will include the app in the update loop 
 * @param app to be added to the update loop
 */
void AppManager::registerApp(AppBaseClass *app){
  //assign a unique id to the object
  app->id = next_id_assignment++;
  app->draw = &draw;
  app->am = this;
  app->ad = &_ad;
  app->sd = &sd;
  app->sci = &sci;
  if (root == 0) {root = app; return;}
  else{
    AppBaseClass *endNode = root;
    //add to the linked list
    while(endNode->next_app_node !=0){endNode=endNode->next_app_node;}
    endNode->next_app_node = app;
    app->previous_app_node = endNode;
  }
};


/**
 * @brief provides an interface for apps to unregister themselves with the AppManager \n
 * once unregistered the AppManager will no longer include the app in the update loop
 * 
 * @param app 
 * @return true if app was removed
 * @return false if app could not be found
 */
bool FLASHMEM AppManager::unregisterApp(AppBaseClass *app){
  if (root == 0) return false;
  else{
    AppBaseClass *endNode = root;
    //walk the linked list to find a match
    while(endNode != app && endNode != NULL){endNode=endNode->next_app_node;}
    if (endNode->next_app_node != app) return false;//no match; just return
    //remove the node
    endNode = endNode->next_app_node;
    if(endNode!=NULL){
      if(endNode->next_app_node!=NULL){
        endNode->next_app_node->previous_app_node = endNode;
      }
    }
  }
  return true;
};

/**
 * @brief provides an interface for apps to request cpu freq change
 * note: this is only a request. the actual frequency will vary based on the allowed 
 * freq range and current cpu temp
 * @param requested_cpu_frequency 
 * @return true 
 * @return false 
 */
bool AppManager::requestArmSetClock(uint32_t requested_cpu_frequency){
    //set cpu freq based on thermal performance
    if(tempmonGetTemp() > CPU_THERMAL_THROTTLE_TEMP){
      set_arm_clock(CPU_LOW_POWER_MODE_FREQ);//thermal guard (safer but not failsafe)
      data->update("F_CPU_ACTUAL",(int32_t)F_CPU_ACTUAL);
      return false;
    } else {
      //silently cap the requested freq 
      if(requested_cpu_frequency > CPU_BOOST_MAX_FREQ) requested_cpu_frequency = CPU_BOOST_MAX_FREQ;
      set_arm_clock(requested_cpu_frequency);
      data->update("F_CPU_ACTUAL",(int32_t)F_CPU_ACTUAL);
      return true;
    }
}


/**
 * @brief singleton AppManager object is initalized on first call to AppManager::getInstance()
 * 
 */
AppManager* AppManager::obj = 0; // or NULL, or nullptr in c++11

