#include "LevelDataLoader.h"
#include <fstream>

const std::string LevelDataLoader::directoryPath = "Resources/levels/";

LevelDataLoader* LevelDataLoader::Create(const std::string& filename)
{
	//�������jUI�̃C���X�^���X�𐶐�
	LevelDataLoader* jsonLoader = new LevelDataLoader();
	if (jsonLoader == nullptr) {
		return nullptr;
	}

	//�t�@�C���ǂݍ���
	jsonLoader->LoadFile(filename);

	return jsonLoader;
}

void LevelDataLoader::Update()
{
	//�I�u�W�F�N�g�X�V
	for (const std::unique_ptr<ObjObject3d>& object : objects) {
		object->Update();
	}
}

void LevelDataLoader::Draw()
{
	//�I�u�W�F�N�g�`��
	for (const std::unique_ptr<ObjObject3d>& object : objects) {
		object->Draw();
	}
}

void LevelDataLoader::DrawLightCameraView()
{
	//�I�u�W�F�N�g�`��
	for (const std::unique_ptr<ObjObject3d>& object : objects) {
		object->DrawLightCameraView();
	}
}

void LevelDataLoader::InsertModel(const std::string& filename, ObjModel* model)
{
	//���f�����R���e�i�Ɋi�[
	models.insert(std::make_pair(filename, model));
}

void LevelDataLoader::CreateLevelDataObjects()
{
	//���x���f�[�^�i�[�p�C���X�^���X�𐶐�
	levelData = new LevelData();

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (nlohmann::json& object : deserialized["objects"]) {
		ParseSceneRecursive(object);
	}

	//���x���f�[�^����I�u�W�F�N�g�𐶐��A�z�u
	for (auto& objectData : levelData->objects) {
		// �t�@�C��������o�^�ς݃��f��������
		ObjModel* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.fileName);
		if (it != models.end()) {
			model = it->second;
		}

		//3D�I�u�W�F�N�g�𐶐�
		std::unique_ptr<ObjObject3d> newObject;
		newObject.reset(ObjObject3d::Create(model));
		//���s�ړ�
		newObject->SetPosition(objectData.translation);
		//��]�p
		newObject->SetRotation(objectData.rotation);
		//�X�P�[�����O
		newObject->SetScale(objectData.scaling);
		//�V���h�E�}�b�v�ł̉e�`����\�ɂ��Ă���
		newObject->SetIsShadowMap(true);

		//�z��ɓo�^
		objects.push_back(std::move(newObject));
	}
}

void LevelDataLoader::LoadFile(const std::string& filename)
{
	//�A�����ăt���p�X�𓾂�
	const std::string fullpath = directoryPath + filename;

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	//��
	file >> deserialized;

	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name =
		deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);
}

void LevelDataLoader::ParseSceneRecursive(const nlohmann::json& object)
{
	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	assert(object.contains("type"));

	//��ʂ��擾
	std::string type = object["type"].get<std::string>();

	//��ނ��Ƃ̏���
	//MESH
	if (type.compare("MESH") == 0) {
		//�v�f�ǉ�
		levelData->objects.emplace_back(LevelData::ObjectData{});
		//���ǉ������v�f�̎Q�Ƃ𓾂�
		LevelData::ObjectData& objectData = levelData->objects.back();

		if (object.contains("file_name")) {
			//�t�@�C����
			objectData.fileName = object["file_name"];
		}

		//�g�����X�t�H�[���̃p�����[�^�ǂݍ���
		nlohmann::json transform = object["transform"];
		//���s�ړ�
		objectData.translation.x = (float)transform["translation"][1];
		objectData.translation.y = (float)transform["translation"][2];
		objectData.translation.z = -(float)transform["translation"][0];
		//��]�p
		objectData.rotation.x = -(float)transform["rotation"][1];
		objectData.rotation.y = -(float)transform["rotation"][2];
		objectData.rotation.z = (float)transform["rotation"][0];
		//��]�p
		objectData.scaling.x = (float)transform["scaling"][1];
		objectData.scaling.y = (float)transform["scaling"][2];
		objectData.scaling.z = (float)transform["scaling"][0];
	}

	//�q�m�[�h������΍ċA����
	if (object.contains("children")) {
		for (const nlohmann::json& child : object["children"]) {
			ParseSceneRecursive(child);
		}
	}
}
