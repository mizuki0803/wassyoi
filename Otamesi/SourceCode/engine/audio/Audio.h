#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#pragma comment(lib, "xaudio2.lib")

/// <summary>
/// �I�[�f�B�I
/// </summary>
class Audio final
{
public: //�G�C���A�X
	//namespace�ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private: //�T�u�N���X

	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];	//�`�����N����ID
		int32_t size;	//�`�����N�T�C�Y
	};

	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;	//RIFF
		char type[4];	//WAVE
	};

	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;	//fmt
		WAVEFORMATEX fmt;	//�g�`�t�H�[�}�b�g
	};

	//�����f�[�^
	struct SoundData
	{
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪�A�h���X
		BYTE* pBuffer;
		//�o�b�t�@�̃T�C�Y
		unsigned int bufferSize;
		//SourceVoice
		IXAudio2SourceVoice* pSourceVoice;
	};

public:

	enum class SoundName{
		bgm,//
		button,//�{�^��
		stage_change,//�X�e�[�W�Z���N�g�ł̕ύX
		d2_d3,//2d����3d�ϊ�
		d3_d2,//3d����2d�ϊ�
		clear,//�N���A
	};

	std::vector<std::string> name = {
		"bgm",
		"button",
		"stage_change",
		"d2_d3",
		"d3_d2",
		"clear",
	};

private: //�V���O���g����
	//�R���X�g���N�^���B��
	Audio() = default;
	//�f�X�g���N�^���B��
	~Audio() = default;
public:
	//�R�s�[�R���X�g���N�^�𖳌���
	Audio(const Audio& audio) = delete;
	//������Z�q�𖳌���
	void operator = (const Audio& audio) = delete;

public: //�����o�֐�
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns>�I�[�f�B�I</returns>
	static Audio* GetInstance();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="directoryPath">�f�B���N�g���p�X</param>
	void Initialize(const std::string& directoryPath = "Resources/sound/");

	/// <summary>
	/// �I������
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAV�f�[�^�ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void LoadWave(const std::string& filename);

	/// <summary>
	/// �T�E���h�f�[�^���
	/// </summary>
	/// <param name="soundData">�T�E���h�f�[�^</param>
	void SoundDataUnload(SoundData* soundData);

	/// <summary>
	/// �����Đ�
	/// </summary>
	/// <param name="soundData">�T�E���h�f�[�^</param>
	/// <param name="isLoop">���[�v�Đ���</param>
	void PlayWave(const SoundName filename, const bool isLoop = false);

	/// <summary>
	/// ������~	
	/// </summary>
	/// <param name="filename">�T�E���h�f�[�^</param>
	void StopWave(const SoundName filename);

	/// <summary>
	/// ���ʕύX
	/// </summary>
	/// <param name="volume">����</param>
	void ChangeVolume(const float volume);

private: //�����o�ϐ�
	//xAudio
	ComPtr<IXAudio2> xAudio2;
	//�}�X�^�[�{�C�X
	IXAudio2MasteringVoice* masterVoice;
	//�T�E���h�f�[�^�̘A�z�z��
	std::unordered_map<std::string, SoundData> soundDatas;
	//�T�E���h�i�[�f�B���N�g��
	std::string directoryPath;
};