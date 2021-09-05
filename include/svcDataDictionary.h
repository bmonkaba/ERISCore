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
#ifndef _svcDataDictionary_
#define _svcDataDictionary_

#define DATADICT_KEYVALUE_PAIRS 48
#define DATADICT_MAX_KEY_LEN 32

enum svcDataDictionaryRecordType{
    READ,
    READWRITE,
    POINTER
};

typedef struct svcDataDictionaryRecord
{
    /* value data */
    int32_t val;
    uint32_t *owner;
    int32_t *pval;
    svcDataDictionaryRecordType record_type;
}svcDataDictionaryRecord;


class SvcDataDictionary{
    private:
        char _key[DATADICT_KEYVALUE_PAIRS][DATADICT_MAX_KEY_LEN];
        svcDataDictionaryRecord record[DATADICT_KEYVALUE_PAIRS];
        uint16_t next;
    public:
        SvcDataDictionary(){
            next=0;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                record[i].owner = 0;
                record[i].val = 0;
                record[i].pval = 0;
                record[i].record_type = READWRITE;
                memset(_key[i],DATADICT_MAX_KEY_LEN,sizeof(_key[i]));
            }
        }

        bool create(const char* key,int32_t val){
            //if dictionary full
            if (next == DATADICT_KEYVALUE_PAIRS) return false;
            //TODO: ensure key doesn't already exist before creating a new record
            
            //else
            if(strlen(key) < DATADICT_MAX_KEY_LEN){
                strcpy(_key[next],key);
                record[next].val = val;
                next++;
                return true;
            }
            return false;//bad key
        }

        int32_t read(const char* key){
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if ((0==strncmp(_key[i],key,DATADICT_MAX_KEY_LEN))) return record[i].val;
            }
            //key not found
            return 0;
        }

        bool update(const char* key,int32_t val){
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if ((0==strncmp(_key[i],key,DATADICT_MAX_KEY_LEN))){
                    record[i].val = val;
                    return true;
                }
            }
            //key not found - try to create a new record
            return create(key, val);
        }

        bool increment(const char* key){
            int index = -1;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if ((0==strncmp(_key[i],key,DATADICT_MAX_KEY_LEN))){
                    index = i;
                    break;
                }
            }
            if (index == -1){
                if (!create(key,0)) return false;
                index = next -1;
            }
            record[index].val++;
            return true;
        }

        //serial interface
        void printStats(){

        }
        
        void printDictionary(){
            //todo: print out in JSON format
            Serial.flush();
            Serial.print("DD {");

            for(int i=0;i<next;i++){
                Serial.printf("\"%s\":%d",_key[i],record[i].val);
                if (i != next-1) Serial.print(",");
                Serial.flush();
            }
            Serial.println("}");
            Serial.flush();
        }

};

#endif