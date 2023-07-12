#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include <cereal/types/string.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>
#include <cereal/archives/binary.hpp>
#include <array>
#include <iostream>
#include <fstream>

class JsonLoader
{
private:

	struct Json
	{
		std::string name;
		std::array<float,3> cameraDist;
		int installationSurface;
		std::array<int,3> mapSize;
		std::vector<std::vector<std::vector<int>>> map;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(name), CEREAL_NVP(cameraDist), CEREAL_NVP(installationSurface), CEREAL_NVP(mapSize), CEREAL_NVP(map));
		}
	};

	struct Binary
	{
		int is2D;
		int moveSurface;
		std::array<int, 3> mapChip;
		std::array<float, 3> cameraPos;
		std::array<int ,2> cameraPosPhase;
		std::array<float,3> cameraRota;
		std::array<float, 3> playerPos;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(is2D), CEREAL_NVP(moveSurface), CEREAL_NVP(mapChip),
				CEREAL_NVP(cameraPos), CEREAL_NVP(cameraPosPhase), CEREAL_NVP(cameraRota), CEREAL_NVP(playerPos));
		}
	};

	enum class transform {
		translation,
		rotation,
		scaling,
		size,
	};

public:

	/// <summary>
	/// json�̓ǂݍ���
	/// </summary>
	/// <param name="_fileName">�t�@�C����</param>
	/// <param name="_object">�I�u�W�F�N�g���</param>
	static void LoadNlohmannJson(const std::string& _fileName, std::vector<std::array<DirectX::XMFLOAT3, int(transform::size)>>* _object);

	/// <summary>
	/// �o��
	/// </summary>
	/// <param name="_fileName">�t�@�C���̖��O</param>
	/// <param name="_cameraDist">�J�����̋���</param>
	/// <param name="_mapSize">�}�b�v�T�C�Y</param>
	/// <param name="_map">�}�b�v�`�b�v</param>
	static void SerializeJsonMap(const std::string& _fileName = "out.json", const std::array<float, 3> _cameraDist = {},
		const std::array<int, 3> _mapSize = { 10,10,10 }, std::vector<std::vector<std::vector<int>>> _map = { {{}} });

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_fileName">�t�@�C���̖��O</param>
	/// <param name="_cameraDist">�J�����̋���</param>
	/// <param name="_installationSurface">�����ڒn��</param>
	/// <param name="_map">�}�b�v�`�b�v</param>
	static bool DeserializeJsonMap(const std::string _fileName, std::array<float, 3>* _cameraDist,
		int* _installationSurface, std::vector<std::vector<std::vector<int>>>* _map);

	/// <summary>
	/// �o��
	/// </summary>
	/// <param name="_fileName">�t�@�C���̖��O</param>
	/// <param name="_is2D">2D���ǂ���</param>
	/// <param name="_moveSurface">�ړ���</param>
	/// <param name="_mapChip">�v���C���[�}�b�v�`�b�v</param>
	/// <param name="_cameraPos">�J�������W</param>
	/// <param name="_cameraPosPhase">�J�����̕���</param>
	/// <param name="_cameraRota">�J�����p�x</param>
	/// <param name="_playerPos">�v���C���[���W</param>
	static void SerializeBinary(const std::string& _fileName, const bool _is2D, const int _moveSurface,
		const std::array<int, 3>& _mapChip, const std::array<float, 3>& _cameraPos, const std::array<int, 2>& _cameraPosPhase,
		const std::array<float, 3>& _cameraRota, const std::array<float, 3>& _playerPos);

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_fileName">�t�@�C���̖��O</param>
	/// <param name="_is2D">2D���ǂ���</param>
	/// <param name="_moveSurface">�ړ���</param>
	/// <param name="_mapChip">�v���C���[�}�b�v�`�b�v</param>
	/// <param name="_cameraPos">�J�������W</param>
	/// <param name="_cameraPosPhase">�J�����̕���</param>
	/// <param name="_cameraRota">�J�����p�x</param>
	/// <param name="_playerPos">�v���C���[���W</param>
	static bool DeserializeBinary(const std::string _fileName, bool* _is2D, int* _moveSurface,
		std::array<int, 3>* _mapChip, std::array<float, 3>* _cameraPos, std::array<int, 2>* _cameraPosPhase,
		std::array<float, 3>* _cameraRota, std::array<float, 3>* _playerPos);

private:

	static const std::string base_directory;
};