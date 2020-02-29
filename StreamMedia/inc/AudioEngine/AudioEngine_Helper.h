#ifndef AUDIO_ENGINE_API_H
#define AUDIO_ENGINE_API_H

#include "AudioEngine.h"

#ifdef WIN32
#ifdef NDEBUG
//#pragma comment(lib, "../AudioEngineSDK/Release/AudioEngine.lib")
#else
#pragma comment(lib, "../../AudioEngineSDK/Debug/AudioEngine.lib")
#endif  //debug
#endif


/*˵��:
* ���ļ�������AudioEngine �ڲ��ļ������ڵ�������������AudioEngineʱ��
* ���ٵ����ļ�������������ֻ��Ҫ����������������´���:

 //#include "AudioEngine_Helper.h"

 AudioEngine_Helper the_audio_engine_instance_;

 AudioEngine *audio_engine = AudioEngine_Helper::Instance().getAudioEngine();

 audio_engine->Init();
 //ʹ����������
 .... 
*
*/


class AudioEngine_Helper;
/*�ڵ��õ�app �ж����ʵ��*/
extern	AudioEngine_Helper the_audio_engine_instance_;


class AudioEngine_Helper{

public:
	bool enable_fec;
	static AudioEngine_Helper* Instance()
	{ 
		return &the_audio_engine_instance_; 
	};

public:
	AudioEngine_Helper():
	  audio_engine_(CreateAudioEngine()),
	  enable_fec(false)
	  {
		  audio_engine_->Init();

		  /*����Ĭ������*/
		  audio_engine_->VOE_SetAECMode(1,kEcDefault);
		  audio_engine_->VOE_SetAGCMode(1,kAgcDefault);
		  audio_engine_->VOE_SetNSMode(1,kNsHighSuppression);
	  };

	  ~AudioEngine_Helper()
	  {
		  if(audio_engine_->IsInitialized())
				  audio_engine_->Terminate();

		  DeleteAudioEngine(audio_engine_);
	  };

	  AudioEngine *getAudioEngine(){ 
		  return audio_engine_ ;
	  };
private:
	AudioEngine *audio_engine_;
};

#endif