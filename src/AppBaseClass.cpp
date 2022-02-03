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



AppBaseClass::AppBaseClass(){
  //ad = &_ad;
  draw = 0;
  //Serial.println("AppBaseClass constructor called");
  has_focus=false;
  has_pop=false;
  parent_node=NULL;          //set by the parent
  next_app_node=NULL;
  previous_app_node=NULL;
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
  render_cpu_time =0;
  render_cpu_time_max =0;
  update_cpu_time =0;
  update_cpu_time_max =0;
  cycle_time =0;
  cycle_time_max =0;
  et_render_period =0;
  render_period_max =0;
  et_update_period =0;
  update_period_max =0;
  update_priority = 65535;
  update_priority_counter = 0;
  strcpy(name,"NONE");
  selfRegister();
}

void AppBaseClass::selfRegister(){AppManager::getInstance()->registerApp(this); }//self register on construction

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

void AppBaseClass::requestPopUp(bool exclusive){
    has_pop = true;
    AppManager::getInstance()->requestPopUp(this->id,exclusive);
    _updatePosition();    
    return;
}

void AppBaseClass::releasePopUp(){
  if(!has_pop) return;
  has_pop = false;
  AppManager::getInstance()->releasePopUp();
    _updatePosition();    
    return;
}


void AppBaseClass::setUpdatePriority(uint16_t level){
    update_priority = level;
    return;
}



