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

void AVCom::setCallbackAudio(FRAMECALLBACK callback)
{
	m_callbackAudio = callback;
}

void AVCom::setCallbackVideo(FRAMECALLBACK callback)
{
	m_callbackVideo = callback;
}

int64_t AVCom::getAudioFrame()
{
	if (m_pInputAudioStream)
		return m_pInputAudioStream->nb_index_entries;
	else
		return -1;
}

int64_t AVCom::getVideoFrame()
{
	if (m_pInputVideoStream)
		return m_pInputVideoStream->nb_index_entries;
	else
		return -1;
}

bool AVCom::OpenStream()
{
	if (!(m_audio && m_video && m_path)) {
		return false;
	}
	try {
		m_pAudioFormatContext = OpenMediaInput(m_audio);
		m_pVideoFormatContext = OpenMediaInput(m_video);
		m_pOutFormatContext = OpenMediaOutput(m_path);
		//find audio stream
		m_pInputAudioStream=QueryStream(m_pAudioFormatContext, AVMEDIA_TYPE_AUDIO);
		if(!m_pInputAudioStream)
			throw exception("No compatible stream found.");
		//find video stream
		m_pInputVideoStream = QueryStream(m_pVideoFormatContext, AVMEDIA_TYPE_VIDEO);
		if (!m_pInputVideoStream)
			throw exception("No compatible stream found.");
		//get streams
		m_pOutAudioStream = AppendStream(m_pOutFormatContext);
		m_pOutVideoStream = AppendStream(m_pOutFormatContext);
		//get codec
		CopyCodec(m_pOutAudioStream->codecpar, m_pInputAudioStream->codecpar);
		CopyCodec(m_pOutVideoStream->codecpar, m_pInputVideoStream->codecpar);

		StreamFrames(m_pAudioFormatContext, m_pInputAudioStream->index);
		StreamFrames(m_pVideoFormatContext, m_pInputVideoStream->index);
	}
	catch (exception& ex) {
		return false;
	}
	return true;
}

bool AVCom::WriteFile()
{
	if (!(m_pAudioFormatContext && m_pVideoFormatContext && m_pOutFormatContext)) {
		return false;
	}
	try {
		WriteHeader(m_pOutFormatContext);
		StreamCopy(m_pAudioFormatContext, m_pInputAudioStream->index, m_pOutFormatContext, m_pOutAudioStream->index, m_callbackAudio);
		StreamCopy(m_pVideoFormatContext, m_pInputVideoStream->index, m_pOutFormatContext, m_pOutVideoStream->index, m_callbackVideo);
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
	m_pInputAudioStream = NULL;
	m_pInputVideoStream = NULL;
	m_pOutAudioStream = NULL;
	m_pOutVideoStream = NULL;
	m_callbackAudio = NULL;
	m_callbackVideo = NULL;
}

void AVCom::dispose()
{
	if (m_pAudioFormatContext)
		avformat_close_input(&m_pAudioFormatContext);
	if (m_pVideoFormatContext)
		avformat_close_input(&m_pVideoFormatContext);
	if (m_pOutFormatContext && m_pOutFormatContext->pb)
		avio_closep(&m_pOutFormatContext->pb);
	if (m_pOutFormatContext)
		avformat_free_context(m_pOutFormatContext);
}
