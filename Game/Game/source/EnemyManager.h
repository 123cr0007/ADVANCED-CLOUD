#pragma once
#include"enemy.h"

class EnemyManager
{
public:
	static EnemyManager* Instance;
	static EnemyManager* GetInstance() { return (EnemyManager*)Instance; }
	EnemyManager();
	~EnemyManager();
	void Add(Enemy* en);
	void Del(Enemy* en);
	void Process( VECTOR target_pos);
	void Render();
	void IsDead();	//死んだかどうか
	void Spawn();	//追加予約
	VECTOR GetEnPos() { return deadPos; }	//死んだ敵の座標
	int GetEnEffect() { return deadEf; }	//死んだ敵のエフェクト


protected:
	bool isProcess;
	VECTOR deadPos=VGet(0,0,0);
	int deadEf = 0;
private:
	std::vector<Enemy*> _vEnemy;
	std::vector<Enemy*> _vAdd;
	std::vector<Enemy*> _vDel;
public:

	// ゲッター
	const std::vector<Enemy*> GetEnemy() {

		return _vEnemy;
	}
};