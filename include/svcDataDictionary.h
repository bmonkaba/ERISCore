#include <Arduino.h>

/**
 * @file svcDataDictionary.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief Data Dictionary service 
 * provides an interface to a data dictionary service
 * the service shall provide a CRUD (create,read,update,destroy) interface
 * data shall be globaly readable
 * create operation shall require a pointer to the owner
 * update and destroy operations can only be made by the owner
 * the owner pointer shall never be dereferenced or accessed by the service; it only serves as an ID
 * NULL pointers are OK. They can be utilized to create globaly modifiable (update,destroy) records
 * 
 * 
 * @version 0.1
 * @date 2021-08-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#define DATADICT_KEYVALUE_PAIRS 32
#define DATADICT_MAX_KEY_LEN 16

class SvcDataDictionary{
    private:
        char _key[DATADICT_KEYVALUE_PAIRS][DATADICT_MAX_KEY_LEN];
        int32_t _val[DATADICT_KEYVALUE_PAIRS];
        uint16_t next;
    public:
        SvcDataDictionary(){
            next=0;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                _val[i] = 0;
                memset(&_key[i][0],DATADICT_MAX_KEY_LEN,sizeof(char));
            }
        }

        bool create(const char* key,int32_t val){
            //if dictionary full
            if (next == DATADICT_KEYVALUE_PAIRS) return false;
            //else
            strcpy(&_key[next][0],key);
            _val[next] = val;
            next++;
            return true;
        }

        int32_t read(const char* key){
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (0==strcmp(&_key[i][0],key)) return _val[i];
            }
            //key not found
            return 0;
        }

        bool update(const char* key,int32_t val){
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (0==strcmp(&_key[i][0],key)){
                    _val[i] = val;
                    return true;
                }
            }
            //key not found
            return create(key, val);
            //return false;
        }

        //serial interface
        void printStats(){

        }
        
        void printDictionary(){
            //todo: print out in JSON format
            Serial.flush();
            Serial.print("DD {");

            for(int i=0;i<next;i++){
                Serial.printf("\"%s\":%d",_key[i],_val[i]);
                if (i != next-1) Serial.print(",");
            }
            Serial.println("}");
            Serial.flush();
        }

};