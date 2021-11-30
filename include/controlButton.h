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

#define SHOW_ACTIVE_TIME_MILLISEC 150

extern const ILI9341_t3_font_t Arial_9;

// Button 
//

class ControlButton:public AppBaseClass {
  public:
    ControlButton(AppBaseClass *parent):AppBaseClass(){
        setParent(parent);
        isDirty = true;
        isPressed = false;
        show_active = false;
        usingImage = false;
        imgloaded = false;
        time_active = 0;
        strcpy(text,"ControlButton");
        strcpy(name,"ControlButton");
        strcpy(img_filename,"NONE");
        strcpy(img_path,"NONE");
    };

    void setText(const char* name_string){
      if (strlen(name_string) < MAX_TEXT_LENGTH - 1){strcpy(text,name_string);
      } else strncpy(text,name_string,MAX_TEXT_LENGTH - 1);
      usingImage=false;
    };

    void setImage(const char* path,const char* filename){
        setPath(path);
        setFile(filename);
        if(imgloaded) delete(imgcache);
        imgloaded = false;
    }

    void setFile(const char* filename){
      if (strlen(filename) < MAX_TEXT_LENGTH - 1){strcpy(img_filename,filename);
      } else strncpy(img_filename,filename,MAX_TEXT_LENGTH - 1);
      usingImage=true;
    };

    void setPath(const char* path){
      if (strlen(path) < MAX_TEXT_LENGTH - 1){strcpy(img_path,path);
      } else strncpy(img_path,path,MAX_TEXT_LENGTH - 1);
    };

    
  protected:
    char text[MAX_TEXT_LENGTH];
    char img_filename[MAX_TEXT_LENGTH];
    char img_path[MAX_TEXT_LENGTH];
    Surface* imgcache;
    bool isDirty;
    bool isPressed;
    bool usingImage;
    bool imgloaded;
    elapsedMillis time_active;
    bool show_active;
    void update(){
        isDirty = true;
        
        if(isPressed==false && show_active == true && time_active > SHOW_ACTIVE_TIME_MILLISEC){
                show_active = false;
                isDirty = true;
                time_active = 0;
        }
        if (!isDirty) return;
        if(usingImage){
            if(!imgloaded){
                //allocate space
                imgcache = new Surface(am->fastImgCacheSurfaceP, &am->sd, img_path, img_filename);
                if(!imgcache){ 
                    Serial.println("M ERROR imgcache out of mem");
                    Serial.flush();
                    delete(imgcache);
                    return;
                }else{
                    draw->bltSD(imgcache->getSurfaceBufferP(), imgcache->getWidth(),img_path, img_filename,0,0,AT_NONE); //load img into cache      
                    imgloaded = true;
                    return;
                }
            } else{
                am->data->update(name,(int32_t)imgcache->getSurfaceBufferP());
                am->data->update("FastImageCacheBuffer",(int32_t)am->fastImgCacheSurfaceP->getSurfaceBufferP());
                
                draw->bltMem(am->displaySurfaceP,imgcache,x,y,AT_NONE);
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
        
        if(!usingImage){
            draw->setTextColor(am->data->read("UI_BUTTON_TEXT_COLOR"));
            draw->setCursor(x+(w/2),y+(h/2),true);
            draw->setFont(Arial_9);
            draw->print(text);
        }
        isDirty = false;
    };
    void onFocusLost(){isPressed=false;};
    void onTouch(uint16_t t_x, uint16_t t_y){
        //Serial.println("MyButton:onTouch");
        //check if touch point is within the application bounding box
        if ((t_x > x && t_x < x + w) && t_y > y && t_y < (y + h)){
            //Serial.println("MyButton:onTouch Button Pressed");
            isPressed = true;
            show_active = true;
            isDirty = true;
        }
    };
    void onTouchRelease(uint16_t t_x, uint16_t t_y){
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //Serial.println("MyButton:onTouchRelease Button Press Event Triggered");
            parentNode->MessageHandler(this,"Pressed");
        }
        isPressed = false;
        time_active = 0;
        isDirty = true;
    };
};