#include "svcDataDictionary.h"

/**
 * @file svcDataDictionary.cpp
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


bool SvcDataDictionary::copyKey(const char* key){
    //if dictionary full
    if (next == DATADICT_KEYVALUE_PAIRS) return false;
    record[next].key = strndup(key,DATADICT_MAX_KEY_LEN);
    //check for strndup malloc failure
    if (record[next].key == NULL) return false;
    /*
        strndup doesn't ensure null termination of strings! meaning it will fill
        the entire buffer with chars ommiting the null ternimation to indicate the
        end of a string.

        strndup however does fill the buffer with null chars if given a string is shorter than the buffer len 

        The simple guard for null termination loss is to always write a null char at the end of the buffer
        */
    record[next].key[strlen(key)]=0;
    return true;
}

uint32_t SvcDataDictionary::hash(const char* s){
    //uint32 djb2 string hash
    uint32_t h = 5381;
    int c;
    while (c = (*s++)){h = ((h << 5) + h) + c;}
    return h;
}
bool SvcDataDictionary::create(const char* key,int32_t val,uint32_t* owner){
    if (!copyKey(key)) return false;
    record[next].val.int32_val = val;
    record[next].pval = 0;
    record[next].owner = owner;
    record[next].record_type = DDRT_READ;
    record[next].key_hash = hash(key);
    next++;
    return true;
}

bool SvcDataDictionary::create(const char* key,int32_t val){
    if (!copyKey(key)) return false;
    record[next].val.int32_val = val;
    record[next].pval = 0;
    record[next].owner = 0;
    record[next].record_type = DDRT_READWRITE;
    record[next].data_type = DDDT_INT32;
    record[next].key_hash = hash(key);
    next++;
    return true;
}

bool SvcDataDictionary::create(const char* key,float32_t val){
    if (!copyKey(key)) return false;
    record[next].val.float32_val = val;
    record[next].pval = 0;
    record[next].owner = 0;
    record[next].record_type = DDRT_READWRITE;
    record[next].data_type = DDDT_FLOAT32;
    record[next].key_hash = hash(key);
    next++;
    return true;
}

int32_t SvcDataDictionary::read(const char* key){
    uint32_t h;
    h = hash(key);
    for(int i=0;i<DATADICT_KEYVALUE_PAIRS;i++){
        //key found
        if (record[i].key_hash == h) return record[i].val.int32_val;
    }
    //key not found
    return 0;
}

float32_t SvcDataDictionary::readf(const char* key){
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

bool SvcDataDictionary::update(const char* key,int32_t val){
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

bool SvcDataDictionary::update(const char* key,float32_t val){
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
void SvcDataDictionary::printStats(){

}

void SvcDataDictionary::printDictionary(){
    //todo: print out in JSON format
    //Serial.flush();
    Serial.print(F("DD {"));

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

