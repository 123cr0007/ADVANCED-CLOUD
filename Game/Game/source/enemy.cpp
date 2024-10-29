//�G�̂̏���
#include "Enemy.h"
#include "Fellow.h"

TimeClass* Etime;

Enemy* Enemy::ENInstance = nullptr;
Enemy::Enemy(int _id,VECTOR sPos,VECTOR gPos) : ObjectBase()
{
	ENInstance = this;

	id = _id;
	level = 0;
	_sPos = sPos;
	_gPos = gPos;
	_vPos = _sPos;

	mode = static_cast<int>(MODE::search);
	shot = false;
	r = ENEMY_SEARCH_AREA;
	
	if(level != -1){
		type = EN_TYPE_MOVE;
	}

	for(int i=0;i<ENEMY_COUNT_MAX;i++)
	{
		count[i] = 0;
	}
	
	En_handle = MV1LoadModel("res/Enemy/drone_hoba.mv1");
	Tex_color[0] = LoadGraph("res/Enemy/texture/drone_uv02_test_lambert1_BaseColor_blue.png");
	Tex_color[1] = LoadGraph("res/Enemy/texture/drone_uv02_test_lambert1_BaseColor_yellow.png");
	Tex_color[2] = LoadGraph("res/Enemy/texture/drone_uv02_test_lambert1_BaseColor_red.png");
	
	//�y�ʉ��̂��߂̂��́B
	En_handle = MV1DuplicateModel(En_handle);
	
	_anim = MV1LoadModel("res/Enemy/hoba.mv1");//�A�j���[�V�������f����ǂݍ���
	_total_time = 0.f;
	_play_time = 0.0f;
	_attach_index = MV1AttachAnim(En_handle, 0, _anim, TRUE);

	FlBlPos = VGet(0,0,0);
	flBlR = 0.0f;

	IsPatrol = false;
	IsFind	 = false;
	IsVoice	 = false;
	IsShot	 = false;
	IsSearchOut = false;

}

Enemy::~Enemy()
{
	
}

bool Enemy::Initialize()
{
	
	return true;
}

bool Enemy::Process(VECTOR target_pos)
{
	VECTOR plPos = Player::GetInstance()->GetPos();
	float plRad = Player::GetInstance()->GetRadius();
	VECTOR target = VGet(target_pos.x, target_pos.y + 100, target_pos.z);

		//�G�l�~�[�{�̏���
	if(type == EN_TYPE_MOVE) {

		//�G�l�~�[��2�_�Ԃ��^������֐�
		switch(mode)
		{
		case static_cast<int>(MODE::search):
			//�G�l�~�[�̃e�N�X�`����ύX
			MV1SetTextureGraphHandle(En_handle, 2, Tex_color[0], TRUE);
			//�G�l�~�[�̉�]
			Rotate();
			//�v���C���[�����G
			MakeArea();
			//�v���C���[����������U�����[�h��
			if(Search(target))
			{
				//�G�l�~�[�̃e�N�X�`����ύX
				MV1SetTextureGraphHandle(En_handle, 2, Tex_color[1], TRUE);
				//�v���C���[���������t���O�𗧂Ă�
				IsFind = true;
				//�U�����[�h��
				mode = static_cast<int>(MODE::attack);
			}
			//�v���C���[���͈͊O�ɏo����p�g���[�����[�h��
			if(Move())
			{
				//�p�g���[�����[�h��
				mode = static_cast<int>(MODE::idle);
			}
			break;
		//�U�����[�h
		case static_cast<int>(MODE::attack):
			//�v���C���[�����։�]
			Rotate(target);
			//�������A60f�҂�
			if(shot == false && Wait(0, 1))
			{
				//�G�̃e�N�X�`����ύX
				MV1SetTextureGraphHandle(En_handle, 2, Tex_color[2], TRUE);
				shot = true;
			}
			//�������񔭎ˉ\�ɂ���
			if(shot)
			{
				Reload();//�����ő҂����Ԃ𖞂��������m�F
				if(!shot) {
					Shot(target);//�v���C���[�֒e�𔭎�
				}
			}
			//�v���C���[���͈͊O�ɏo������G���[�h��
			if(SearchOut(target))
			{
				//�G�̃e�N�X�`����ύX
				MV1SetTextureGraphHandle(En_handle, 2, Tex_color[0], TRUE);
				shot = false;
				Rot.x = 0;
				IsSearchOut = true;
				//���G���[�h��
				mode = static_cast<int>(MODE::search);
			}
			break;

			//�S�[���֒������̂Ő���
		case static_cast<int>(MODE::idle):
			//���W����
			PosSwap();
			IsPatrol = true;
			//���񃂁[�h��
			mode = static_cast<int>(MODE::rotate);
			break;

			//���񃂁[�h
		case static_cast<int>(MODE::rotate):
			//�S�[���։�]
			if (RotToGoal())
			{
				//���G���[�h��
				mode = static_cast<int>(MODE::search);
			}
			break;
		}
		//�A�j���[�V�����֘A����
		_play_time += 1.f;
		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		_total_time = MV1GetAttachAnimTotalTime(En_handle, _attach_index);
		// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
		if(_play_time >= _total_time)
		{
			_play_time = 0.0f;
		}

		//�G�l�~�[����ł����e�̏��
		if(Fellow::GetFLInstance() != nullptr) {
			for(int n = 0; n < BULLET_MAX; n++) {

				//�����̒e�������ŊǗ�
				//�����̒e���A�N�e�B�u�Ȃ�
				if(Fellow::GetFLInstance()->GetCheckActive(n) != 0) {
					//�����̒e�̍��W�Ɣ��a���擾
					FlBlPos = Fellow::GetFLInstance()->GetBlPos(n);
					flBlR = Fellow::GetFLInstance()->GetBlR(n);

					//��e������
					if(CheckHitPointCapsule(VGet(_vPos.x, _vPos.y , _vPos.z), VGet(_vPos.x, _vPos.y + 50, _vPos.z),
						FlBlPos, 30, flBlR) == true) {
						//�G�l�~�[���S
						ef_type = EF_ENEMY_BOMB;
						type = EN_TYPE_DEAD;
						//���S����
						Dead();
					}
				}
			}
		}
	}

	//�G�l�~�[�e����
	for(int j = 0; j < BULLET_MAX; j++){
		//�e���A�N�e�B�u�Ȃ�
		CheckBlActive[j] = bullet.GetActive(j);
		if (CheckBlActive[j] != 0) {
			//�e�̏�������
			bullet.BulletProcess(j);
			//�e�̍��W�Ɣ��a���擾
			BLPos[j] = bullet.GetPos(j);
			CheckBlActive[j] = bullet.GetR(j);
		}
		//�v���C���[��e������
		if (CheckHitPointCapsule(plPos, VGet(plPos.x, plPos.y , plPos.z),
			bullet.GetPos(j), plRad, bullet.GetR(j)) == true) {
			//�e���������Ă���̂�
			if (!bullet.GetCheckHit(j)) 
			{
				//�v���C���[�̃G�l���M�[�����炷
				bullet.SetCheckHit(j, true);
				UseEnergy = 1;
			}
		}
		else{
			//�e���������Ă��Ȃ�
			bullet.SetCheckHit(j, false);
		}
	}
	return true;
}

bool Enemy::Render()
{
	if (type == EN_TYPE_MOVE)
	{
		MV1SetAttachAnimTime(En_handle, _attach_index, _play_time);		// �A�j���[�V���������f���ɃA�^�b�`
		MV1SetRotationXYZ(En_handle, Rot);//�G�l�~�[��]�w��
		MV1SetPosition(En_handle, _vPos);//�G�l�~�[�|�W�V�����w��
		MV1DrawModel(En_handle);//�G�l�~�[�`��
		
		//�f�o�b�O�p
		if (AMG_wp::CheckUseCol == true) 
		{
			DrawCone3D(_tPos, _aPos, r, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), FALSE); 
			DrawFormatString(0, 30, GetColor(0, 0, 255), "_aPos.x=%f,_aPos.y=%f,_aPos.z=%f", _aPos.x, _aPos.y, _aPos.z);
			DrawFormatString(0, 50, GetColor(0, 0, 255), "shot=%d", shot); 
		}
		
	}

	//�e�`��
	for (int i = 0; i < BULLET_MAX; i++) {
		bullet.BulletRender(i);
	}
	return true;
}

bool Enemy::Move()	//�G�l�~�[��2�_�Ԃ��ړ�����֐�
{
	if(PosMagnitude(_vPos, _gPos) <= 1)//�S�[�����W�߂��ɓ��B������
	{
		_vPos = _gPos;//�����ɃS�[�����W�ʒu�ւ����Ă���
		return true;
	}
	else
	{
		//�S�[�����W�֌������Ĉړ�
		_vPos = VSub(_vPos, RotateToTarget(_gPos, _vPos));
		return false;
	}
}
	
void Enemy::PosSwap()	//���W����
{
	VECTOR temp = _sPos;
	_sPos = _gPos;
	_gPos = temp;
}

void Enemy::Rotate()	//�S�[�����W�֌���
{
	_vDir = RotateToTarget(_gPos, _vPos);//�S�[�����W�֌����x�N�g�����쐬
	Rot.y = atan2(_vDir.x, _vDir.z);
}

void Enemy::Rotate(VECTOR target_pos)	//�v���C���[�֌���
{
	_vDir = RotateToTarget(target_pos, _vPos);//�v���C���[�֌����x�N�g�����쐬

	Rot.x = -atan2(_vDir.y, hypot(_vDir.x, _vDir.z));
	Rot.y = atan2(_vDir.x, _vDir.z);
}

bool Enemy::Search(VECTOR target_pos)	//�v���C���[�����G
{
	// ������x�N�g�����u�P�ʃx�N�g���v�i�X�J����1�̃x�N�g���j�ɂ���
	_tPos = VGet(_vPos.x, _vPos.y + 60, _vPos.z);
	VECTOR _vA=VGet(0,0,0);
	float angle = Rot.y;

	//�~���̒��_�ݒ�
	_aPos = VSub(_aPos, _vPos);
	_vA.x = _aPos.x * cos(angle) - _aPos.z * sin(angle);
	_vA.z = _aPos.x * sin(angle) - _aPos.z * cos(angle);

	_vA=VAdd(_vA,_tPos);
	_aPos = _vA;
	//�~���Ɠ_�̓����蔻��
	if(CheckHitPointCorn(_vPos, _aPos, r, target_pos))
	{
		return true;
	}
	return false;
}

bool Enemy::SearchOut(VECTOR target_pos)	//�v���C���[���͈͊O�֓��S
{
	//�v���C���[���͈͊O�ɏo�����ǂ���
	if(PosMagnitude(target_pos, _vPos) >= ENEMY_SEARCH_AREA * 2)
	{
		return true;
	}
	return false;
}

void Enemy::Shot(VECTOR target_pos)	//�v���C���[�֍U��
{
	//�v���C���[������ł��Ȃ����ǂ���
	bool IsPlDead = Player::GetPLInstance()->GetPlayerDeath();
	//�v���C���[������ł��Ȃ��Ȃ�
	if(!IsPlDead) 
	{
		//�e�𔭎�
		IsShot = true;
		bullet.AddBullet(VGet(_vPos.x, _vPos.y + 30, _vPos.z), target_pos, BULLET_TEX_ENEMY);
	}
	shot = false;
}

void Enemy::Reload()	//�e�̃����[�h
{
	if(Wait(1, ENEMY_SHOT_WAIT))
	{
		shot = true;
	}
	else
	{
		shot = false;
	}
}

bool Enemy::Wait(int i,int second)	//�҂�����
{
	int fps = 60;
	//�J�E���g�i�߂�
	count[i]++;
	//���Ԃ�����
	if(count[i] >= second * fps)
	{
		count[i] = 0;
		return true;
	}
	return false;
}

void Enemy::MakeArea()	//���G�͈͂̍쐬
{
	int normal = ENEMY_SHOT_RANGE;
	switch(level)
	{
	case 0:
		_aPos = VGet(_vPos.x, _vPos.y, _vPos.z + normal);
		break;
	case 1:
		_aPos = VGet(_vPos.x, _vPos.y, _vPos.z + normal*1.2);
		break;
	case 2:
		_aPos = VGet(_vPos.x, _vPos.y, _vPos.z + normal*1.5);
		break;
	case 3:
		_aPos = VGet(_vPos.x, _vPos.y, _vPos.z + normal);
		break;
	}
}

bool Enemy::RotToGoal()	//�S�[���֐���
{
	_vDir = RotateToTarget(_gPos, _vPos);
	float rot_y = atan2(_vDir.x, _vDir.z);
	Rot.y += DegToRad(1);
	RotateCnt++;
	//��]��180�x�ɂȂ�����
	if(RotateCnt==180)
	{
		Rotate();
		RotateCnt = 0;
		return true;
	}
	return false;
}

bool Enemy::Dead()	//���S
{
	if(type==EN_TYPE_DEAD)
	{
		//�����G�t�F�N�g�����S�n�_�ɐ���
		EffectManager::GetEFInstance()->Add(EF_ENEMY_BOMB,_vPos);
		return true;
	}
	else
	{
		return false;
	}
}