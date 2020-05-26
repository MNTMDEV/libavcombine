#include "pch.h"
#include "AVCom.h"

AVCom::AVCom()
{
	init();
}

AVCom::~AVCom()
{
	dispose();
}

void AVCom::setAudio(LPCSTR audio)
{
	m_audio = audio;
}

void AVCom::setVideo(LPCSTR video)
{
	m_video = video;
}

void AVCom::setOutPath(LPCSTR path)
{
	m_path = path;
}

bool AVCom::OpenStream()
{
	if(!(m_audio&& m_video&& m_path))
		return false;
	try {
		m_pAudioFormatContext = OpenMediaInput(m_audio);
		m_pVideoFormatContext = OpenMediaInput(m_video);
		m_pOutFormatContext = OpenMediaOutput(m_path);
		//find audio stream
		QueryStream(m_pAudioFormatContext, AVMEDIA_TYPE_AUDIO, &m_pAudioCodecParameters, &audio_stream_index);
		//find video stream
		QueryStream(m_pVideoFormatContext, AVMEDIA_TYPE_VIDEO, &m_pVideoCodecParameters, &video_stream_index);
		//get streams
		m_pOutAudioStream = AppendStream(m_pOutFormatContext);
		m_pOutVideoStream = AppendStream(m_pOutFormatContext);
		//get codec
		CopyCodec(m_pOutAudioStream->codecpar, m_pAudioCodecParameters);
		CopyCodec(m_pOutVideoStream->codecpar, m_pVideoCodecParameters);
	}
	catch (exception& ex) {
		return false;
	}
	return true;
}

bool AVCom::WriteFile()
{
	try {
		WriteHeader(m_pOutFormatContext);
		StreamCopy(m_pAudioFormatContext, audio_stream_index, m_pOutFormatContext, m_pOutAudioStream->index);
		StreamCopy(m_pVideoFormatContext, video_stream_index, m_pOutFormatContext, m_pOutVideoStream->index);
		WriteTrailer(m_pOutFormatContext);
	}
	catch (exception& ex) {
		return false;
	}
	return true;
}

void AVCom::init()
{
	m_audio = NULL;
	m_video = NULL;
	m_path = NULL;
	m_pAudioFormatContext = NULL;
	m_pVideoFormatContext = NULL;
	m_pOutFormatContext = NULL;
	m_pAudioCodecParameters = NULL;
	m_pVideoCodecParameters = NULL;
	audio_stream_index = -1;
	video_stream_index = -1;
	m_pOutAudioStream = NULL;
	m_pOutVideoStream = NULL;
}

void AVCom::dispose()
{
	if(m_pAudioFormatContext)
		avformat_close_input(&m_pAudioFormatContext);
	if(m_pVideoFormatContext)
		avformat_close_input(&m_pVideoFormatContext);
	if(m_pOutFormatContext&&m_pOutFormatContext->pb)
		avio_closep(&m_pOutFormatContext->pb);
	if(m_pOutFormatContext)
		avformat_free_context(m_pOutFormatContext);
}
