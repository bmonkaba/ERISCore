#include "svcDataDictionary.h"

/**
 * @file svcDataDictionary.cpp
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief Data Dictionary service 
 * provides an interface to a data dictionary service
 * the service shall provide a CRUD (create,read,update,destroy) interface
 * data shall be globally readable
 * create operation shall require a pointer to the owner
 * update and destroy operations can only be made by the owner
 * the owner pointer shall never be dereferenced or accessed by the service; it only serves as an ID
 * NULL pointers are OK. They can be utilized to create globally modifiable (update,destroy) records
 * 
 * 
 * @version 0.1
 * @date 2021-08-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

bool SvcDataDictionary::copyKey(const char* key){
    if (next == DATADICT_KEYVALUE_PAIRS) return false;
    return safer_strncpy(record[next].key,key,DATADICT_MAX_KEY_LEN);
}

SvcDataDictionary::SvcDataDictionary(){
            next=0;
            dd_transmitt_block = 0;
            for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
                record[i].owner = 0;
                record[i].val.int32_val = 0;
                record[i].record_type = DDRT_READWRITE;
                record[i].data_type = DDDT_INT32;
                record[i].key_hash = 48879; //0xBEEF
                #ifdef DATADICT_USE_MALLOC
                record[i].key = (char*)extmem_malloc(DATADICT_MAX_KEY_LEN+1);
                #else
                memset(record[i].key,0,sizeof(record[i].key));
                #endif
            }
}

uint32_t FASTRUN SvcDataDictionary::hash(const char* s){
    //uint32 djb2 string hash
    uint32_t h = 5381;
    int c;
    while (c = (*s++)){h = ((h << 5) + h) + c;}
    return h;
}

bool SvcDataDictionary::create(const char* key,int32_t val,uint32_t* owner){
    if (!copyKey(key)) return false;
    record[next].val.int32_val = val;
    record[next].owner = owner;
    record[next].record_type = DDRT_READ;
    record[next].key_hash = hash(key);
    next++;
    return true;
}

bool FLASHMEM SvcDataDictionary::create(const char* key,int32_t val){
    if (!copyKey(key)) return false;
    record[next].val.int32_val = val;
    record[next].owner = 0;
    record[next].record_type = DDRT_READWRITE;
    record[next].data_type = DDDT_INT32;
    record[next].key_hash = hash(key);
    next++;
    return true;
}

bool FLASHMEM SvcDataDictionary::create(const char* key,float32_t val){
    if (!copyKey(key)) return false;
    record[next].val.float32_val = val;
    record[next].owner = 0;
    record[next].record_type = DDRT_READWRITE;
    record[next].data_type = DDDT_FLOAT32;
    record[next].key_hash = hash(key);
    next++;
    return true;
}

int32_t FASTRUN SvcDataDictionary::read(const char* key){
    uint32_t h;
    h = hash(key);
    for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
        //key found
        if (record[i].key_hash == h) return record[i].val.int32_val;
    }
    //key not found
    return 0;
}

float32_t FASTRUN SvcDataDictionary::readf(const char* key){
    uint32_t h;
    h = hash(key);
    for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
        //key found
        if (record[i].key_hash == h) return record[i].val.float32_val;
    }
    //key not found
    return 0;
}

bool SvcDataDictionary::update(const char* key,int32_t val,uint32_t* owner){
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

bool SvcDataDictionary::update(const char* key,float32_t val,uint32_t* owner){
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

bool FASTRUN SvcDataDictionary::update(const char* key,int32_t val){
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

bool FASTRUN SvcDataDictionary::update(const char* key,float32_t val){
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

bool SvcDataDictionary::increment(const char* key){
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
void FLASHMEM SvcDataDictionary::printStats(){

}

void FLASHMEM SvcDataDictionary::printDictionary(SvcSerialCommandInterface* sci){
    //todo: print out in JSON format
    //Serial.flush();
    if(sci->requestStartLZ4Message()){
        sci->print(F("DD {"));
        uint16_t from, to;
        from = dd_transmitt_block++ * 16; //block size
        to = dd_transmitt_block * 16;
        if (to >= next){
            to = next;
            dd_transmitt_block = 0;
        }

        for(int i=from;i<to;i++){
            if (record[i].data_type == DDDT_INT32){
                sci->printf("\"%s\":%d",record[i].key,record[i].val.int32_val);
            }else if (record[i].data_type == DDDT_FLOAT32){
                sci->printf("\"%s\":%f",record[i].key,record[i].val.float32_val);
            }
            if (i != to-1) sci->print(",");
        }
        sci->println("}");
        sci->sendLZ4Message();
    }
}

