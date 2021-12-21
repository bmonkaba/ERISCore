#ifndef __AppWren__
#define __AppWren__
/**
 * @file appWren.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-12-13
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"
#include "wren.hpp"
#include "globaldefs.h"
/**
 * @brief Wren is a scripting language. This class is a proxy which mirrors the AppBaseClass into wren.
 * 
 * The use case is to allow scripting of apps
 * Why? with this, applications can be dynamically loaded and executed
 * Need another reason? script storage moves from the precious and limited microcontroller flash to the SD card
 * which gives effectivly limitless program storage space.
 * Beware: The available heap defines the upper boundary of VM size
 * 
 * From https://wren.io/:
 * "Wren is a small, fast, class-based concurrent scripting language. 
 * Think Smalltalk in a Lua-sized package with a dash of Erlang and wrapped up in a familiar, modern syntax. 
 * Wren is small. The VM implementation is under 4,000 semicolons"
 */
class AppWren:public AppBaseClass {
  public:
    AppWren():AppBaseClass(){
        strcpy(name,"AppWren");
        h_slot0 = 0;
        vm = 0;
        h_update = 0;
        h_updateRT = 0;
        h_onFocus = 0;
        h_onFocusLost = 0;
        h_onTouch = 0;
        h_onTouchDrag = 0;
        h_onTouchRelease = 0;
        h_onAnalog1 = 0;
        h_onAnalog2 = 0;
        h_onAnalog3 = 0;
        h_onAnalog4 = 0;
        h_messageHandler = 0;
        compileOnly = false;
        isPressed = false;
        show_active = false;
        usingImage = true;
        time_active = 0;
        surface_cache = NULL;
        vmConstructor();
        widget_width = 64;
        widget_height = 64;
        widget_origin_x = 320 - 64 - 5;
        widget_origin_y = 240 - 64 - 5;
    };

    ~AppWren(){
        wrenFreeVM(vm);
    };

    void vmConstructor();

    void loadScript(const char* script){
        const char* module = "main";
        Serial.println(F("M AppWren: Loading script into the VM"));
        isWrenResultOK(wrenInterpret(vm, module, script));
    }

    void MessageHandler(AppBaseClass *sender, const char *message);

    //in the AppBase class these are protected methods.. we need to expose them here publicly to set up the Wren callbacks.
    //void _updatePosition(){AppBaseClass::_updatePosition();}
    void setParent(AppBaseClass *parent){AppBaseClass::setParent(parent);}
    void setPosition(int16_t newOriginX, int16_t newOriginY){AppBaseClass::setPosition(newOriginX, newOriginY);}
    void setDimension(int16_t new_width, int16_t new_height){AppBaseClass::setDimension(new_width, height);}
    void setWidgetPosition(int16_t newOriginX, int16_t newOriginY){AppBaseClass::setWidgetPosition(newOriginX, newOriginY);}
    void setWidgetDimension(int16_t new_width, int16_t new_height){AppBaseClass::setWidgetDimension(new_width, new_height);}
    void getFocus(){AppBaseClass::getFocus();}
    void returnFocus(){AppBaseClass::returnFocus();}
    void requestPopUp(bool exclusive=false){AppBaseClass::requestPopUp(exclusive);}
    void releasePopUp(){AppBaseClass::releasePopUp();}
    void setRTPriority(uint16_t level);

    //and now we go a step further by integrating draw and data object methods directly into the AppWren class
    void setPixel(int16_t x, int16_t y, int16_t r, int16_t g, int16_t b){
        if (surface_cache && draw) draw->drawPixel(surface_cache,x,y,CL(r,g,b));
    }

  protected:
    void restartVM(){
        releaseWrenHandles();
        wrenFreeVM(vm);
        startVM();
    }

    void startVM();
    
    void releaseWrenHandles(){
        //release any existing handles
        if (h_slot0!=NULL) wrenReleaseHandle(vm, h_slot0);
        if (h_update!=NULL) wrenReleaseHandle(vm, h_update);
        if (h_updateRT!=NULL) wrenReleaseHandle(vm, h_updateRT);
        if (h_onFocus!=NULL) wrenReleaseHandle(vm, h_onFocus);
        if (h_onFocusLost!=NULL) wrenReleaseHandle(vm, h_onFocusLost);
        if (h_onTouch!=NULL) wrenReleaseHandle(vm, h_onTouch);
        if (h_onTouchDrag!=NULL) wrenReleaseHandle(vm, h_onTouchDrag);
        if (h_onTouchRelease!=NULL) wrenReleaseHandle(vm, h_onTouchRelease);
        if (h_onAnalog1!=NULL) wrenReleaseHandle(vm, h_onAnalog1);
        if (h_onAnalog2!=NULL) wrenReleaseHandle(vm, h_onAnalog2);
        if (h_onAnalog3!=NULL) wrenReleaseHandle(vm, h_onAnalog3);
        if (h_onAnalog4!=NULL) wrenReleaseHandle(vm, h_onAnalog4);
        if (h_messageHandler!=NULL) wrenReleaseHandle(vm, h_messageHandler);
    }

    void getWrenHandles(){
        wrenEnsureSlots(vm, 1);
        wrenGetVariable(vm, "main", "App", 0);
        //get the handles
        h_slot0 = wrenGetSlotHandle(vm, 0);
        h_update = wrenMakeCallHandle(vm, "update()");
        h_updateRT = wrenMakeCallHandle(vm, "updateRT()");
        
        h_onFocus = wrenMakeCallHandle(vm, "onFocus()");
        h_onFocusLost = wrenMakeCallHandle(vm, "onFocusLost()");
        h_onTouch = wrenMakeCallHandle(vm, "onTouch()");
        h_onTouchDrag = wrenMakeCallHandle(vm, "onTouchDrag()");
        h_onTouchRelease = wrenMakeCallHandle(vm, "onTouchRelease()");
        h_onAnalog1 = wrenMakeCallHandle(vm, "onAnalog1()");
        h_onAnalog2 = wrenMakeCallHandle(vm, "onAnalog2()");
        h_onAnalog3 = wrenMakeCallHandle(vm, "onAnalog3()");
        h_onAnalog4 = wrenMakeCallHandle(vm, "onAnalog4()");
        h_messageHandler = wrenMakeCallHandle(vm, "messageHandler()");
    }

    bool isWrenResultOK(WrenInterpretResult res){
        switch (res){
        case WREN_RESULT_COMPILE_ERROR:
            { Serial.printf("M WREN: Compile Error!\n"); } return false;
        case WREN_RESULT_RUNTIME_ERROR:
            //{ Serial.printf("M WREN: Runtime Error!\n"); } 
            return false;
        case WREN_RESULT_SUCCESS:
            return true;
        default:
            return false; //should never execute
        }
        
    }

    void update() override{
        if (h_update==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (!isWrenResultOK(wrenCall(vm,h_update))){

            }else{
                if(isPressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                        show_active = false;
                        time_active = 0;
                }
                if(usingImage){
                    if(!surface_cache){
                        //allocate space
                        surface_cache = new Surface(am->fastImgCacheSurfaceP, widget_width, widget_height);
                        if(!surface_cache){ 
                            Serial.println(F("M ERROR imgcache out of mem"));
                            Serial.flush();
                            delete(surface_cache);
                            return;
                        }
                    } else{
                        draw->bltMem(am->displaySurfaceP,surface_cache,x,y,AT_NONE);
                    }
                }else{
                    draw->fillRoundRect(x,y,w,h/2+3,3,am->data->read("UI_BUTTON_FILL_COLOR"));
                    draw->fillRoundRect(x,y+h/2,w,h/2,3,am->data->read("UI_BUTTON_SHADE_COLOR"));
                }
                if (show_active){
                    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_ACTIVE_BORDER_COLOR")); 
                } else{
                    draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_INACTIVE_BORDER_COLOR"));
                }
                
                if(!usingImage){
                    draw->setTextColor(am->data->read("UI_BUTTON_TEXT_COLOR"));
                    draw->setCursor(x+(w/2),y+(h/2),true);
                    draw->setFont(Arial_9);
                    //draw->print(text);
                }
            }
        }
        wrenCollectGarbage(vm);
    };    //called only when the app is active

    void updateRT() override{
        if (h_updateRT==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_updateRT))){
                
            };
        }
    }; //allways called even if app is not active
    void onFocus() override{
        if (h_onFocus==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onFocus))){
                
            };
        }
    };   //called when given focus
    void onFocusLost() override{
        if (h_onFocusLost==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onFocusLost))){
                
            };
        }
    }; //called when focus is taken
    void onTouch(uint16_t t_x, uint16_t t_y) override{
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            if (h_onTouch==0){ //if no script loaded
                return;
            } else{
                wrenEnsureSlots(vm, 1);
                wrenSetSlotHandle(vm, 0, h_slot0);//App
                if (isWrenResultOK(wrenCall(vm,h_onTouch))){
                    
                };
            }
        }
    };
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{
        if (h_onTouchDrag==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onTouchDrag))){
                
            };
        }
    };
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{
        if (h_onTouchRelease==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onTouchRelease))){
                
            };
        }
    };
    void onAnalog1(uint16_t uval, float fval) override{
        if (h_onAnalog1==0){
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onAnalog1))){
                
            };
        }
    };
    void onAnalog2(uint16_t uval, float fval) override{
        if (h_onAnalog2==0){
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onAnalog2))){
                
            };
        }
    };
    void onAnalog3(uint16_t uval, float fval) override{
        if (h_onAnalog3==0){
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onAnalog3))){
                
            };
        }
    };
    void onAnalog4(uint16_t uval, float fval) override{
        if (h_onAnalog4==0){
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (isWrenResultOK(wrenCall(vm,h_onAnalog4))){
                
            };
        }
    };
 protected:
    bool compileOnly;
    char img_filename[MAX_TEXT_LENGTH];
    char img_path[MAX_TEXT_LENGTH];
    Surface* surface_cache;
    bool isPressed;
    bool usingImage;
    bool imgloaded;
    elapsedMillis time_active;
    bool show_active;
 private:
    WrenVM* vm;
    WrenHandle* h_slot0;
    WrenHandle* h_update;
    WrenHandle* h_updateRT;
    WrenHandle* h_onFocus;
    WrenHandle* h_onFocusLost;
    WrenHandle* h_onTouch;
    WrenHandle* h_onTouchDrag;
    WrenHandle* h_onTouchRelease;
    WrenHandle* h_onAnalog1;
    WrenHandle* h_onAnalog2;
    WrenHandle* h_onAnalog3;
    WrenHandle* h_onAnalog4;
    WrenHandle* h_messageHandler;
};

#endif
