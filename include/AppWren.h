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
        save_module = false;
        isPressed = false;
        show_active = false;
        usingImage = true;
        time_active = 0;
        surface_cache = NULL;
        module_load_buffer = NULL;
        vmConstructor();
        widget_width = 64;
        widget_height = 64;
        widget_origin_x = 320 - 64 - 5;
        widget_origin_y = 240 - 64 - 5;
        //surface_mempool = (uint16_t*)malloc(sizeof(uint16_t)*128*128);
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
    void setPosition(int16_t newOriginX, int16_t newOriginY);
    void setDimension(int16_t new_width, int16_t new_height);
    void setWidgetPosition(int16_t newOriginX, int16_t newOriginY);
    void setWidgetDimension(int16_t new_width, int16_t new_height);
    void getFocus(){AppBaseClass::getFocus();}
    void returnFocus(){AppBaseClass::returnFocus();}
    void requestPopUp(bool exclusive=false){AppBaseClass::requestPopUp(exclusive);}
    void releasePopUp(){AppBaseClass::releasePopUp();}
    void setRTPriority(uint16_t level);

    //interface for requesting a module be loaded
    const char* loadModuleSource(const char * name);
    void freeModuleSource();
    //provide an interface for wren callbacks to request the draw object
    ILI9341_t3_ERIS* getDraw(){return draw;}
    AudioDirector* getAudioDirector(){return ad;} 
    //and now we go a step further by integrating draw and data object methods directly into the AppWren class
    void setPixel(int16_t x, int16_t y, int16_t r, int16_t g, int16_t b){
        if (draw){
            if (has_pop){
                draw->ILI9341_t3n::drawPixel(x,y,CL((uint16_t)r,(uint16_t)g,(uint16_t)b));
            }else{
                dynamicSurfaceManager();
                if (!surface_cache) return;
                if (x >= surface_cache->getWidth()) return;
                if (y >= surface_cache->getHeight()) return;
                if (x < 0 || y < 0) return;
                draw->drawPixel(surface_cache,x,y,CL(r,g,b));
            }
        } 
    }
    
    uint16_t getPixel(int16_t x, int16_t y){
        if (draw){
            if (has_pop){
                //return draw->readPixel(x,y);
                return ILI9341_YELLOW;//draw->ILI9341_t3n::readPixel(x,y);
            }else{
                if (!surface_cache) return ILI9341_MAGENTA;
                if (x >= surface_cache->getWidth()) return ILI9341_MAGENTA;
                if (y >= surface_cache->getHeight()) return ILI9341_MAGENTA;
                if (x < 0 || y < 0) return ILI9341_MAGENTA;
                return draw->readSurfacePixel(surface_cache,x,y);
            }
        } 
    }
    void drawLine(int16_t start_x, int16_t start_y,int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b){
        
        if ((start_x == end_x) && (start_y == end_y)){
            if (draw){
                if (has_pop){
                    draw->ILI9341_t3n::drawPixel(start_x,start_y,CL((uint16_t)r,(uint16_t)g,(uint16_t)b));
                } else{
                    dynamicSurfaceManager();
                    if (!surface_cache) return;
                    if (start_x >= surface_cache->getWidth()) start_x = surface_cache->getWidth()-1;
                    if (start_y >= surface_cache->getHeight()) start_y = surface_cache->getHeight()-1;
                    if (start_x < 0 ) start_x = 0;
                    if (start_y < 0 ) start_y = 0;

                    if (end_x >= surface_cache->getWidth()) end_x = surface_cache->getWidth()-1;
                    if (end_y >= surface_cache->getHeight()) end_y = surface_cache->getHeight()-1;
                    if (end_x < 0 ) end_x = 0;
                    if (end_y < 0 ) end_y = 0;
                    draw->drawPixel(surface_cache,start_x,start_y,CL(r,g,b));
                }
            }
            return;
        }else if (draw){
            if (has_pop){
                draw->ILI9341_t3n::drawLine(start_x, start_y, end_x, end_y, CL(r,g,b));
            }else{
                dynamicSurfaceManager();
                if (!surface_cache) return;
                if (start_x >= surface_cache->getWidth()) start_x = surface_cache->getWidth()-1;
                if (start_y >= surface_cache->getHeight()) start_y = surface_cache->getHeight()-1;
                if (start_x < 0 ) start_x = 0;
                if (start_y < 0 ) start_y = 0;

                if (end_x >= surface_cache->getWidth()) end_x = surface_cache->getWidth()-1;
                if (end_y >= surface_cache->getHeight()) end_y = surface_cache->getHeight()-1;
                if (end_x < 0 ) end_x = 0;
                if (end_y < 0 ) end_y = 0;
                draw->drawSurfaceLine(surface_cache, start_x, start_y, end_x, end_y, CL(r,g,b));
            }
            return;   
        }
    }

    void drawFill(int16_t r, int16_t g, int16_t b){
        dynamicSurfaceManager();
        if (draw){
            if (has_pop){
                draw->ILI9341_t3n::fillScreen(CL(r,g,b));
            } else{
                if (!surface_cache) return;
                draw->fillSurface(surface_cache,CL(r,g,b));
            }
        }
        return;
    }

  protected:
    void restartVM(){
        if(has_pop){
            am->releasePopUp();
            has_pop = false;
        }
        releaseWrenHandles();
        wrenFreeVM(vm);
        if(surface_cache){
            //delete(surface_cache); TODO 
            //surface_cache = NULL;
        }
        startVM();
    }

    void startVM();
    void releaseWrenHandles();
    void getWrenHandles();
    bool isWrenResultOK(WrenInterpretResult res){
        switch (res){
            case WREN_RESULT_COMPILE_ERROR:
                Serial.printf("M WREN: Compile Error!\n");
                return false;
            case WREN_RESULT_RUNTIME_ERROR:
                //Serial.printf("M WREN: Runtime Error!\n");
                return false;
            case WREN_RESULT_SUCCESS:
                return true;
            default:
                return false; //should never execute
        } 
    }

    bool dynamicSurfaceManager();

    void update() override;//called only when the app is active

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
                wrenEnsureSlots(vm, 3);
                wrenSetSlotHandle(vm, 0, h_slot0);//App
                wrenSetSlotDouble(vm, 0, t_x);//param
                wrenSetSlotDouble(vm, 0, t_y);//param
                if (isWrenResultOK(wrenCall(vm,h_onTouch))){
                    
                };
            }
        }
    };
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{
        if (h_onTouchDrag==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 3);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 0, t_x);//param
            wrenSetSlotDouble(vm, 0, t_y);//param
            if (isWrenResultOK(wrenCall(vm,h_onTouchDrag))){
                
            };
        }
    };
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{
        if (h_onTouchRelease==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 3);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 0, t_x);//param
            wrenSetSlotDouble(vm, 0, t_y);//param
            if (isWrenResultOK(wrenCall(vm,h_onTouchRelease))){
                
            };
        }
    };
    void onAnalog1(uint16_t uval, float fval) override{
        if (h_onAnalog1==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (isWrenResultOK(wrenCall(vm,h_onAnalog1))){
                
            };
        }
    };
    void onAnalog2(uint16_t uval, float fval) override{
        if (h_onAnalog2==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (isWrenResultOK(wrenCall(vm,h_onAnalog2))){
                
            };
        }
    };
    void onAnalog3(uint16_t uval, float fval) override{
        if (h_onAnalog3==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (isWrenResultOK(wrenCall(vm,h_onAnalog3))){
                
            };
        }
    };
    void onAnalog4(uint16_t uval, float fval) override{
        if (h_onAnalog4==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (isWrenResultOK(wrenCall(vm,h_onAnalog4))){
                
            };
        }
    };
 protected:
    bool compileOnly;
    bool save_module;
    char img_filename[MAX_TEXT_LENGTH];
    char img_path[MAX_TEXT_LENGTH];
    char save_module_as[MAX_TEXT_LENGTH];
    char* module_load_buffer;
    Surface* surface_cache;
    uint16_t* surface_mempool;
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
