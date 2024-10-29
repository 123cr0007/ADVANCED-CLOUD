#include "calculation.h"

// radian��degree
float calculation::DegToRad(float deg) {
	return deg * PI / 180.0f;		// �p�x��degree����radian�ɕϊ�
}

// degree��radian
float calculation::RadToDeg(float rad) {
	return rad * 180.0f / PI;		// �p�x��radian����degree�ɕϊ�
}

VECTOR calculation::RadPositionVector(VECTOR position,float deg, float speed)
{
	float rad = RadToDeg(deg);

	//���݂̊p�x�Ńx�N�g�������
	VECTOR vDir = VGet(0, 0, 0);
	vDir.x = cos(rad);
	vDir.y = sin(rad);
	// ������x�N�g�����u�P�ʃx�N�g���v�i�X�J����1�̃x�N�g���j�ɂ���
	VECTOR vUnit = VNorm(vDir);
	// �P�ʃx�N�g���Ɉړ����x���������A�ړ��x�N�g�������
	VECTOR vMove = VScale(vUnit, speed);
	// �ړ��x�N�g�����ʒu�ɉ��Z����
	position = VAdd(position, vMove);
	return position;
}

VECTOR calculation::RadUnitVector(float deg, float scale)
{

	float rad = RadToDeg(deg);

	//���݂̊p�x�Ńx�N�g�������
	VECTOR vDir = VGet(0, 0, 0);
	vDir.x = cos(rad);
	vDir.y = sin(rad);
	// ������x�N�g�����u�P�ʃx�N�g���v�i�X�J����1�̃x�N�g���j�ɂ���
	VECTOR vUnit = VNorm(vDir);
	if (scale > 0) {
		// �P�ʃx�N�g���̃X�P�[����ς���
		VECTOR vMove = VScale(vUnit, scale);
	}
	else{
		return VGet(0, 0, 0);
	}

}

VECTOR calculation::Rad3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed)
{

	float rad_xy = DegToRad(deg_xy);
	float rad_zx = DegToRad(deg_zx);

	//���݂̊p�x�Ńx�N�g�������
	VECTOR vDir = VGet(0, 0, 0);
	

	VECTOR willpos = VScale(position, speed);
	
	float r = VSize(VSub(willpos,position));

	vDir.x = r * sin(rad_xy) * cos(rad_zx);
	vDir.y = r * cos(rad_xy);
	vDir.z = r * sin(rad_xy) * sin(rad_zx);
	
	// ������x�N�g�����u�P�ʃx�N�g���v�i�X�J����1�̃x�N�g���j�ɂ���
	VECTOR vUnit = VNorm(vDir);

	// �P�ʃx�N�g���Ɉړ����x���������A�ړ��x�N�g�������
	VECTOR vMove = VScale(vUnit, speed);
	 //�ړ��x�N�g�����ʒu�ɉ��Z����
	position = VAdd(position, vMove);

	return	position;
}