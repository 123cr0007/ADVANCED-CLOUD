#pragma once
#include "AppFrame.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include <string>
#include <vector>
class LoadEnemy
{
public:
	LoadEnemy();
	~LoadEnemy();

	static LoadEnemy* LoadInstance;
	static LoadEnemy* GetInstance() { return(LoadEnemy*)LoadInstance; }
	bool Initialize();
	bool Process();
	bool LoadJson();//Json�ǂݍ���
	bool SetJson();//Json�̃f�[�^������new����Add�ɓn��

protected:

	struct EnData
	{
		int id;
		int level;
		VECTOR _sPos;
		VECTOR _gPos;
	};
	EnData _data;

	//����͉��œ���Ă����p�̃R���e�i
	std::vector<EnData> _vData;
};