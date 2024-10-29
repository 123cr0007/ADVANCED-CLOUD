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
	void IsDead();	//���񂾂��ǂ���
	void Spawn();	//�ǉ��\��
	VECTOR GetEnPos() { return deadPos; }	//���񂾓G�̍��W
	int GetEnEffect() { return deadEf; }	//���񂾓G�̃G�t�F�N�g


protected:
	bool isProcess;
	VECTOR deadPos=VGet(0,0,0);
	int deadEf = 0;
private:
	std::vector<Enemy*> _vEnemy;
	std::vector<Enemy*> _vAdd;
	std::vector<Enemy*> _vDel;
public:

	// �Q�b�^�[
	const std::vector<Enemy*> GetEnemy() {

		return _vEnemy;
	}
};