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

#define DATADICT_KEYVALUE_PAIRS 42
#define DATADICT_MAX_KEY_LEN 32

enum svcDataDictionaryRecordType{
    DDRT_READ,
    DDRT_READWRITE,
    DDRT_POINTER,
    DDRT_EMPTY
};

typedef struct svcDataDictionaryRecord
{
    /* value data */
    char key[DATADICT_MAX_KEY_LEN];
    int32_t val;
    uint32_t *owner;
    int32_t *pval;
    svcDataDictionaryRecordType record_type;
    char _padding[16];
}svcDataDictionaryRecord __attribute__ ((aligned (32)));;

class SvcDataDictionary{
    private:
        svcDataDictionaryRecord record[DATADICT_KEYVALUE_PAIRS];
        uint16_t next;
    public:
        SvcDataDictionary(){
            next=0;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                record[i].owner = 0;
                record[i].val = 0;
                record[i].pval = 0;
                record[i].record_type = DDRT_READWRITE;
                memset(record[i].key,0,sizeof(record[i].key));
            }
        }

        bool create(const char* key,int32_t val){
            //if dictionary full
            if (next == DATADICT_KEYVALUE_PAIRS) return false;
            //TODO: ensure key doesn't already exist before creating a new record
            
            //else
            if(strlen(key) < DATADICT_MAX_KEY_LEN){
                strcpy(record[next].key,key);
                record[next].val = val;
                record[next].pval = 0;
                record[next].owner = 0;
                record[next].record_type = DDRT_READWRITE;
                next++;
                return true;
            }
            return false;//bad key
        }

        int32_t read(const char* key){
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))) return record[i].val;
            }
            //key not found
            return 0;
        }

        bool update(const char* key,int32_t val){
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
                    if (record[next].record_type == DDRT_READWRITE){
                        record[i].val = val;
                        arm_dcache_flush_delete(&record[i], sizeof(record[i]));
                        return true;
                    } else return false;
                }
            }
            //key not found - try to create a new record
            return create(key, val);
        }

        bool increment(const char* key){
            int index = -1;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
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
                Serial.printf("\"%s\":%d",record[i].key,record[i].val);
                if (i != next-1) Serial.print(",");
            }
            Serial.println("}");
            Serial.flush();
        }

};

#endif