#pragma once

//TYPE DEF
#ifndef LPCSTR
typedef const char* LPCSTR;
#endif // !LPCSTR

// callback
typedef void (*FRAMECALLBACK)(int64_t);

//DLL EXPORT
#ifdef DLL_EXPORT
#define DLL_API		__declspec(dllexport)
#else
#define DLL_API		__declspec(dllimport)
#endif

//FFMPEG
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/timestamp.h>
}
