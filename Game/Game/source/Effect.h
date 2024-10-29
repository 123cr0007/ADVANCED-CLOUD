#pragma once
#include"AppFrame.h"

//�X�e�[�^�X�Ǘ��p
constexpr auto EF_SCALE = 10;

//���̉��G��Ȃ�
constexpr auto EF_JUMP_FLOAT = 100;		//���V�W�����v
constexpr auto EF_JUMP_NORMAL = 102;	//�ʏ�W�����v
constexpr auto EF_JUMP_AVOID = 103;		//����W�����v
constexpr auto EF_JUMP_ATTACK = 104;	//�U���W�����v
constexpr auto EF_ENEMY_BOMB = 105;		//����
constexpr auto EF_CRYSTAL = 106;		//���ƁA�点���ɉQ�������i�΁j
constexpr auto EF_DIAMOND = 107;		//�傫�ȃ_�C���^�̌��������Ă����i���j
constexpr auto EF_RECOVERY = 108;		//�c�����̃����O�ƕ��U������i�΁j
constexpr auto EF_DIFFUSE = 109;		//���U������i�΁j	�����1
constexpr auto EF_TORNADO_MOVE = 110;	//�点���`�����i�΁j	����̂Q�@
constexpr auto EF_TORNADO = 111;		//�点���`�����i�΁j	�v���C���[�Ǐ]�Ȃ�
constexpr auto EF_GATHER = 112;			//�����W�܂�i�΁j	����̂R

constexpr auto EFFECT_PLAYER = 0;	//�v���C���[�p�G�t�F�N�g���ʗp
constexpr auto EFFECT_ENEMY = 1;	//�G�l�~�[�p�G�t�F�N�g���ʗp
constexpr auto EFFECT_OTHER = 2;	//���̑��I�u�W�F�N�g�p�G�t�F�N�g���ʗp

class Effect
{
public:
 	Effect(int Effect, VECTOR pos);
	~Effect();
	void Process(VECTOR pl_pos,VECTOR en_pos, VECTOR item_pos);
	void Render();

public:
	bool isPlay;//�G�t�F�N�g���Đ����邩

protected:
	int handle;
	int play_handle;	//�Đ��G�t�F�N�g�w��n���h��
	VECTOR efPos;		//�G�t�F�N�g�\�����W
	bool isMove;		//�G�t�F�N�g�͒Ǐ]���邩
	int ef_controller;	//�G�t�F�N�g�̏��L��
};