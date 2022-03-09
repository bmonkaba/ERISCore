/**
 * @file svcMIDI.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2022-MAR-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"

/**
 * @brief basic MIDI pub/sub service which is delivered through the subscribed objects message handler method.
 * multiple subscribers is supported, bound by MAX_MIDI_MESSAGE_APP_SUBSCRIPTIONS 
 */
class SvcMIDI:public AppBaseClass {
  public:
    SvcMIDI():AppBaseClass(){
        update_priority = 1;
        strcpy(name,"MIDI");
        midi_init = false;
        memset(&subscriptions,0, sizeof(uint16_t)*MAX_MIDI_MESSAGE_APP_SUBSCRIPTIONS);
    };
    /**
     * @brief interface for apps to request a subscription to receive incomming MIDI messages through thier message handlers
     * 
     * @param app_id 
     * @return true 
     * @return false 
     */
    bool subscribe(uint16_t app_id);

    /**
     * @brief public interface for the midi callback functions to publish messages to subscribers through thier message handlers
     * 
     * @param message 
     */
    void publish(const char *message);

    /**
     * @brief messageHandler for recieving subscription (SUB) request messages
     * 
     * @param sender 
     * @param message 
     */
    void messageHandler(AppBaseClass *sender, const char *message){
        if(0==strcmp("SUB",message)){
            subscribe(sender->getID());
        }
    }
  protected:
    bool midi_init;
    uint16_t subscriptions[MAX_MIDI_MESSAGE_APP_SUBSCRIPTIONS];

    void midiInit();
    void render() override{
        if (!midi_init){
            midiInit(); 
        }
    };    //called only when the app is active
    void update() override; //allways called even if app is not active
    void onFocus() override{};   //called when given focus
    void onFocusLost() override{}; //called when focus is taken
    void onTouch(uint16_t t_x, uint16_t t_y) override{
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //is touched
        }
    };
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{};
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{};
    void onAnalog1(uint16_t uval, float fval) override{};
    void onAnalog2(uint16_t uval, float fval) override{};
    void onAnalog3(uint16_t uval, float fval) override{};
    void onAnalog4(uint16_t uval, float fval) override{};
};