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

	//XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);

	//�����傫�����Ď����ɂ��Ȃ�̂ŁA�\�ߏ��������Ă���
	ChangeVolume(0.1f);
}

void Audio::Finalize()
{
	//xAudio2���
	xAudio2.Reset();

	//�T�E���h�f�[�^���
	std::map<std::string, SoundData>::iterator it = soundDatas.begin();
	for (; it != soundDatas.end(); ++it) {
		SoundDataUnload(&it->second);
	}
	soundDatas.clear();

}

void Audio::LoadWave(const std::string& filename)
{
	//�d���ǂݍ��݂̏ꍇ���������ɔ�����
	if (soundDatas.find(filename) != soundDatas.end()) { return; }

	//�f�B���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	std::string fullPath = directoryPath + filename;

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fullPath, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) { assert(0); }
	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) { assert(0); }

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};
	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) { assert(0); }

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));
	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0 || strncmp(data.id, "LIST", 4) == 0) {
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);
		//�ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	//,wav�f�[�^�̓ǂݍ���
	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	//Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Wave�t�@�C�������
	file.close();

	//return����ׂ̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	//�T�E���h�f�[�^��A�z�z��Ɋi�[
	soundDatas.insert(std::make_pair(filename, soundData));
}

void Audio::SoundDataUnload(SoundData* soundData)
{
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::PlayWave(const std::string& filename, const bool isLoop)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);
	//���ǂݍ��݂̌��o
	assert(it != soundDatas.end());
	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//���[�v�Đ�
	if (isLoop) {
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}
	//���[�v���Ȃ�
	else {
		buf.LoopCount = 0;
		buf.LoopBegin = 0;
		buf.LoopLength = 0;
	}

	//�g�`�f�[�^�̍Đ�
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start();
}

void Audio::StopWave(const std::string& filename)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas.find(filename);
	//���ǂݍ��݂̌��o
	assert(it != soundDatas.end());
	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;

	//soundVoice��nullptr�̏ꍇ���������ɔ�����
	if (soundData.pSourceVoice == nullptr) { return; }

	//��~����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//�g�`�f�[�^�̒�~
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