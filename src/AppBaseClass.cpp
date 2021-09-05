/**
 * @file AppBaseClass.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-20
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppBaseClass.h"
#include "AppManager.h"

AudioDirector _ad;

AppBaseClass::AppBaseClass(){
  ad = &_ad;
  draw = 0;
  //Serial.println("AppBaseClass constructor called");
  has_focus=false;
  has_pop=false;
  parentNode=NULL;          //set by the parent
  nextAppicationNode=NULL;
  previousAppicationNode=NULL;
  touch_state = false;      //set by the application manager
  origin_x=0;
  origin_y=0;
  width=320;
  height=240;
  widget_origin_x=0;
  widget_origin_y=0;
  widget_width=320;
  widget_height=240;
  x=0;
  y=0;
  w=320;
  h=240;
  update_loop_time =0;
  update_loop_time_max =0;
  updateRT_loop_time =0;
  updateRT_loop_time_max =0;
  cycle_time =0;
  cycle_time_max =0;
  update_call_period =0;
  update_call_period_max =0;
  updateRT_call_period =0;
  updateRT_call_period_max =0;
  strcpy(name,"NONE");
  selfRegister();
}

void AppBaseClass::selfRegister(){AppManager::getInstance()->RegisterApp(this); }//self register on construction

void AppBaseClass::_updatePosition(){
  if(has_focus || has_pop){
    w = width;
    h = height;
    x = origin_x;
    y = origin_y;
  }else{
    w = widget_width;
    h = widget_height;
    x = widget_origin_x;
    y = widget_origin_y;
  }
  return;
}

void AppBaseClass::getFocus(){
    has_focus = true;
    AppManager::getInstance()->getFocus(this->id);
    _updatePosition();    
    return;
}

void AppBaseClass::returnFocus(){
  if(!has_focus) return;
  has_focus = false;
  AppManager::getInstance()->returnFocus();
  _updatePosition();
  return;
}  

void AppBaseClass::requestPopUp(){
    has_pop = true;
    //AppManager::getInstance()->requestPopUp(this->id);
    _updatePosition();    
    return;
}

void AppBaseClass::releasePopUp(){
  if(!has_pop) return;
  has_pop = false;
  //AppManager::getInstance()->releasePopUp();
    _updatePosition();    
    return;
}


