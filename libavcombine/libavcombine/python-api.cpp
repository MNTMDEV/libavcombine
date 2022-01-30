#include "pch.h"
#include "AVCom.h"

#define C_EXPORT extern "C" DLL_API

C_EXPORT AVCom* getInstance() {
	return new AVCom();
}

C_EXPORT void releaseInstance(AVCom* inst) {
	delete inst;
}

C_EXPORT void setAudio(AVCom* inst,LPCSTR path) {
	inst->setAudio(path);
}

C_EXPORT void setVideo(AVCom* inst, LPCSTR path) {
	inst->setVideo(path);
}

C_EXPORT void setOutPath(AVCom* inst, LPCSTR path) {
	inst->setOutPath(path);
}

C_EXPORT bool OpenStream(AVCom* inst) {
	return inst->OpenStream();
}

C_EXPORT bool WriteToFile(AVCom* inst) {
	return inst->WriteFile();
}

C_EXPORT int64_t getAudioFrame(AVCom* inst) {
	return inst->getAudioFrame();
}

C_EXPORT int64_t getVideoFrame(AVCom* inst) {
	return inst->getVideoFrame();
}

C_EXPORT void setCallbackAudio(AVCom* inst, FRAMECALLBACK callback) {
	inst->setCallbackAudio(callback);
}

C_EXPORT void setCallbackVideo(AVCom* inst,FRAMECALLBACK callback) {
	inst->setCallbackVideo(callback);
}