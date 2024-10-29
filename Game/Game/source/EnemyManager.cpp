//敵のコンテナのマネージャー

#include"EnemyManager.h"
EnemyManager* EnemyManager::Instance = nullptr;
EnemyManager::EnemyManager()
{
	Instance = this;
	isProcess = false;
}

EnemyManager::~EnemyManager()
{

}

void EnemyManager::Add(Enemy* en)
{
	if(!isProcess)
	{
		_vEnemy.push_back(en);
	}
	else
	{
		_vAdd.push_back(en);
	}
}

void EnemyManager::Del(Enemy* en)
{
	if(isProcess)
	{
		for(auto ite = _vEnemy.begin(); ite != _vEnemy.end();)
		{
			if((*ite) == en)
			{
				delete (*ite);
				ite = _vEnemy.erase(ite);
			}
			else
			{
				++ite;
			}
		}
	}
	else
	{
		_vDel.push_back(en);
	}
}

void EnemyManager::Process(VECTOR target_pos)
{
	for(auto ite : _vAdd)
	{
		Add(ite);
	}

	_vAdd.clear();
	isProcess = true;
	
	for(auto ite:_vEnemy)
	{
		ite->Process(target_pos);	//敵の処理
	}

	IsDead();

	isProcess = false;
	
	for(auto ite : _vDel)
	{
		Del(ite);
	}
	_vDel.clear();
}

void EnemyManager::Render()
{
	for(auto ite:_vEnemy)
	{
		ite->Render();
	}
}

//追加予約
void EnemyManager::Spawn()
{
	for(auto ite = _vEnemy.begin(); ite != _vEnemy.end(); ++ite)
	{

	}
}

//削除予約
void EnemyManager::IsDead()
{
	for(auto ite :_vEnemy)
	{
		if(ite->Dead())
		{
			Del(ite);	//削除予約
		}
	}
}





