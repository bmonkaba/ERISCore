//template_start
// {filenames} filenames (*.cpp,*.h)
// {AudioTypeName}
// {AudioBlockSize}
//template_end

#include <Arduino.h>
#include "{filenames}.h"

class {AudioTypeName} : public AudioStream
{
public:
        AudioEffectTemplate() : AudioStream({AudioBlockSize}, inputQueueArray);
        virtual void update(void);
private:
        audio_block_t *inputQueueArray[{AudioBlockSize}];
};


void AudioEffectTemplate::update(void){

}
