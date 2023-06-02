#pragma once
#include "ObjObject3d.h"

class Player;

class PlayerEffect :
    public ObjObject3d
{
public:
	static PlayerEffect *Create(ObjModel *model, float scale);

	/// <summary>
	/// çXêV
	/// </summary>
	void Update(Player *player);

private:

	void ScaleUp();
	const float maxScale{ 4.0f };
	const float minScale{ 1.0f };
	float scale{ 0 };
};

