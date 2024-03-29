#include "pch.h"
#include "IAVCom.h"

AVFormatContext* IAVCom::OpenMediaInput(LPCSTR fileName)
{
	AVFormatContext* pCtx = NULL;
	if (avformat_open_input(&pCtx, fileName, NULL, NULL)) {
		throw exception("avformat_open_input failed");
		return NULL;
	}
	if (avformat_find_stream_info(pCtx, NULL) < 0) {
		throw exception("avformat_find_stream_info failed");
		return NULL;
	}
	return pCtx;
}

AVFormatContext* IAVCom::OpenMediaOutput(LPCSTR fileName)
{
	AVFormatContext* pCtx = NULL;
	avformat_alloc_output_context2(&pCtx, NULL, NULL, fileName);
	if (!pCtx) {
		throw exception("avformat_alloc_output_context2 failed");
		return NULL;
	}
	if (avio_open(&pCtx->pb, fileName, AVIO_FLAG_WRITE) < 0) {
		throw exception("avio_open failed");
		return NULL;
	}
	return pCtx;
}

AVStream* IAVCom::QueryStream(AVFormatContext* pCtx, AVMediaType type)
{
	for (int i = 0; i < pCtx->nb_streams; i++)
	{
		AVCodecParameters* pLocalCodecParameters = NULL;
		AVStream* pStream = pCtx->streams[i];
		pLocalCodecParameters = pStream->codecpar;
		if (pLocalCodecParameters->codec_type == type) {
			return pStream;
		}
	}
	return NULL;
}

int64_t IAVCom::StreamCopy(AVFormatContext* in_ctx, int in_index, AVFormatContext* out_ctx, int out_index, FRAMECALLBACK callback)
{
	AVStream* in_stream = in_ctx->streams[in_index];
	AVStream* out_stream = out_ctx->streams[out_index];
	AVPacket packet;
	//output each packet
	int64_t count = 0;
	while (true) {
		if (av_read_frame(in_ctx, &packet) < 0)
			break;
		if (packet.stream_index != in_index) {
			av_packet_unref(&packet);
			continue;
		}
		//copy packet
		packet.stream_index = out_stream->index;
		packet.pts = av_rescale_q_rnd(packet.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_NEAR_INF));
		packet.dts = av_rescale_q_rnd(packet.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_NEAR_INF));
		packet.duration = av_rescale_q(packet.duration, in_stream->time_base, out_stream->time_base);
		packet.pos = -1;
		if (av_interleaved_write_frame(out_ctx, &packet) < 0) {
			throw exception("av_interleaved_write_frame failed");
			break;
		}
		av_packet_unref(&packet);
		count++;
		if (callback) {
			callback(count);
		}
	}
	return count;
}

int64_t IAVCom::StreamFrames(AVFormatContext* in_ctx, int in_index)
{
	AVStream* in_stream = in_ctx->streams[in_index];
	AVPacket packet;
	while (true) {
		if (av_read_frame(in_ctx, &packet) < 0)
			break;
		av_packet_unref(&packet);
	}
	av_seek_frame(in_ctx, -1, 0, AVSEEK_FLAG_ANY);
	return in_stream->nb_index_entries;
}

AVStream* IAVCom::AppendStream(AVFormatContext* pCtx)
{
	return avformat_new_stream(pCtx, NULL);
}

void IAVCom::CopyCodec(AVCodecParameters* dst, const AVCodecParameters* src)
{
	if (avcodec_parameters_copy(dst, src) < 0)
		throw exception("Can not copy codec");
	dst->codec_tag = 0;
}

void IAVCom::WriteHeader(AVFormatContext* pCtx, AVDictionary* opts)
{
	if (avformat_write_header(pCtx, &opts) < 0) {
		throw exception("avformat_write_header failed");
	}
}

void IAVCom::WriteTrailer(AVFormatContext* pCtx)
{
	if (av_write_trailer(pCtx))
		throw exception("av_write_trailer failed");
}

