#pragma once
#include "ObjObject3d.h"
#include "Vector3.h"
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <json.hpp>

/// <summary>
/// ���x���f�[�^�ǂݍ���
/// </summary>
class LevelDataLoader
{
	/// <summary>
	/// ���x���f�[�^
	/// </summary>
	struct LevelData
	{
		//�I�u�W�F�N�g1���̃f�[�^
		struct ObjectData {
			std::string fileName;	//�t�@�C����
			Vector3 translation;	//���s�ړ�
			Vector3 rotation;		//��]�p
			Vector3 scaling;		//�X�P�[�����O
			bool isParent;
			std::vector<ObjectData> children;
		};

		//�I�u�W�F�N�g�̃R���e�i
		std::vector<ObjectData> objects;
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	/// <returns>JSON�t�@�C���ǂݍ���</returns>
	static LevelDataLoader* Create(const std::string& filename);

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// ���f�����i�[
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	/// <param name="model">���f��</param>
	void InsertModel(const std::string& filename, ObjModel* model);

	/// <summary>
	/// �ǂݍ��񂾃��x���f�[�^�I�u�W�F�N�g����
	/// </summary>
	void CreateLevelDataObjects();


private: //�����o�֐�
	/// <summary>
	/// �t�@�C���ǂݍ���
	/// </summary>
	/// <param name="filename">�t�@�C����</param>
	void LoadFile(const std::string& filename);

	/// <summary>
	/// �V�[�����ċA�I�ɉ�͂���
	/// </summary>
	/// <param name="object">�I�u�W�F�N�g</param>
	void ParseSceneRecursive(const nlohmann::json& object);


private: //�ÓI�����o�ϐ�
	//�f�B���N�g���p�X
	const static std::string directoryPath;

private: //�����o�ϐ�
	//���f���f�[�^�R���e�i
	std::map<std::string, ObjModel*> models;
	//�I�u�W�F�N�g�f�[�^�R���e�i
	std::vector<std::unique_ptr<ObjObject3d>> objects;
	//JSON�����񂩂�𓀂���f�[�^
	nlohmann::json deserialized;
	//���x���f�[�^�i�[�p�C���X�^���X
	LevelData* levelData;
};