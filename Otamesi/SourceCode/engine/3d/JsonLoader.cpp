#include <JsonLoader.h>

void JsonLoader::SerializeJson(const std::string& _fileName, const float _cameraDist,
    const std::array<int, 3> _mapSize, std::vector<std::vector<std::vector<int>>> _map)
{
    Json x;
    x.name = _fileName;
    x.cameraDist = 30.0f;
	x.mapSize = _mapSize;
	x.map = _map;
	// ファイル出力
    std::ofstream os(_fileName, std::ios::out);
    cereal::JSONOutputArchive archiveFile(os);
    x.serialize(archiveFile);
}

bool JsonLoader::DeserializeJson(const std::string _fileName,
    float* _cameraDist, std::vector<std::vector<std::vector<int>>>* _map)
{
    Json x;

    std::ifstream os(_fileName, std::ios::in);
    if (!os.is_open()) {
        return false;
    }
    cereal::JSONInputArchive archive(os);
    x.serialize(archive);

    *_cameraDist = x.cameraDist;
    *_map = x.map;

    return true;
}

void JsonLoader::SerializeBinary(const std::string& _fileName, const bool _is2D, const int _moveSurface,
    const std::array<int, 3>& _mapChip, const std::array<float, 3>& _cameraPos, const std::array<int, 2>& _cameraPosPhase, const std::array<float, 3>& _playerPos)
{
    Binary x;
    x.is2D = int(_is2D);
    x.moveSurface = _moveSurface;
    x.mapChip = _mapChip;
    x.cameraPos = _cameraPos;
    x.cameraPosPhase = _cameraPosPhase;
    x.playerPos = _playerPos;

    // ファイル出力
    std::ofstream os("Resources/binary/" + _fileName + ".binary", std::ios::out | std::ios::binary);
    cereal::BinaryOutputArchive archive(os);
    x.serialize(archive);
}

bool JsonLoader::DeserializeBinary(const std::string _fileName, bool* _is2D, int* _moveSurface,
    std::array<int, 3>* _mapChip, std::array<float, 3>* _cameraPos, std::array<int, 2>* _cameraPosPhase, std::array<float, 3>* _playerPos)
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
    *_playerPos = x.playerPos;

    return true;
}
