#ifndef UI_h
#define UI_h
#include "Arduino.h"
#include "SdFat.h"
#include "cqt.h"
//#include <ArduinoJson.h>


//defines
#define SCREEN_SLEEP_TIME 500000

#define ENC_ACCEL 2
#define RIBBON_SHOW_TIME 15
#define MESSAGE_SHOW_TIME 60

#define MENU_TITLE_SHOW_TIME 40
#define CONTROL_IMAGE_WIDTH 64
#define CONTROL_FONT_SIZE 14
#define CONTROL_FONT Arial_14
#define FPS_FONT Arial_12

#define CQT_FONT Arial_16
#define CQT_FONT_SIZE 16


#define MESSAGE_FONT Arial_14
#define MESSAGE_FONT_SIZE 14

#define NUM_MOD_SOURCES 3
#define UI_MAX_CONTROLS_PER_MENU 8
#define UI_ENCODER_MUTE_TIME 10


enum UIBLTAlphaType{AT_NONE, AT_TRANS, AT_HATCHBLK,AT_HATCHXOR};

enum UIControlType{CT_TOGGLE, CT_NUMERIC, CT_LIST};
enum UIControlList{CL_NOTE, CL_AMPSIM, CL_TBD};

enum UIEditMode{EDITMODE_REALTIMEMONITOR, EDITMODE_VALUE, EDITMODE_MODGAIN, EDITMODE_MODINVERT};

typedef struct UICType {  //UI control type
  const char *path_1;       //root folder path to theme
  const char *path_2;       //root folder path to theme
  const char *path_3;       //root folder path to theme
  const char *path_4;       //root folder path to theme
  const char *icon;       //icon bitmap filename
  const char *text;       //control text to display
  const char *key;        //key name of variable to attach to
  const char *data_type;
  const char *list_type;
  UIControlType enum_data_type;
  UIControlList enum_list_type;
  uint8_t min_value;
  uint8_t max_value;
  uint8_t step_value;
  uint8_t last_value;
  uint8_t value;
  uint8_t mod_source;
  uint8_t mod_invert;
  uint8_t mod_gain;
  uint8_t mod_value;
  uint8_t cc;
  uint16_t x;
  uint16_t y;
} UICType __attribute__ ((aligned));

typedef struct UIMenuType{
  uint8_t index;
  const char *menu_name;
  const char *description;
  const char *wallpaper;       //path to video wallpaper
  uint8_t controls_count;
  UICType *controls;
} UIMenuType __attribute__ ((aligned));

class UI
{
  uint16_t *pFB;
  SdFatSdioEX *sd;
  UIMenuType *menu;
  UIEditMode edit_mode;
  char display_message_buffer[64];
  uint16_t display_message_time;
  uint8_t update_step;
  int menu_count;
  const char *ribbon_image_folder;         //root folder path to menu theme
  const char *ribbon_image;                //menu ribbon filename
  const char *ribbon_image_select_folder;  //
  const char *ribbon_image_select;         //active ribbon selection overlay bitmap
  uint16_t x;
  uint16_t y;
  uint16_t ribbon_icon_width;
  uint16_t ribbon_icon_height;
  uint16_t ribbon_display;
  uint16_t menu_title_display;
  int16_t menu_title_slide_posy;
  int active_menu_index;
  int active_control_index;
  uint16_t active_wallpaper_animation_frame;
  uint16_t *fft_bins;
  float *cqt_bins;
  CQT *pcqt;
  int16_t *oscope;
  unsigned long last_user_interaction_time;
  unsigned long frame_time;
  unsigned long encoder1_mute;
  unsigned long encoder2_mute;
  int8_t encoder1_accel;
  int8_t encoder2_accel;
  int8_t encoder1_move;
  int8_t encoder2_move;
  int8_t encoder1_switch_pressed;
  int8_t encoder2_switch_pressed;
  int8_t encoder1_switch_released;
  int8_t encoder2_switch_released;
  int8_t encoder1_switch_state;
  int8_t encoder2_switch_state;
  unsigned long encoder1_switch_press_time;
  unsigned long encoder2_switch_press_time;
  uint16_t encoder1_switch_pressed_time;
  uint16_t encoder2_switch_pressed_time;
  uint8_t knob1_pos_changed;
  uint8_t knob2_pos_changed;
  uint8_t knob3_pos_changed;
  uint8_t knob4_pos_changed;
  uint8_t expin_pos_changed;
  unsigned long spi_start_time; //used to jump start render of next frame while current frame is still being transmitted


  public:
    UI();
    void Setup(SdFatSdioEX *sdfat);
    void Update();
    void LoadPreset();
    void setDisplayMessageWithValue(char *message,uint16_t value);
    void setDisplayMessage(char *message);
    float getControlValueFloat(const char *control_name);
    uint16_t getControlValue(const char *control_name);
    void setCCValue(uint8_t cc_id, uint8_t value);
    void setControlValue(const char *control_name,uint16_t value);
    void setControlModSource(const char *control_name,uint16_t value);
    void setControlModInvert(const char *control_name,uint16_t value);
    void setControlModGain(const char *control_name,uint16_t value);
    void setModulatorInput(uint8_t modulator_index, float modulator_value);
    uint8_t getMenuIndex(uint8_t menu_index_id);
    const char* getActiveMenuName();
    uint8_t getUpdateStep();
    void setpFFT(uint16_t *fft_bin_array);
    void setpCQT(CQT *p_cqt);
    void setpScope(int16_t *oscope_array);
    void setTapLED(bool state);
    uint16_t knob1_pos;
    uint16_t knob2_pos;
    uint16_t knob3_pos;
    uint16_t knob4_pos;
    uint16_t expin_pos;
    float level_in_r;
    float level_in_l;
    float level_out_r;
    float level_out_l;


  private:
    void None();
    UICType * getpControl(const char *control_name);
    UICType * getpCCControl(uint8_t cc_id);
    void SavePreset();
    void blt(const char *p, const char *f,int16_t x,int16_t y,UIBLTAlphaType alpha);
    void DrawMenuTitle(int16_t x,int16_t y);
    void DrawControlsText();
    void DrawControlsImg(uint8_t control_index);
    void DrawRibbon();
    void DrawScreenDoor(uint16_t y,uint16_t h);
    void DrawWallpaper();
    void DrawWallpaperImage(char *filename);
    void DrawSpectrumAnalyzer();
    void DrawConstantQTransform();
    void DrawConstantQChromagram();
    void DrawOScope();
    void DrawLevels();
    void UpdateInputs();
};
#endif
