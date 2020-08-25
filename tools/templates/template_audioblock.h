/*node_template_start
{filenames:{filenames}} (*.cpp,*.h)
{AudioTypeName:{AudioTypeName}}
{AudioBlockSize:0}
/*template_end

/*
!node_documentation_start  (do not edit this line)
Node Template Keys:
{name:"{AudioTypeName}"}
{short_name:"{filenames}"}
{inputs:""}
{outputs:""}

Node Document Keys:
{type:""}
{summary:""}
{connections:""}
{functions:""}

Node Connection Keys:
{direction and port}  #IN or OUT
{purpose}

!node_documentation_end (do not edit this line)
*/




#ifndef {filenames}_h_
#define {filenames}_h_
#include "Arduino.h"
#include "AudioStream.h"
#include "utility/dspinst.h"

#define SAMPLES_PER_MSEC (AUDIO_SAMPLE_RATE_EXACT/1000.0)

class {AudioTypeName} : public AudioStream
{
public:
	{AudioTypeName}() : AudioStream({AudioBlockSize}, inputQueueArray) {
	}
	using AudioStream::release;
	virtual void update(void);

private:
  audio_block_t *inputQueueArray[{AudioBlockSize}];
#endif
