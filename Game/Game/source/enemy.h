#pragma once
#include "AppFrame.h"
#include "Bullet.h"
#include "ObjectBase.h"
#include "Player.h"

//#include <string>
//#include <vector>
//#include <math.h>
//#include <stdio.h>

//�X�e�[�^�X�Ǘ��p
constexpr auto ENEMY_SHOT_RANGE = 1000;	//�G�̐��ʕ����̒T�m�͈�
constexpr auto ENEMY_COUNT_MAX = 10;	//�����̃^�C�}�[�𐧌䂷�邽��
constexpr auto ENEMY_SEARCH_AREA = 1000;//�G�̒T�m�p�~����ʂ̔��a
constexpr auto ENEMY_SHOT_WAIT = 3;
//���̉�������Ȃ�
constexpr auto EN_TYPE_DEAD = 10;//�Ȃ�
constexpr auto EN_TYPE_MOVE = 11;//2�_�Ԉړ�

class Enemy :public ObjectBase 
{
public:

	static Enemy* ENInstance;
	static Enemy* GetENInstance() { return (Enemy*)ENInstance; }

	Enemy(int _id, VECTOR sPos, VECTOR gPos);
	~Enemy();
	bool Initialize();
	bool Process(VECTOR target_pos);
	bool Render();

	bool Move();//�ړ�
	void PosSwap();//���W����
	void Rotate();//��]
	void Rotate(VECTOR target_pos);//�v���C���[�։�]
	bool Search(VECTOR target_pos);//�v���C���[���G
	bool SearchOut(VECTOR target_pos);//�v���C���[���͈͊O�֓��S
	void Shot(VECTOR target_pos);//�v���C���[�֍U��
	void Reload();//�e�̃����[�h
	bool Wait(int i,int second);//�҂�����
	void MakeArea();//����G���A�̍쐬
	bool RotToGoal();//�S�[���֌�����
	bool Dead();//���S

	
	int GetUseEnrgy() { return UseEnergy; };//�U���q�b�g�ŏ������G�l���M�[���擾
	VECTOR GetBlPos(int i) { return BLPos[i]; }//�e�̍��W���擾
	float GetBlR(int i) { return BlRadius[i]; }//�e�̔��a���擾
	int GetCheckActive(int i) { return CheckBlActive[i]; }//�e�̃A�N�e�B�u��Ԃ��擾

	void SetIsPatrol(bool Patrol) { IsPatrol = Patrol; }//�p�g���[�������ǂ�����ݒ�
	void SetIsFind(bool Find) { IsFind = Find; }//�v���C���[�𔭌��������ǂ�����ݒ�
	void SetIsVoice(bool Voice) { IsVoice = Voice; }//����炷���ǂ�����ݒ�
	void SetIsShot(bool Shot) { IsShot = Shot; }//�U�������ǂ�����ݒ�


	//���֘A
	bool GetIsPatrol() { return IsPatrol; };//�p�g���[�������ǂ������擾
	bool GetIsFind() { return IsFind; };//�v���C���[�𔭌��������ǂ������擾
	bool GetIsVoise() { return IsVoice; };//����炷���ǂ������擾
	bool GetIsShot() { return IsShot; };//�U�������ǂ������擾


	void SetRestEnergy(int Energy) { RestEnergy = Energy; };//�c��̃G�l���M�[��ݒ�
	void SetUseEnergy(int Energy) { UseEnergy = Energy; }; //�g���G�l���M�[��ݒ�
	void SetStartPos(VECTOR sPos) { _sPos = sPos; }//�X�^�[�g���W��ݒ�
	void SetGoalPos(VECTOR gPos) { _gPos = gPos; }//�S�[�����W��ݒ�
	
	int id;
	VECTOR _sPos;	//�G�X�^�[�g���W
	VECTOR _gPos;	//�G�S�[�����W
	VECTOR Rot;		//��]
	int RotateCnt = 0;
	VECTOR _aPos;	//����G���A�̒��
	VECTOR _tPos;	//����G���A�̒��_
	float r;		//����G���A�̔��a
	int level;		//�G�̃��x��
	int spd;		//�G�̈ړ����x�i����g���ĂȂ��j
	int type;		//�G�̓�����ύX���邽�߂̂���
	bool shot = false;	//�e����p

	int count[ENEMY_COUNT_MAX];	//�҂����ԃJ�E���^�[(�����g������)

protected:
	//��������A�j���[�V����
	float _total_time;
	float _play_time;
	int _attach_index;

	//���f���֘A
	int Tex_color[3];
	int En_handle;//�G�{�̃��f��
	int _anim;
	//�G�l���M�[�֘A
	int RestEnergy; //�c��̃G�l���M�[
	int UseEnergy;  //�g���G�l���M�[
	float Check;

	//���֘A
	bool IsPatrol;//�p�g���[�������ǂ���
	bool IsFind;//�v���C���[�𔭌��������ǂ���
	bool IsVoice;//����炷���ǂ���
	bool IsShot;//�U�������ǂ���
	bool IsSearchOut;//�v���C���[���͈͊O�֓��S�������ǂ���

	//���N���X�֘A
	VECTOR FlBlPos;//�e�̍��W
	float flBlR;//�e�̔��a

	VECTOR BLPos[BULLET_MAX];
	float BlRadius[BULLET_MAX];
	int CheckBlActive[BULLET_MAX];
	int mode;

private:

	BulletClass bullet;
};

enum class MODE:int
{
	search,
	attack,
	idle,
	rotate
};