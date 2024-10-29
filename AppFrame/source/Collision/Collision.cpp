#include "DxLib.h"
#include "Collision.h"

float CheckAngle(VECTOR p1, VECTOR p2, VECTOR p3) {

	VECTOR v1 = VSub(p1, p2);
	VECTOR v2 = VSub(p3, p2);

	float radp2 = VDot(v1, v2);

	if (radp2 < 0) {
		return false;
	}
	else {
		return true;
	}
}

// 0�`1�̊ԂɃN�����v
void clamp01(float& v) {
	if (v < 0.0f)
		v = 0.0f;
	else if (v > 1.0f)
		v = 1.0f;
}


bool isParallel(VECTOR p1, VECTOR p2) {

	//�O�ώ��
	VECTOR v = VCross(p1, p2);

	//�T�C�Y����
	float d = VSize(v);

	// �덷�͈͓��Ȃ畽�s�Ɣ���
	if (-0.0001f < d && d < 0.0001f) {

		return true;
	}

	return false;
}

float CheckDistPointLine(VECTOR p1, VECTOR p2a, VECTOR p2b, VECTOR* h, float* t) {

	//p1 :���߂����_ 
	//p2a:�����̍��W1
	//p2b:�����̍��W2

	//p2a��p2b�Œ����
	VECTOR v2 = VSub(p2b, p2a);

	//������̓_H�����߂�
	//�����@���@p2+��v2
	//�܂��������߂�
	*t = VDot(v2, VSub(p1, p2a)) / VDot(v2, v2);

	//�����ɓ��Ă͂߂�
	*h = VAdd(p2a, VGet(v2.x * *t, v2.y * *t, v2.z * *t));

	//p1�Ƃg�̋������Ƃ�
	VECTOR in = VSub(p1, *h);
	float length = sqrt(VDot(in, in));

	return length;
}

float CheckDistPointSegment(VECTOR p1, VECTOR p2a, VECTOR p2b, VECTOR* h, float* t) {

	//p1 :���߂����_ 
	//p2a:�����̎n�_
	//p2b:�����I�_

	//�Ƃ肠���������Ƃ̍ŒZ����
	float length = CheckDistPointLine(p1, p2a, p2b, h, t);


	//���ς�0�����Ȃ�_���m�̍ŒZ�����ɂ���
	//������p1a��
	if (CheckAngle(p1, p2a, p2b) == false) {

		*h = p2a;
		VECTOR in = VSub(p1, p2a);
		length = sqrt(VDot(in, in));
	}
	//������p1b��
	else if (CheckAngle(p1, p2b, p2a) == false) {

		*h = p2b;
		VECTOR in = VSub(p1, p2b);
		length = sqrt(VDot(in, in));
	}

	return length;
}

float CheckDistLineLine(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b,
	VECTOR* h1, VECTOR* h2, float* t1, float* t2) {

	//p1a:�����̍��W1
	//p1b:�����̍��W2

	//p2a:�����̍��W1
	//p2b:�����̍��W2

	//���W����璼�����ލ쐬
	VECTOR v1 = VSub(p1b, p1a);
	VECTOR v2 = VSub(p2b, p2a);

	//���s�Ȃ��_�Ԃ̍ŒZ����
	if (isParallel(v1, v2) == true) {
		//p1�Ƃg�̋������Ƃ�
		VECTOR in = VSub(p1a, p2a);
		*h1 = p1a;
		float length = CheckDistPointSegment(p1a, p2a, p2b, h2, t2);

		return length;
	}


	float dv1v2 = VDot(v1, v2);
	float dv1v1 = VDot(v1, v1);
	float dv2v2 = VDot(v2, v2);
	VECTOR vp2p1 = VSub(p1a, p2a);

	float a = VDot(v1, v1);
	float b = VDot(v1, v2);
	float c = VDot(v2, v2);
	float d = VDot(v1, VSub(p1a, p2a));
	float e = VDot(v2, VSub(p1a, p2a));

	//
	*t1 = (dv1v2 * VDot(v2, vp2p1) - dv2v2 * VDot(v1, vp2p1))
		/ (dv1v1 * dv2v2 - dv1v2 * dv1v2);

	//
	*h1 = VAdd(p1a, VScale(v1, *t1));

	//
	*t2 = VDot(v2, VSub(*h1, p2a)) / dv2v2;

	//
	*h2 = VAdd(p2a, VScale(v2, *t2));

	VECTOR in = VSub(*h1, *h2);
	float length = sqrt(VDot(in, in));

	return length;
}

float CheckDistSegment(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b,
	VECTOR* h1, VECTOR* h2, float* t1, float* t2) {


	VECTOR v1 = VSub(p1b, p1a);
	VECTOR v2 = VSub(p2b, p2a);

	if (isParallel(v1, v2) == true) {
		//p1�Ƃg�̋������Ƃ�
		VECTOR in = VSub(p1a, p2a);

		float length = CheckDistPointSegment(p2a, p1a, p1b, h1, t1);
		length = CheckDistPointSegment(p1a, p2a, p2b, h2, t2);

		return length;
	}
	else {
		float length = CheckDistLineLine(p1a, p1b, p2a, p2b, h1, h2, t1, t2);

		if (
			0.0f <= *t1 && *t1 <= 1.0f &&
			0.0f <= *t2 && *t2 <= 1.0f
			) {

			return length;
		}
	}

	clamp01(*t1);
	*t1 = 1;
	*h1 = VAdd(p1a, VScale(v1, *t1));
	float length = CheckDistPointSegment(p1a, p2a, p2b, h2, t2);
	if (0.0f <= *t1 && *t1 <= 1.0f)
		return length;

	clamp01(*t2);
	*h2 = VAdd(p2a, VScale(v2, *t2));
	float length2 = CheckDistPointSegment(p2a, p1a, p1b, h1, t1);
	if (0.0f <= *t2 && *t2 <= 1.0f)
		return length;
}

bool CheckHitPointCapsule(VECTOR p1a, VECTOR p1b, VECTOR p2, float r1, float r2) {

	VECTOR c1 = VGet(0, 0, 0), c2 = VGet(0, 0, 0);
	float f1 = 0, f2 = 0;

	float MaxLength = r1 + r2;
	float NowLength = CheckDistPointSegment(p2, p1a, p1b, &c2, &f2);

	if (MaxLength > NowLength) {

		return true;
	}

	return false;
}

bool CheckHitCapsule(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, float r1, float r2) {

	VECTOR c1 = VGet(0,0,0), c2 = VGet(0, 0, 0);
	float f1 = 0, f2 = 0;

	float MaxLength = r1 + r2;
	float NowLength = CheckDistSegment(p1a, p1b, p2a, p2b, &c1, &c2, &f1, &f2);

	if (MaxLength > NowLength) {

		return true;
	}
	
	return false;
}

VECTOR CheckCapsule(VECTOR p1a, VECTOR p1b, float r1, VECTOR p2a, VECTOR p2b, float r2) {

	VECTOR c1, c2;
	float f1 = 0, f2 = 0;

	//�����ŐG��ĂȂ���Ԃōő�̓�_�Ԃ̋������Ƃ�
	float _Max = r1 + r2;
	float MaxLen = CheckDistSegment(VGet(0, 0, 0), VGet(0, 50, 0), VGet(_Max, 0, 0), VGet(_Max, 50, 0), &c1, &c2, &f1, &f2);

	//�������獡�̏�Ԃ̃J�v�Z����̋������Ƃ�
	float Len = CheckDistSegment(p1a, p1b, p2a, p2b, &c1, &c2, &f1, &f2);

	//�������m�̍ŒZ��������ŋߓ_����Ƃ��Ċp�x���o���Ă���P�ʉ�
	VECTOR rad = VSub(c1, c2);
	rad = VNorm(rad);

	if (Len < _Max) {

		float SubLen = MaxLen - Len;
		VECTOR v = VScale(rad, SubLen);

		return v;
	}


	return VGet(0, 0, 0);
}

VECTOR MakeWallLap(MV1_COLL_RESULT_POLY HitNorm, VECTOR MoveV) {

	//�@���x�N�g��(V)�����߂邽�߂̌���
				// V = F * aN
				// F(�ړ��x�N�g��),a(-F * N),N(�@���x�N�g���i�P�ʉ�������j)

				// �@���x�N�g��
	VECTOR Normal = HitNorm.Normal;
	Normal = VNorm(Normal);

	// �ړ��x�N�g��
	VECTOR FrontV = MoveV;

	// �ړ��x�N�g����-1��������iReverseMove�j
	VECTOR RMove = VScale(FrontV,-1);

	//�ǂɑ΂��ĕ��s�ȃx�N�g��(�ǎC��)���߂�̂Ɏg�����
	//�@���Ƌt�ړ��x�N�g���̓���
	float a = VDot(RMove, Normal);

	//�ǂɑ΂��ĕ��s�ȃx�N�g��(�ǎC��)���߂�̂Ɏg������̂Q
	//�ړ��x�N�g���ɑ������߂̃x�N�g��
	Normal = VScale(Normal, a);

	//�ǎC��x�N�g���쐬
	//WR = wall rapping = �ǎC��
	VECTOR WR = VAdd(FrontV, Normal);

	return WR;
}

float VectorAbs(VECTOR v)
{
	float v_comp[] = {v.x,v.y,v.z};
	int length = sizeof(v_comp) / sizeof(v_comp[0]);
	for(int i=0;i<length;++i)
	{
		v_comp[i] = abs(v_comp[i]);
	}
	VECTOR absV = VGet(v_comp[0], v_comp[1], v_comp[2]);
	return  sqrt(VSquareSize(absV));
}

bool CheckHitPointCorn(VECTOR top, VECTOR bottom, float r, VECTOR p)
{
	float h = VectorAbs(VSub(bottom, top));
	VECTOR PnT = VSub(p, top);
	VECTOR BnT = VSub(bottom, top);
	if(0.0f <= VDot(PnT,BnT) &&
		VDot(PnT,BnT) <= h*h &&
		VectorAbs(VCross(PnT, BnT)) <= VectorAbs(PnT)*r*h/sqrt(r*r+h*h))
	{
		return true;
	}
	return false;
}
