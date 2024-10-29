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
	bool LoadJson();//Json読み込み
	bool SetJson();//Jsonのデータを一つ一つnewしてAddに渡す

protected:

	struct EnData
	{
		int id;
		int level;
		VECTOR _sPos;
		VECTOR _gPos;
	};
	EnData _data;

	//これは仮で入れておく用のコンテナ
	std::vector<EnData> _vData;
};