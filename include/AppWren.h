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
 * @brief Wren is a scripting language. This class embeds Wren.
 * the purpose of the class is to mirror the AppBaseClass into wren
 * the use case is to allow scripting of apps
 * the major benifit and the reason to even try this is the ability to dynamicly load applications \
 * the second benifit is program storage moves from the precious and limited microcontroller flash to the SD card
 * note: available heap defines the upper boundary of VM size
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
        vmConstructor();
    }; 
    ~AppWren(){
        wrenFreeVM(vm);
    };

    void vmConstructor();

    void compileScript(const char* script){


    }

    void loadScript(const char* script){
        const char* module = "main";
        Serial.println(F("M AppWren: Loading script into the VM"));
        isWrenResultOK(wrenInterpret(vm, module, script));
    }
    
    //define event handlers
    void MessageHandler(AppBaseClass *sender, const char *message){   
        if(sender->isName("SOME APP NAME")){
            
        }
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
        wrenGetVariable(vm, "main", "ErisApp", 0);
        //get the handles
        h_slot0 = wrenGetSlotHandle(vm, 0);
        h_update = wrenMakeCallHandle(vm, "update()");
        h_updateRT = wrenMakeCallHandle(vm, "updateRT()");
        /*
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
        */
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
            if (isWrenResultOK(wrenCall(vm,h_update))){

            };
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
