#include "ErisUtils.h"


/**
 * @brief a safer strncpy 
 * 
 * strncpy will fill the entire available buffer until full\n 
 * if the source string is larger than the destination then the null termination char will be omitted when using the standard lib strncpy.
 * this wrapper always writes a null char at the end of the destination to protect for this condition.
 * a string length check is then performed.
 * if the source is larger than the destination a partial copy will be made, up to the size of the available dest buffer, the return value will be FALSE in this case
 * other wise the string will be copied and return TRUE
 * 
 * @param dest 
 * @param source 
 * @param dest_size 
 * @return true 
 * @return false 
 */
bool safer_strncpy(char* dest,const char* source,uint16_t dest_size){
    strncpy(dest,source,dest_size);
    dest[dest_size-1] = 0; //ensure copied string is null terminated
    if (strlen(source) > dest_size) return false;
    return true;
}

