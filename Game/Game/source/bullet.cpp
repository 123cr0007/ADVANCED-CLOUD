//�G�Ƃ������g�p����e�̏���
#include "AppFrame.h"
#include "Bullet.h"
#include "ApplicationMain.h"

BulletClass::BulletClass()
{
	//������
	for (int i = 0; i < BULLET_MAX; i++)
	{
		bl[i].pos = VGet(0.0f, 0.0f, 0.0f);
		bl[i].rot = VGet(0.0f, 0.0f, 0.0f);
		bl[i].active = false;
	}
	handle = MV1LoadModel("res/Enemy/Bullet/Bullet_v01.MV1");
	handle = MV1DuplicateModel(handle);
	tex_en = LoadGraph("res/Enemy/Bullet/Bullet_Texture_v01.png");
	tex_fl = LoadGraph("res/Enemy/Bullet/Bullet_Texture_v01.png");
}

BulletClass::~BulletClass()
{

}

//�e�ǉ�
int BulletClass::AddBullet(VECTOR my_pos, VECTOR target_pos,int handle_num)
{
	//�󂢂Ă���z���T��
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�󂢂Ă���z�����������
		if (!bl[i].active)
		{
			//���˂���e�̏�����
			bl[i]._sPos = my_pos;//�X�^�[�g���W
			bl[i].tPos = target_pos;//�^�[�Q�b�g���W

			//�e�̃e�N�X�`����ύX�i�����Ȃ��j
			switch(handle_num)
			{
			case 0:
				MV1SetTextureGraphHandle(handle,0,tex_en, TRUE);
				break;
			case 1:
				MV1SetTextureGraphHandle(handle, 0, tex_fl, TRUE);
				break;
			}
			//�^�[�Q�b�g���Ďw�肵���������i�΍������̂��߁j
			bl[i].tPos.x + float(rand() % 100);
			bl[i].tPos.y + float(rand() % 100);
			bl[i].tPos.z + float(rand() % 100);
			bl[i].CollisionR = 30;
			bl[i].CheckHit = false;

			//�e��G�{�̂̍��W��
			bl[i].pos = bl[i]._sPos;

			//�e�̎g�p���A�N�e�B�u��
			bl[i].active = true;

			break;
		}
	}
	return 0;
}

//�e����
void BulletClass::BulletProcess(int i)
{
	Move(i);//�ړ�
	Rotate(i);//��]
	Destroy(i);//����
}

//�e�`��
void BulletClass::BulletRender(int i)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//�A�N�e�B�u�Ȓe�����`��
		if (bl[i].active)
		{
			MV1SetPosition(handle, bl[i].pos);	//�e�̍��W
			MV1SetScale(handle, VGet(5,5,5));	//�e�̑傫��
			MV1SetRotationXYZ(handle,bl[i].rot);	//�e�̉�]
			MV1DrawModel(handle);	//�e�`��
		}
	}
}

void BulletClass::Move(int i)	//�e�ړ�
{
	//�����x�N�g������
	VECTOR dir = VGet(0.f, 0.f, 0.f);
	//�������擾
	VECTOR comp = VScale(VSub(bl[i]._sPos, bl[i].tPos), 20);
	//�x�N�g���̑傫�����擾
	float mag = PosMagnitude(bl[i]._sPos, bl[i].tPos);
	dir.x = comp.x / mag;
	dir.y = comp.y / mag;
	dir.z = comp.z / mag;
	//�ړ�
	bl[i].pos = VSub(bl[i].pos, dir);
}

void BulletClass::Destroy(int i)	//�e����
{
	//�e����苗�����ꂽ�����
	if(PosMagnitude(bl[i].tPos, bl[i].pos) >= 2000/*||MV1CollCheck_Sphere()*/)
	{
		bl[i].active = false;
	}
}

void BulletClass::Rotate(int i)	//���˕�����]
{
	VECTOR _vDir = VGet(0, 0, 0);
	_vDir = RotateToTarget(bl[i].tPos, bl[i]._sPos);	//�^�[�Q�b�g���W�֌����x�N�g�����쐬

	bl[i].rot.x = -atan2(_vDir.y, hypot(_vDir.x, _vDir.z));
	bl[i].rot.y = atan2(_vDir.x, _vDir.z);
}
