#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include "stdlib.h"

//�p123���݊p�����ׂ�
float CheckAngle(VECTOR p1, VECTOR p2, VECTOR p3);

// ���s�֌W�����ׂ�
// 0�������畽�s
bool isParallel(VECTOR p1, VECTOR p2);

void clamp01(float& v);

//�_�ƒ����̍ŒZ����
float CheckDistPointLine(VECTOR p1, VECTOR p2a, VECTOR p2b, VECTOR* h, float* t);

//�_�Ɛ����̍ŒZ����
float CheckDistPointSegment(VECTOR p1, VECTOR p2a, VECTOR p2b, VECTOR* h, float* t);

//�����̒����̍ŒZ����
float CheckDistLineLine(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, VECTOR* h1, VECTOR* h2, float* t1, float* t2);

//�������m�̍ŒZ����
float CheckDistSegment(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, VECTOR* h1, VECTOR* h2, float* t1, float* t2);

//�J�v�Z�����m���������Ă邩�ǂ���
bool CheckHitCapsule(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, float t1, float t2);

//�_�ƃJ�v�Z��
bool CheckHitPointCapsule(VECTOR p1a, VECTOR p1b, VECTOR p2, float t1, float t2);

//�J�v�Z�����m�̉����߂�����
VECTOR CheckCapsule(VECTOR p1a, VECTOR p1b, float r1, VECTOR p2a, VECTOR p2b, float r2);

//�ǎC��x�N�g���̍쐬
VECTOR MakeWallLap(MV1_COLL_RESULT_POLY HitNorm, VECTOR MoveV);

//abs�֐����g���ăx�N�g���̐�Βl�����߂�
float VectorAbs(VECTOR v);

//�_�Ɖ~���̓����蔻��
bool CheckHitPointCorn(VECTOR top,VECTOR bottom,float r,VECTOR p);