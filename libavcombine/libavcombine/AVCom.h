#pragma once
#include "avc_def.h"
#include "IAVCom.h"
class DLL_API AVCom :IAVCom
{
public:
	AVCom();
	~AVCom();
	void setAudio(LPCSTR audio);
	void setVideo(LPCSTR video);
	void setOutPath(LPCSTR path);
	void setCallbackAudio(FRAMECALLBACK callback);
	void setCallbackVideo(FRAMECALLBACK callback);
	int64_t getAudioFrame();
	int64_t getVideoFrame();
	virtual bool OpenStream();
	virtual bool WriteFile();
private:
	void init();
	void dispose();
private:
	LPCSTR m_audio;
	LPCSTR m_video;
	LPCSTR m_path;
	AVFormatContext* m_pAudioFormatContext;
	AVFormatContext* m_pVideoFormatContext;
	AVFormatContext* m_pOutFormatContext;
	AVStream* m_pInputAudioStream;
	AVStream* m_pInputVideoStream;
	AVStream* m_pOutAudioStream;
	AVStream* m_pOutVideoStream;
	FRAMECALLBACK m_callbackAudio;
	FRAMECALLBACK m_callbackVideo;
};

