/**
 * @file controlButton.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppManager.h"
#include "ErisGlobals.h"
#include "ErisUtils.h"

extern const ILI9341_t3_font_t Arial_9;

// Button 
//

class ControlButton:public AppBaseClass {
  public:
    ControlButton(AppBaseClass *parent):AppBaseClass(){
        setParent(parent);
        update_priority = 35000;
        is_dirty = true;
        is_pressed = false;
        show_active = false;
        using_image = false;
        img_loaded = false;
        time_active = 0;
        img_cache = NULL;
        strcpy(text,"ControlButton");
        strcpy(name,"ControlButton");
        strcpy(img_filename,"NONE");
        strcpy(img_path,"NONE");
    };

    void FLASHMEM setText(const char* name_string){
      if (strlen(name_string) < MAX_TEXT_LENGTH - 1){strcpy(text,name_string);
      } else safer_strncpy(text,name_string,MAX_TEXT_LENGTH - 1);
      using_image=false;
    };

    void FLASHMEM setImage(const char* path,const char* filename){
        setPath(path);
        setFile(filename);
        if(img_loaded) delete(img_cache);
        img_loaded = false;
    }

    void FLASHMEM setFile(const char* filename){
      if (strlen(filename) < MAX_TEXT_LENGTH - 1){strcpy(img_filename,filename);
      } else safer_strncpy(img_filename,filename,MAX_TEXT_LENGTH - 1);
      using_image=true;
    };

    void FLASHMEM setPath(const char* path){
      if (strlen(path) < MAX_TEXT_LENGTH - 1){strcpy(img_path,path);
      } else safer_strncpy(img_path,path,MAX_TEXT_LENGTH - 1);
    };

    
  protected:
    char text[MAX_TEXT_LENGTH];
    char img_filename[MAX_TEXT_LENGTH];
    char img_path[MAX_TEXT_LENGTH];
    Surface* img_cache;
    bool is_dirty;
    bool is_pressed;
    bool using_image;
    bool img_loaded;
    elapsedMillis time_active;
    bool show_active;
    void FLASHMEM render() override{
        is_dirty = true;
        
        if(is_pressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                show_active = false;
                is_dirty = true;
                time_active = 0;
        }
        if (!is_dirty) return;
        if(using_image){
            if(!img_loaded){
                //allocate space
                img_cache = new Surface(am->p_fast_img_cache_surface, &am->sd, img_path, img_filename);
                if(!img_cache){ 
                    Serial.println(F("M ERROR img_cache out of mem"));
                    Serial.flush();
                    delete(img_cache);
                    return;
                }else{
                    draw->bltSDB(img_cache->getSurfaceBufferP(), img_cache->getWidth(), img_cache->getHeight(), img_path, img_filename, 0, 0, AT_NONE); //load img into cache      
                    img_loaded = true;
                    return;
                }
            } else{
                draw->bltMem(am->p_display_surface,img_cache,x,y,AT_NONE);
            }
        }else{
            draw->fillRoundRect(x,y,w,h/2+3,3,am->data->read("UI_BUTTON_FILL_COLOR"));
            draw->fillRoundRect(x,y+h/2,w,h/2,3,am->data->read("UI_BUTTON_SHADE_COLOR"));
        }
        if (show_active){
            draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_ACTIVE_BORDER_COLOR")); //ILI9341_GREENYELLOW
        } else{
            draw->drawRoundRect(x,y,w,h,4,am->data->read("UI_BUTTON_INACTIVE_BORDER_COLOR"));//ILI9341_MAGENTA
        }
        
        if(!using_image){
            draw->setTextColor(am->data->read("UI_BUTTON_TEXT_COLOR"));
            draw->setCursor(x+(w/2),y+(h/2),true);
            draw->setFont(Arial_9);
            draw->print(text);
        }
        is_dirty = false;
    };
    void FLASHMEM onFocusLost() override{is_pressed=false;};
    void FLASHMEM onTouch(uint16_t t_x, uint16_t t_y) override{
        //Serial.println("MyButton:onTouch");
        //check if touch point is within the application bounding box
        if ((t_x > x && t_x < x + w) && t_y > y && t_y < (y + h)){
            //Serial.println("MyButton:onTouch Button Pressed");
            is_pressed = true;
            show_active = true;
            is_dirty = true;
        }
    };
    void FLASHMEM onTouchRelease(uint16_t t_x, uint16_t t_y) override{
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //Serial.println("MyButton:onTouchRelease Button Press Event Triggered");
            parent_node->messageHandler(this,"Pressed");
        }
        is_pressed = false;
        time_active = 0;
        is_dirty = true;
    };
};