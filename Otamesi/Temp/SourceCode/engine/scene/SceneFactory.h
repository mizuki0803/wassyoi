#pragma once
#include "AbstractSceneFactory.h"
#include <string>

/// <summary>
/// �V�[���H��
/// </summary>
class SceneFactory : public AbstractSceneFactory
{
public:
	/// <summary>
	/// �V�[������
	/// </summary>
	/// <param name="sceneName">�V�[����</param>
	/// <returns>���������V�[��</returns>
	BaseScene* CreateScene(const std::string& sceneName) override;
};

