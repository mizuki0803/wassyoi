#pragma once
#include <wrl.h>
#include <xaudio2.h>
#include <cstdint>
#include <string>
#include <map>

#pragma comment(lib, "xaudio2.lib")

/// <summary>
/// オーディオ
/// </summary>
class Audio final
{
public: //エイリアス
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //サブクラス
	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];	//チャンク毎のID
		int32_t size;	//チャンクサイズ
	};

	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;	//RIFF
		char type[4];	//WAVE
	};

	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;	//fmt
		WAVEFORMATEX fmt;	//波形フォーマット
	};

	//音声データ
	struct SoundData
	{
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭アドレス
		BYTE* pBuffer;
		//バッファのサイズ
		unsigned int bufferSize;
		//SourceVoice
		IXAudio2SourceVoice* pSourceVoice;
	};

private: //シングルトン化
	//コンストラクタを隠蔽
	Audio() = default;
	//デストラクタを隠蔽
	~Audio() = default;
public:
	//コピーコンストラクタを無効化
	Audio(const Audio& audio) = delete;
	//代入演算子を無効化
	void operator = (const Audio& audio) = delete;

public: //メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>オーディオ</returns>
	static Audio* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	void Initialize(const std::string& directoryPath = "Resources/sound/");

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAVデータ読み込み
	/// </summary>
	/// <param name="filename">ファイル名</param>
	void LoadWave(const std::string& filename);

	/// <summary>
	/// サウンドデータ解放
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	void SoundDataUnload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	/// <param name="isLoop">ループ再生か</param>
	void PlayWave(const std::string& filename, const bool isLoop);

	/// <summary>
	/// 音声停止	
	/// </summary>
	/// <param name="filename">サウンドデータ</param>
	void StopWave(const std::string& filename);

	/// <summary>
	/// 音量変更
	/// </summary>
	/// <param name="volume">音量</param>
	void ChangeVolume(const float volume);

private: //メンバ変数
	//xAudio
	ComPtr<IXAudio2> xAudio2;
	//マスターボイス
	IXAudio2MasteringVoice* masterVoice;
	//サウンドデータの連想配列
	std::map<std::string, SoundData> soundDatas;
	//サウンド格納ディレクトリ
	std::string directoryPath;
};