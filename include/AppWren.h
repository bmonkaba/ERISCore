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
#include <LittleFS.h>
#include "AppBaseClass.h"
#include "wren.hpp"
#include "ErisGlobals.h"


/**
 * @brief Wren is a scripting language. This class is a proxy which mirrors the AppBaseClass into wren and hosts a VM.\n 
 * \n  
 * The use case for this class is to allow scripting of apps\n 
 * With this interface, applications can be dynamically loaded and executed\n 
 * Beware: The available heap defines the upper boundary of VM size\n 
 * \n 
 * From https://wren.io/:\n 
 * "Wren is a small, fast, class-based concurrent scripting language. \n 
 * Think Smalltalk in a Lua-sized package with a dash of Erlang and wrapped up in a familiar, modern syntax.\n 
 * Wren is small. The VM implementation is under 4,000 semicolons"\n 
 */

class AppWren:public AppBaseClass {
  public:
    LittleFS_RAM wren_file_system;//public access for callback
    File wren_file;//public access for callback
    bool useNativeFS;

    AppWren():AppBaseClass(){
        strcpy(name,"AppWren");
        strcpy(img_filename,"");
        strcpy(img_path,"");
        strcpy(wren_module_name,"");
        update_priority = 0; 
        surface_mempool = NULL;
        image_loaded = false;
        h_slot0 = 0;
        useNativeFS = true;
        vm = 0;
        h_render = 0;
        h_update = 0;
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
        compile_only = false;
        save_module = false;
        reboot_request = false;
        enable_call_forwarding = false;
        is_pressed = false;
        show_active = false;
        using_image = true;
        time_active = 0;
        surface_cache = NULL;
        module_load_buffer = NULL;
        vmConstructor(&g_wrenScript[0]);
        widget_width = 64;
        widget_height = 64;
        widget_origin_x = SCREEN_WIDTH - 64 - 5;
        widget_origin_y = SCREEN_HEIGHT - 64 - 5;
        //surface_mempool = (uint16_t*)malloc(sizeof(uint16_t)*128*128);
        wren_file_system.begin(8000000);//init the shared ext_ram drive
        wren_file_system.quickFormat();  
        wren_file_system.mkdir("system");//system folder
        wren_file = 0;
    };

    ~AppWren(){
        wrenFreeVM(vm);
    };

    /**
     * @brief start, load and configure the handles of the VM
     * 
     */
    void vmConstructor(const char* initial_script);


    /**
     * @brief shutdown the VM
     * 
     */
    void shutdownVM(){
        if(has_pop){
            am->releasePopUp();
            has_pop = false;
        }
        releaseWrenHandles();
        wrenFreeVM(vm);
        if(surface_cache){
            //delete(surface_cache);
            //surface_cache = NULL;
        }
    }

    /**
     * @brief shutsdown then starts the VM
     * 
     */
    void restartVM(){
        shutdownVM();
        useNativeFS = true;
        startVM();
    }

    /**
     * @brief start the VM
     * 
     */
    void startVM();

    /**
     * @brief halts forwarding of the AppBase class method calls/n
     * this allows scripts to run without utilizing the App interface within wren\n 
     */
    void haltCallForwarding(){
        enable_call_forwarding = false;
    }

    /**
     * @brief runs the script in the VM
     * 
     * @param script 
     * @return true wrenInterpret result is ok
     * @return false wrenInterpret result is not ok
     */
    bool loadScript(const char* script){
        const char* module = "main";
        bool res;
        res = isWrenResultOK(wrenInterpret(vm, module, script));
        if(res){
            getWrenHandles();
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
        }

        if(sci->requestStartLZ4Message()){
            sci->printf("M AppWren:loadScript: Loading script (size: %d bytes)\n",strlen(script));
            if (res){ 
                sci->println(F("M AppWren::loadScript: OK"));
            }else sci->println(F("M AppWren::loadScript: NOT OK"));
            sci->sendLZ4Message();
        }
        return res;
    }

     /**
     * @brief c callback support to allow wren scripts to request a VM reboot & script load\n
     * 
     * @param script_name 
     */
    void rebootRequest(char* script_name){
        //the actual reboot load request will be completed in the AppWren::update loop.
        if(sci->requestStartLZ4Message()){
            sci->printf("M AppWren:rebootRequest: %s\n",script_name);
            sci->sendLZ4Message();
        }
        reboot_request = true;
        safer_strncpy(wren_module_name,script_name,sizeof(wren_module_name));
    }

    /**
     * @brief handles messages from other sw components
     * 
     * @param sender 
     * @param message 
     */
    void messageHandler(AppBaseClass *sender, const char *message);

    //in the AppBase class these are protected methods.. we need to expose them here publicly to set up the Wren callbacks.
    //void _updatePosition(){AppBaseClass::_updatePosition();}

    /**
     * @brief Set the Parent object
     * 
     * @param parent 
     */
    void setParent(AppBaseClass *parent){AppBaseClass::setParent(parent);}
    
    /**
     * @brief Set the Position object/n
     * receiver for the wren c callback function setPositionCallback
     * 
     * @param newOriginX 
     * @param newOriginY 
     */
    void setPosition(int16_t newOriginX, int16_t newOriginY);
    
    /**
     * @brief Set the Dimension object\n
     * receiver for the wren c callback function setDimensionCallback
     * @param new_width 
     * @param new_height 
     */
    void setDimension(int16_t new_width, int16_t new_height);
    
    /**
     * @brief Set the Widget Position object\n
     * receiver for the wren c callback function setWidgetPositionCallback
     * @param newOriginX 
     * @param newOriginY 
     */
    void setWidgetPosition(int16_t newOriginX, int16_t newOriginY);
    
    /**
     * @brief Set the Widget Dimension object\n
     * receiver for the wren c callback function setWidgetDimensionCallback
     * @param new_width 
     * @param new_height 
     */
    void setWidgetDimension(int16_t new_width, int16_t new_height);
    
    /**
     * @brief Get the Focus object
     * 
     */
    void getFocus(){AppBaseClass::getFocus();}
    
    /**
     * @brief 
     * 
     */
    void returnFocus(){AppBaseClass::returnFocus();}
    
    /**
     * @brief 
     * 
     * @param exclusive 
     */
    void requestPopUp(bool exclusive=false){AppBaseClass::requestPopUp(exclusive);}
    
    /**
     * @brief 
     * 
     */
    void releasePopUp(){AppBaseClass::releasePopUp();}
    
    /**
     * @brief 
     * 
     * @param level 
     */
    void setUpdatePriority(uint16_t level);

    /**
     * @brief receiver for the wren c callback function getSourceForModule
     * 
     * @param name 
     * @return const char* 
     */
    const char* loadModuleSource(const char * name);

    /**
     * @brief receiver for the wren c callback function loadModuleComplete indicating it's now ok to release the module_load_buffer
     * 
     */
    void freeModuleSource();

    /**
     * @brief provides an interface for wren c call backfunctions to get the ILI9341_t3_ERIS draw object.
     * 
     * @return ILI9341_t3_ERIS* 
     */
    ILI9341_t3_ERIS* getDraw(){return draw;}

    /**
     * @brief provides an interface for wren c call backfunctions to get the AudioDirector object
     * 
     * @return AudioDirector* 
     */
    AudioDirector* getAudioDirector(){return ad;} 
    


    /**
     * @brief block tramsfer from ram drive file to surface
     * 
     * @param path 
     * @param filename 
     * @param x 
     * @param y 
     */
    void bltRAMDrive2Surface(const char *path, const char *filename, int16_t x, int16_t y,bltMode blt_mode){
        dynamicSurfaceManager();
        uint16_t *sb = surface_cache->getSurfaceBufferP();
        File file = wren_file_system.open(filename,O_RDONLY);
        if(file.available()>0){
            draw->bltRAMFileB(sb,surface_cache->getWidth(),surface_cache->getHeight(),&file,x,y,blt_mode);
            file.close();
        }
    }

    /**
     * @brief block tramsfer from ram drive filei to the frame buffer
     * 
     * @param path 
     * @param filename 
     * @param x 
     * @param y 
     * @param blt_mode 
     */
    void bltRAMDrive2FrameBuffer(const char *path, const char *filename, int16_t x, int16_t y,bltMode blt_mode){
        dynamicSurfaceManager();
        File file = wren_file_system.open(filename,O_RDONLY);
        if(file.available()>0){
            draw->bltRAMFileB(draw->getFrameBuffer(),SCREEN_WIDTH,SCREEN_HEIGHT,&file,x,y,blt_mode);
            file.close();
        }
    }
    
    /**
     * @brief block tramsfer from SD to surface
     * 
     * @param path 
     * @param filename 
     * @param x 
     * @param y 
     */
    void bltSD2Surface(const char *path, const char *filename, int16_t x, int16_t y,bltMode blt_mode){
        dynamicSurfaceManager();
        uint16_t *sb = surface_cache->getSurfaceBufferP();
        draw->bltSDB(sb,surface_cache->getWidth(),surface_cache->getHeight(),path,filename,x,y,blt_mode);
    }


    void bltSurface2FrameBuffer(int16_t from_x, int16_t from_y,int16_t width,int16_t height,int16_t to_x, int16_t to_y,bltMode blt_mode){
        dynamicSurfaceManager();
        uint16_t *sb = surface_cache->getSurfaceBufferP();
        Surface source((uint16_t*)(sb + from_x + (from_y * surface_cache->getWidth())),width,height);
        Surface dest(draw->getFrameBuffer(),SCREEN_WIDTH,SCREEN_HEIGHT);
        
        //bltSurface2Surface(Surface *dest, Surface *source,int16_t pos_x,int16_t pos_y,bltMode blt_mode)
        draw->bltSurface2Surface(&dest,to_x,to_y,&source,0,0,source.getWidth(),source.getHeight(),blt_mode);
    }

    /**
     * @brief set a pixel on the render target
     * 
     * @param x 
     * @param y 
     * @param r 
     * @param g 
     * @param b 
     */
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
    
    /**
     * @brief get a pixel from the render target
     * 
     * @param x 
     * @param y 
     * @return uint16_t 
     */
    uint16_t getPixel(int16_t x, int16_t y){
        if (draw){
            if (!surface_cache) return ILI9341_MAGENTA;
            if (x >= surface_cache->getWidth()) return ILI9341_MAGENTA;
            if (y >= surface_cache->getHeight()) return ILI9341_MAGENTA;
            if (x < 0 || y < 0) return ILI9341_MAGENTA;
            return draw->readSurfacePixel(surface_cache,x,y);
        }
        return 0; 
    }

    /**
     * @brief draws a line on the render target
     * 
     * @param start_x 
     * @param start_y 
     * @param end_x 
     * @param end_y 
     * @param r 
     * @param g 
     * @param b 
     */
    void drawLine(int16_t start_x, int16_t start_y,int16_t end_x, int16_t end_y, int16_t r, int16_t g, int16_t b){
        
        if ((start_x == end_x) && (start_y == end_y)){
            if (draw){
                if (has_pop){
                    //draw->ILI9341_t3n::drawPixel(start_x,start_y,CL((uint16_t)r,(uint16_t)g,(uint16_t)b));
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
                //draw->ILI9341_t3n::drawLine(start_x, start_y, end_x, end_y, CL(r,g,b));
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

    /**
     * @brief fills the render target (either a surface or the framebuffer depending on )
     * 
     * @param r 
     * @param g 
     * @param b 
     */
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
    
    /**
     * @brief release any/all Wren embedded call handles
     * 
     */
    void releaseWrenHandles();

    /**
     * @brief VM post module load support function creates/updates the Wren embedded call handles
     * 
     */
    void getWrenHandles();
    
    bool isWrenResultOK(WrenInterpretResult res){
        switch (res){
            case WREN_RESULT_COMPILE_ERROR:
                if(sci->requestStartLZ4Message()){
                    sci->printf(F("M AppWren::isWrenResultOK WREN_RESULT_COMPILE_ERROR\n"));
                    sci->printf(F("VM WREN_ERR Compilation Failed\n"));
                    sci->sendLZ4Message();
                }
                return false;
            case WREN_RESULT_RUNTIME_ERROR:
                if(sci->requestStartLZ4Message()){
                    sci->printf(F("M AppWren::isWrenResultOK WREN_RESULT_RUNTIME_ERROR\n"));
                    sci->printf(F("VM WREN_WRN [Implementation Warning] call forwarding will be disabled until next script load.\n"));
                    sci->sendLZ4Message();
                }
                return false;
            case WREN_RESULT_SUCCESS:
                return true;
            default:
                if(sci->requestStartLZ4Message()){
                    sci->printf(F("M AppWren::isWrenResultOK UNDEFINED RESULT!\n"));
                    sci->sendLZ4Message();
                }
                return false; //should never execute
        } 
    }

    /**
     * @brief responsible for managing the surface buffer memory allocation
     * 
     * @return true 
     * @return false 
     */
    bool dynamicSurfaceManager();

    /**
     * @brief class specific update implementation
     * 
     */
    void render() override;//called only when the app is active

    /**
     * @brief class specific update implementation
     * 
     */
    void update() override;

    /**
     * @brief class specific OnFocus implementation
     * 
     */
    void onFocus() override{
        if (!enable_call_forwarding || h_onFocus==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (!isWrenResultOK(wrenCall(vm,h_onFocus))){
                releaseWrenHandles();
            };
        }
    };   //called when given focus

    /**
     * @brief class specific onFocusLost implementation
     * 
     */
    void onFocusLost() override{
        if (!enable_call_forwarding || h_onFocusLost==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 1);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            if (!isWrenResultOK(wrenCall(vm,h_onFocusLost))){
                releaseWrenHandles();
            };
        }
    }; //called when focus is taken

    /**
     * @brief class specific onTouch implementation
     * 
     * @param t_x 
     * @param t_y 
     */
    void onTouch(uint16_t t_x, uint16_t t_y) override{
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            if (!enable_call_forwarding || h_onTouch==0){ //if no script loaded
                return;
            } else{
                wrenEnsureSlots(vm, 3);
                wrenSetSlotHandle(vm, 0, h_slot0);//App
                wrenSetSlotDouble(vm, 1, t_x);//param
                wrenSetSlotDouble(vm, 2, t_y);//param
                if (!isWrenResultOK(wrenCall(vm,h_onTouch))){
                    releaseWrenHandles();
                };
            }
        }
    };

    /**
     * @brief class specific onTouchDrag implementation
     * 
     * @param t_x 
     * @param t_y 
     */
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{
        if (!enable_call_forwarding || h_onTouchDrag==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 3);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, t_x);//param
            wrenSetSlotDouble(vm, 2, t_y);//param
            if (!isWrenResultOK(wrenCall(vm,h_onTouchDrag))){
                releaseWrenHandles();
            };
        }
    };

    /**
     * @brief class specific onTouchRelease implementation
     * 
     * @param t_x 
     * @param t_y 
     */
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{
        if (!enable_call_forwarding || h_onTouchRelease==0){ //if no script loaded
            return;
        } else{
            wrenEnsureSlots(vm, 3);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, t_x);//param
            wrenSetSlotDouble(vm, 2, t_y);//param
            if (!isWrenResultOK(wrenCall(vm,h_onTouchRelease))){
                releaseWrenHandles();
            };
        }
    };

    /**
     * @brief class specific onAnalog1 implementation
     * 
     * @param uval 
     * @param fval 
     */
    void onAnalog1(uint16_t uval, float fval) override{
        if (!enable_call_forwarding || h_onAnalog1==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (!isWrenResultOK(wrenCall(vm,h_onAnalog1))){
                releaseWrenHandles();
            };
        }
    };

    /**
     * @brief class specific onAnalog2 implementation
     * 
     * @param uval 
     * @param fval 
     */
    void onAnalog2(uint16_t uval, float fval) override{
        if (!enable_call_forwarding || h_onAnalog2==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (!isWrenResultOK(wrenCall(vm,h_onAnalog2))){
                releaseWrenHandles();
            };
        }
    };

    /**
     * @brief class specific onAnalog3 implementation
     * 
     * @param uval 
     * @param fval 
     */
    void onAnalog3(uint16_t uval, float fval) override{
        if (!enable_call_forwarding || h_onAnalog3==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (!isWrenResultOK(wrenCall(vm,h_onAnalog3))){
                releaseWrenHandles();
            };
        }
    };

    /**
     * @brief class specific onAnalog4 implementation
     * 
     * @param uval 
     * @param fval 
     */
    void onAnalog4(uint16_t uval, float fval) override{
        if (!enable_call_forwarding || h_onAnalog4==0){
            return;
        } else{
            wrenEnsureSlots(vm, 2);
            wrenSetSlotHandle(vm, 0, h_slot0);//App
            wrenSetSlotDouble(vm, 1, fval);//param
            if (!isWrenResultOK(wrenCall(vm,h_onAnalog4))){
                releaseWrenHandles();
            };
        }
    };

 protected:
    bool compile_only;
    bool save_module;
    bool reboot_request;
    bool enable_call_forwarding;
    char img_filename[MAX_TEXT_LENGTH];
    char img_path[MAX_TEXT_LENGTH];
    char wren_module_name[MAX_TEXT_LENGTH];
    char* module_load_buffer;
    Surface* surface_cache;
    uint16_t* surface_mempool;
    bool is_pressed;
    bool using_image;
    bool image_loaded;
    elapsedMillis time_active;
    bool show_active;
    

 private:
    WrenVM* vm;
    WrenHandle* h_slot0;
    WrenHandle* h_render;
    WrenHandle* h_update;
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
