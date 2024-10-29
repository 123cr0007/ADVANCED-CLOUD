#pragma once
#include "AppFrame.h"

constexpr auto BULLET_MAX = 20;
constexpr auto BULLET_TEX_ENEMY = 0;
constexpr auto BULLET_TEX_FELLOW = 1;
class BulletClass
{
public:
	BulletClass();
	~BulletClass();
	int AddBullet(VECTOR my_pos, VECTOR target_pos,int handle);//�e����
	void BulletProcess(int i);//�e����
	void BulletRender(int i);//�e�`��

	void Move(int i);//�e�ړ�
	void Destroy(int i);//�e����
	void Rotate(int i);//���˕�����]
	VECTOR GetPos(int i) { return bl[i].pos; };//�e�̍��W�擾
	float GetR(int i) { return bl[i].CollisionR; };//�e�̔��a�擾
	bool GetActive(int i) {return  bl[i].active; };//�e�̃A�N�e�B�u��Ԏ擾
	int GetCheckHit(int i) { return  bl[i].CheckHit; };//�e�̓����蔻��擾

	void SetCheckHit(int i, bool n) { bl[i].CheckHit = n; };//�e�̓����蔻��ݒ�
	
private:
	struct Bullet
	{
		VECTOR tPos;//�^�[�Q�b�g���W
		VECTOR _sPos;//�X�^�[�g���W
		VECTOR pos;//�����W	�R���W�����t����Ƃ��͂������ɂ��肢���܂��B
		VECTOR rot;//�ꉞ����
		bool active;//�e�̎g�p��
		float CollisionR;//�e�̔��a�i���������j
		bool CheckHit;
	};
	struct Bullet bl[BULLET_MAX];
	int handle;
	int tex_en;
	int tex_fl;
};