#include <Arduino.h>
#include <string.h>
#include <arm_math.h>
#include "svcSerialCommandInterface.h"

/**
 * @file svcDataDictionary.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief Data Dictionary service 
 * provides an interface to a data dictionary service\n
 * the service shall provide a CRUD (create,read,update,destroy) interface\n
 * data shall be globaly readable\n
 * create operation shall offer an ownership interface requiring a pointer to the owner\n
 * update and destroy operations on owned keys can only be made by the owner\n
 * the owner pointer shall never be dereferenced or accessed by the service; it only serves as an owner ID\n
 * NULL pointers are OK. They can be utilized to create globaly modifiable (update,destroy) records\n
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

#define DATADICT_KEYVALUE_PAIRS 80
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

typedef union pointer_container{
    int16_t* pint16_val;
    uint16_t* puint16_val;
    int32_t* pint32_val;
    uint32_t* puint32_val;
    float32_t* pfloat16_val;
    float32_t* pfloat32_val;
} pointer_container;

typedef union value_container{
    int32_t int32_val;
    float32_t float32_val;
    pointer_container pointer_container_val;
} value_container;


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
    svcDataDictionaryRecordType record_type;
    svcDataDictionaryDataType data_type;
} svcDataDictionaryRecord __attribute__ ((aligned (4)));

class SvcDataDictionary{
    
    public:
        SvcDataDictionary();
        uint32_t hash(const char* s);
        bool create(const char* key,int32_t val,uint32_t* owner);
        bool create(const char* key,int32_t val);
        bool create(const char* key,float32_t val);
        int32_t read(const char* key);
        float32_t readf(const char* key);
        bool update(const char* key,int32_t val,uint32_t* owner);
        bool update(const char* key,float32_t val,uint32_t* owner);
        bool update(const char* key,int32_t val);
        bool update(const char* key,float32_t val);
        bool increment(const char* key);
        //serial interface
        void printStats();
        void printDictionary(SvcSerialCommandInterface* sci);
    
    private:
        svcDataDictionaryRecord record[DATADICT_KEYVALUE_PAIRS];
        uint16_t next;
        bool copyKey(const char* key);

};

#endif