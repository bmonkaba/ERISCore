#include <Arduino.h>
#include <string.h>
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
#define DATADICT_MAX_KEY_LEN 48

#define DATADICT_USE_MALLOC
enum svcDataDictionaryRecordType{
    DDRT_READ,
    DDRT_READWRITE,
    DDRT_POINTER,
    DDRT_EMPTY
};

enum svcDataDictionaryDataType{
    DDDT_INT16,
    DDDT_UINT16,
    DDDT_INT32,
    DDDT_UINT132,
    DDDT_FLOAT16,
    DDDT_FLOAT32,
    DDDT_PINT16,
    DDDT_PUINT16,
    DDDT_PINT32,
    DDDT_PUINT132,
    DDDT_PFLOAT16,
    DDDT_PFLOAT32,
    DDDT_EMPTY
};

typedef union value_container{
    int16_t int16_val;
    uint16_t uint16_val;
    int32_t int32_val;
    uint32_t uint32_val;
    float32_t float16_val;
    float32_t float32_val;
};

typedef union pointer_container{
    int16_t* pint16_val;
    uint16_t* puint16_val;
    int32_t* pint32_val;
    uint32_t* puint32_val;
    float32_t* pfloat16_val;
    float32_t* pfloat32_val;
};

typedef struct svcDataDictionaryRecord
{
    /* value data */
    uint32_t key_hash;
#ifdef DATADICT_USE_MALLOC
    char* key;
#else
    char key[DATADICT_MAX_KEY_LEN];
#endif
    value_container val;
    uint32_t *owner;
    pointer_container *pval;
    svcDataDictionaryRecordType record_type;
    svcDataDictionaryDataType data_type;
}svcDataDictionaryRecord;// __attribute__ ((aligned (32)));

class SvcDataDictionary{
    private:
        svcDataDictionaryRecord record[DATADICT_KEYVALUE_PAIRS];
        uint16_t next;

    uint32_t hash(const char* s){
        //uint32 djb2 string hash
        uint32_t h = 5381;
        int c;
        while (c = *s++){h = ((h << 5) + h) + c;}
        return h;
    }

    public:
        SvcDataDictionary(){
            next=0;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                record[i].owner = 0;
                record[i].val.int32_val = 0;
                record[i].pval = 0;
                record[i].record_type = DDRT_READWRITE;
                record[i].data_type = DDDT_INT32;
                record[i].key_hash = 48879; //0xBEEF
                #ifdef DATADICT_USE_MALLOC
                record[i].key = NULL;
                #else
                memset(record[i].key,0,sizeof(record[i].key));
                #endif
            }
        }

        bool create(const char* key,int32_t val,uint32_t* owner){
            //if dictionary full
            if (next == DATADICT_KEYVALUE_PAIRS) return false;
            //TODO: ensure key doesn't already exist before creating a new record
            //else
            if(strlen(key) < DATADICT_MAX_KEY_LEN){
                #ifdef DATADICT_USE_MALLOC
                record[next].key = strdup(key);
                //record[next].key = (char*)malloc(strlen(key)+1);
                //if (record[next].key != NULL){
                //strcpy(record[next].key,key);
                //} else return false; //if malloc failed
                #else
                strcpy(record[next].key,key);
                #endif
                record[next].val.int32_val = val;
                record[next].pval = 0;
                record[next].owner = owner;
                record[next].record_type = DDRT_READ;
                record[next].key_hash = hash(key);
                next++;
                return true;
            }
            return false;//bad key
        }

        bool create(const char* key,int32_t val){
            //if dictionary full
            if (next == DATADICT_KEYVALUE_PAIRS) return false;
            //TODO: ensure key doesn't already exist before creating a new record
            //else
            if(strlen(key) < DATADICT_MAX_KEY_LEN){
                #ifdef DATADICT_USE_MALLOC
                record[next].key = (char*)malloc(strlen(key)+1);
                if (record[next].key != NULL){
                    strcpy(record[next].key,key);
                } else return false; //if malloc failed               
                #else
                strcpy(record[next].key,key);
                #endif
                record[next].val.int32_val = val;
                record[next].pval = 0;
                record[next].owner = 0;
                record[next].record_type = DDRT_READWRITE;
                record[next].data_type = DDDT_INT32;
                record[next].key_hash = hash(key);
                next++;
                return true;
            }
            return false;//bad key
        }

        bool create(const char* key,float32_t val){
            //if dictionary full
            if (next == DATADICT_KEYVALUE_PAIRS) return false;
            //TODO: ensure key doesn't already exist before creating a new record
            //else
            if(strlen(key) < DATADICT_MAX_KEY_LEN){
                #ifdef DATADICT_USE_MALLOC
                record[next].key = (char*)malloc(strlen(key)+1);
                if (record[next].key != NULL){
                    strcpy(record[next].key,key);
                } else return false; //if malloc failed               
                #else
                strcpy(record[next].key,key);
                #endif
                record[next].val.float32_val = val;
                record[next].pval = 0;
                record[next].owner = 0;
                record[next].record_type = DDRT_READWRITE;
                record[next].data_type = DDDT_FLOAT32;
                record[next].key_hash = hash(key);
                next++;
                return true;
            }
            return false;//bad key
        }

        int32_t read(const char* key){
            uint32_t h;
            h = hash(key);
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h) return record[i].val.int32_val;
            }
            //key not found
            return 0;
        }

        float32_t readf(const char* key){
            uint32_t h;
            h = hash(key);
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h) return record[i].val.float32_val;
            }
            //key not found
            return 0;
        }

        bool update(const char* key,int32_t val,uint32_t* owner){
            uint32_t h;
            h = hash(key);

            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h){
                //if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
                    if (record[next].owner == owner && record[next].data_type == DDDT_INT32){
                        record[i].val.int32_val = val;
                        //arm_dcache_flush_delete(&record[i], sizeof(record[i]));
                        return true;
                    } else return false;
                }
            }
            //key not found - try to create a new record
            return create(key, val, owner);
        }

        bool update(const char* key,float32_t val,uint32_t* owner){
            uint32_t h;
            h = hash(key);

            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h){
                //if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
                    if (record[next].owner == owner && record[next].data_type == DDDT_FLOAT32){
                        record[i].val.float32_val = val;
                        //arm_dcache_flush_delete(&record[i], sizeof(record[i]));
                        return true;
                    } else return false;
                }
            }
            //key not found - try to create a new record
            return create(key, val, owner);
        }

        bool update(const char* key,int32_t val){
            uint32_t h;
            h = hash(key);

            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h){
                //if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
                    if (record[i].record_type == DDRT_READWRITE && record[i].data_type == DDDT_INT32){
                        record[i].val.int32_val = val;
                        //arm_dcache_flush_delete(&record[i], sizeof(record[i]));
                        return true;
                    } else return false;
                }
            }
            //key not found - try to create a new record
            return create(key, val);
        }

        bool update(const char* key,float32_t val){
            uint32_t h;
            h = hash(key);

            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h){
                //if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
                    if (record[i].record_type == DDRT_READWRITE && record[i].data_type == DDDT_FLOAT32){
                        record[i].val.float32_val = val;
                        //arm_dcache_flush_delete(&record[i], sizeof(record[i]));
                        return true;
                    } else return false;
                }
            }
            //key not found - try to create a new record
            return create(key, val);
        }

        bool increment(const char* key){
            uint32_t h;
            h = hash(key);

            int index = -1;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                //key found
                if (record[i].key_hash == h){
                //if ((0==strncmp(record[i].key,key,DATADICT_MAX_KEY_LEN))){
                    index = i;
                    break;
                }
            }
            if (index == -1){
                if (!create(key,(int32_t)0)) return false;
                index = next -1;
            }
            if (record[index].data_type == DDDT_INT32){
                record[index].val.int32_val++;
            }else if (record[index].data_type == DDDT_FLOAT32){
                record[index].val.float32_val++;
            }
            return true;
        }

        //serial interface
        void printStats(){

        }
        
        void printDictionary(){
            //todo: print out in JSON format
            //Serial.flush();
            Serial.print("DD {");

            for(int i=0;i<next;i++){
                if (record[i].data_type == DDDT_INT32){
                    Serial.printf("\"%s\":%d",record[i].key,record[i].val.int32_val);
                }else if (record[i].data_type == DDDT_FLOAT32){
                    Serial.printf("\"%s\":%f",record[i].key,record[i].val.float32_val);
                }
                if (i != next-1) Serial.print(",");
            }
            Serial.println("}");
            //Serial.flush();
        }

};

#endif