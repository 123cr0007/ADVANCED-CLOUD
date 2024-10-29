#pragma once
#include "Dxlib.h"
#include <math.h>
#include<stdio.h>
#include<time.h>

constexpr float PI = 3.141592653589793;

namespace calculation
{
	
	float RadToDeg(float rad);

	float DegToRad(float deg);

	/*
	�p�x���烉�W�A���ɕϊ����ĒP�ʃx�N�g�������߂�
	�܂��X�P�[�������߂��
	�P�ʃx�N�g���̂܂܎g���ꍇscale�ɂ�0.0f������
	*/
	VECTOR RadUnitVector(float deg, float scale);
	//���W�A���ňړ��ʒu�����߂�
	VECTOR RadPositionVector(VECTOR position,float deg, float speed);
	//3D
	VECTOR Rad3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed);

	//�f�o�b�N�p



}; 