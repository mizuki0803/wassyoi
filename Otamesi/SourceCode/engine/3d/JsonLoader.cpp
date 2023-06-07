#include <JsonLoader.h>
#include <json.hpp>

using njson = nlohmann::json;
const std::string JsonLoader::base_directory = "Resources/";

void JsonLoader::LoadNlohmannJson(const std::string& _fileName, std::vector<std::array<DirectX::XMFLOAT3, int(transform::size)>>* _object)
{
    const std::string fullpath = base_directory + _fileName + ".json";

	//�t�@�C���X�g���[��
	std::ifstream file;

	//�t�@�C�����J��
	file.open(fullpath);
	if (file.fail()) {
		assert(0);
	}

	//�𓀃f�[�^�ۑ���
	njson deserialized;

	//��
	file >> deserialized;

	//�f�[�^�����������̃`�F�b�N
	assert(deserialized.is_object());
	assert(deserialized.contains("name"));
	assert(deserialized["name"].is_string());

	//"name"�𕶎���Ƃ��Ď擾
	std::string name = deserialized["name"].get<std::string>();
	//���������x���f�[�^�t�@�C�����`�F�b�N
	assert(name.compare("scene") == 0);

	//"objects"�̑S�I�u�W�F�N�g�𑖍�
	for (njson& object : deserialized["objects"]) {
        std::array<DirectX::XMFLOAT3, int(transform::size)> addObject{};
        //�g�����X�t�H�[�����
		njson& transform = object["transform"];
		// ���s�ړ�
        addObject[int(transform::translation)].x = float(transform["translation"][0]) * 5.0f;
        addObject[int(transform::translation)].y = (float(transform["translation"][2]) * 5.0f) - 50.0f;
        addObject[int(transform::translation)].z = float(transform["translation"][1]) * 5.0f;
		// ��]�p
		addObject[int(transform::rotation)].x = -float(transform["rotation"][1]);
		addObject[int(transform::rotation)].y = -float(transform["rotation"][2]);
		addObject[int(transform::rotation)].z = float(transform["rotation"][0]);
		// �X�P�[�����O
        addObject[int(transform::scaling)].x = float(transform["scaling"][1]) * 5.0f;
        addObject[int(transform::scaling)].y = float(transform["scaling"][2]) * 5.0f;
        addObject[int(transform::scaling)].z = float(transform["scaling"][0]) * 5.0f;

        _object->emplace_back(addObject);
    }
}

void JsonLoader::SerializeJsonMap(const std::string& _fileName, const std::array<float, 3> _cameraDist,
    const std::array<int, 3> _mapSize, std::vector<std::vector<std::vector<int>>> _map)
{
    Json x;
    x.name = _fileName;
    x.cameraDist = _cameraDist;
	x.mapSize = _mapSize;
	x.map = _map;
	// �t�@�C���o��
    std::ofstream os(_fileName, std::ios::out);
    cereal::JSONOutputArchive archiveFile(os);
    x.serialize(archiveFile);
}

bool JsonLoader::DeserializeJsonMap(const std::string _fileName, std::array<float, 3>* _cameraDist,
    int* _installationSurface, std::vector<std::vector<std::vector<int>>>* _map)
{
    Json x;

    std::ifstream os(_fileName, std::ios::in);
    if (!os.is_open()) {
        return false;
    }
    cereal::JSONInputArchive archive(os);
    x.serialize(archive);

    *_cameraDist = x.cameraDist;
    *_installationSurface = x.installationSurface;
    *_map = x.map;

    return true;
}

void JsonLoader::SerializeBinary(const std::string& _fileName, const bool _is2D, const int _moveSurface, const std::array<int, 3>& _mapChip,
    const std::array<float, 3>& _cameraPos, const std::array<int, 2>& _cameraPosPhase, const std::array<float, 3>& _cameraRota, const std::array<float, 3>& _playerPos)
{
    Binary x;
    x.is2D = int(_is2D);
    x.moveSurface = _moveSurface;
    x.mapChip = _mapChip;
    x.cameraPos = _cameraPos;
    x.cameraPosPhase = _cameraPosPhase;
    x.cameraRota = _cameraRota;
    x.playerPos = _playerPos;

    // �t�@�C���o��
    std::ofstream os("Resources/binary/" + _fileName + ".binary", std::ios::out | std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    x.serialize(archive);
}

bool JsonLoader::DeserializeBinary(const std::string _fileName, bool* _is2D, int* _moveSurface, std::array<int, 3>* _mapChip,
    std::array<float, 3>* _cameraPos, std::array<int, 2>* _cameraPosPhase, std::array<float, 3>* _cameraRota, std::array<float, 3>* _playerPos)
{
    Binary x;

    std::ifstream os("Resources/binary/" + _fileName + ".binary", std::ios::in | std::ios::binary);
    if (!os.is_open()) {
        return false;
    }
    cereal::BinaryInputArchive archive(os);
    x.serialize(archive);

    *_is2D = bool(x.is2D);
    *_moveSurface = x.moveSurface;
    *_mapChip = x.mapChip;
    *_cameraPos = x.cameraPos;
    *_cameraPosPhase = x.cameraPosPhase;
    *_cameraRota = x.cameraRota;
    *_playerPos = x.playerPos;

    return true;
}
