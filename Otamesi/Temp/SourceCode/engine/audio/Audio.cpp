#include "Audio.h"
#include <cassert>
#include <fstream>


Audio* Audio::GetInstance()
{
	static Audio instance;

	return &instance;
}

void Audio::Initialize(const std::string& directoryPath)
{
	this->directoryPath = directoryPath;

	HRESULT result;

	//XAudioエンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//音が大きすぎて耳が痛くなるので、予め小さくしておく
	ChangeVolume(0.1f);
}

void Audio::Finalize()
{
	//xAudio2解放
	xAudio2.Reset();

	//サウンドデータ解放
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();
	for (; it != soundDatas.end(); ++it) {
		SoundDataUnload(&it->second);
	}
	soundDatas.clear();

}

void Audio::LoadWave(const std::string& filename)
{
	//重複読み込みの場合何もせずに抜ける
	if (soundDatas.find(filename) != soundDatas.end()) { return; }

	//ディレクトリパスとファイル名を連結してフルパスを得る
	std::string fullPath = directoryPath + filename;

	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.wavファイルをバイナリモードで開く
	file.open(fullPath, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	assert(file.is_open());

	//RIFFヘッダーの読み込み
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) { assert(0); }
	//タイプがWAVEかチェック
	if (strncmp(riff.type, "WAVE", 4) != 0) { assert(0); }

	//Formatチャンクの読み込み
	FormatChunk format = {};
	//チャンクヘッダーの確認
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }

	//チャンク本体の読み込み
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Dataチャンクの読み込み
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNKチャンクを検出した場合
	if (strncmp(data.id, "JUNK", 4) == 0 || strncmp(data.id, "LIST", 4) == 0) {
		//読み取り位置をJUNKチャンクの終わりまで進める
		file.seekg(data.size, std::ios_base::cur);
		//再読み込み
		file.read((char*)&data, sizeof(data));
	}

	//,wavデータの読み込み
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Dataチャンクのデータ部（波形データ）の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

	//returnする為の音声データ
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//サウンドデータを連想配列に格納
	soundDatas.insert(std::make_pair(filename, soundData));
}

void Audio::SoundDataUnload(SoundData* soundData)
{
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::PlayWave(const std::string& filename, const bool isLoop)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);
	//未読み込みの検出
	assert(it != soundDatas.end());
	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	//波形フォーマットを元にSourceVoiceの生成
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//ループ再生
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	//ループしない
	else {
		buf.LoopCount = 0;
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}

	//波形データの再生
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start();
}

void Audio::StopWave(const std::string& filename)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);
	//未読み込みの検出
	assert(it != soundDatas.end());
	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	//soundVoiceがnullptrの場合何もせずに抜ける
	if (soundData.pSourceVoice == nullptr) { return; }

	//停止する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの停止
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Stop();
	result = soundData.pSourceVoice->FlushSourceBuffers();
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	assert(SUCCEEDED(result));
}

void Audio::ChangeVolume(const float volume)
{
	masterVoice->SetVolume(volume);
}