#include "Utility.h"
#include "..//Mymath/mymath.h"
#include <math.h>
//�}�E�X�ł̎��_�ړ�
void ViewMouseMove()
{

}

//2�_�Ԃ̑傫���v��
float PosMagnitude(VECTOR target_pos, VECTOR my_pos)
{
	VECTOR v= VSub(target_pos, my_pos);
	float magnitude = sqrt(VSquareSize(v));
	return magnitude;
}

//�^�[�Q�b�g�Ɍ���
VECTOR RotateToTarget(VECTOR target_pos,VECTOR my_pos)
{
	//���������߂�
	//�x�N�g���̐��K��
	if(my_pos.x != target_pos.x || my_pos.y != target_pos.y || my_pos.z != target_pos.z)
	{
		return VNorm(VSub(my_pos, target_pos));
	}
	else 
	{ 
		return VGet(0, 0, 0); 
	}
}

//���g�����ւ���(VECTOR�p)
void swapVECTOR(VECTOR pos1,VECTOR pos2)
{
	VECTOR tmp = pos1;
	pos1 = pos2;
	pos2 = tmp;
}

//���g�����ւ���(int�p)
void swapInt(int x, int y)
{
	int tmp = x;
	x = y;
	y = tmp;
}

//������S������
float DeciToPer(float i)
{
	float per = i * 100;
	return per;
}

//�S������������
float PerToDeci(float i)
{
	float deci = i / 100;
	return deci;
}

//Particle
VECTOR Deg3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed)
{

	float rad_xy = DegToRad(deg_xy);
	float rad_zx = DegToRad(deg_zx);

	//���݂̊p�x�Ńx�N�g�������
	VECTOR vDir = VGet(0, 0, 0);


	VECTOR willpos = VScale(position, speed);

	float r = VSize(VSub(willpos, position));

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