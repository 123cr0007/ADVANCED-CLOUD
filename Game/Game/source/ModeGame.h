#pragma once

#include "DxLib.h"
#include "AppFrame.h"
#include "ObjectBase.h"

#include <string>
#include <vector>

#include "EnergyGauge.h"
#include "Camera.h"

#include "Player.h"
#include "Fellow.h"
#include "Bullet.h"
#include "enemy.h"
#include "UI.h"
#include "Item.h"
#include "EnemyManager.h"
#include "LoadEnemy.h"
#include "FadeInOut.h"
#include "ModeMenu.h"
#include "ModeGameOver.h"
#include "EffectManager.h"

// ÉÇÅ[Éh
class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	bool Respawn();

	void AddEnemy(int id, int level, VECTOR sPos, VECTOR gPos);

protected:

	Camera* _cam;
	
	Player* _pl;
	Fellow* _tab;
	EnergyGauge* _energy;
	BulletClass* _bul;
	//LoadEnemy* _loadEn;
	EnemyManager* _enemyM;
	UI* _ui;
	EffectManager* _ef;
	Item* _item;
	Fade* _fade;
	//ParticleServer* _PartServer;
	std::vector<Enemy*> _vEnemy;

	//std::thread SetJson;
private:
	bool EnterStory=false;
	int BgWhite;
};

