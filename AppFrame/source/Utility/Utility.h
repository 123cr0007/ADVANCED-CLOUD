#pragma once
#include "DxLib.h"


//�}�E�X�ł̎��_�ړ�
void ViewMouseMove();

//2�_�Ԃ̑傫���v��
float PosMagnitude(VECTOR target_pos,VECTOR my_pos);//�^�[�Q�b�g�Ɍ���

VECTOR RotateToTarget(VECTOR target_pos,VECTOR my_pos);

//���g�����ւ���(�x�N�g���p)
void swapVECTOR(VECTOR pos1, VECTOR pos2);
//���g�����ւ���(int�p)
void swapInt(int x, int y);

////degree�p����radian�p��
//float	DegToRad(int x);
////radian�p����degree�p��
//float	RadToDeg(float x);

//��������S������
float DeciToPer(float x);
//�S�������珬����
float PerToDeci(float x);
//Particle
VECTOR Deg3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed);