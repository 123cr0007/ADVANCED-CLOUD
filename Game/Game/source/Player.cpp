#include "DxLib.h"
#include "ApplicationMain.h"

#include "Player.h"
#include "enemy.h"
#include "Stage.h"
#include "Camera.h"
#include "Effekseer.h"
#include "EnergyGauge.h"


// �h���N���X�̃R���X�g���N�^�� : �ő����Ċ��N���X�̃R���X�g���N�^���Ăׂ�
Player* Player::PLInstance = nullptr;
Player::Player() : ObjectBase() {

	// �v���C���[�̃C���X�^���X��ݒ�
	PLInstance = this;

	//�d�͊֘A������
	gravity = 0;
	OldCheckGrv = false;
	CheckGravity = false;
	CheckJump = JUMP_TYPE_NONE;
	Srow = 0;
	FloatingTime = 0;

	//�J�����֘A������
	camPos = VGet(0, 0, 0);
	camTgtPos = VGet(0, 0, 0);

	//�ړ��l�֘A������
	MoveV = VGet(0, 0, 0);
	oldMoveV = VGet(0, 0, 0);

	//�W�����v�֘A������
	jumptype = -1;//0�Ԃ���g�������̂�-1�B
	jumpTime = 60;
	release_jump = 0;
	CheckAtack = 0;

	//�����x������
	cherOpacity = 1;

	//m�Ƀ��[�V�����֘A������
	animBrendRate = 0;
	animBrendIndex = -1;
	idleTime = 0;

	Cr = GetColor(0, 0, 0);
	
	//�����L�����̍����ɂ���ĕς���
	_colSubY = 50.f;

	//���[�v�֘A
	CheckAvoid = 0;
	AvoidTime = 0;
	PlSize = VGet(1, 1, 1);
	OldPos = VGet(0, 0, 0);

	//�e
	ShadowPos = VGet(0, 0, 0);
	CheckMakeShadow = false;

	//�W�����v�֘A	������
	JumpCast = 0;

	//�G�l���M�[�֘A������
	restEnergy = 10;
	plUseEnergy = 0;
	jumpTime = 0;
	chargeEnergy = 0;
	checkHealEnergy = 0;

	ChHight = 150;

	//�f�o�b�O�֘A������
	CP = VGet(0, 0, 0);
	CheckNum = 0;
	grvNum = 0;
	muveNum = 0;

	ModePhoto = false;

	//����ł��邩
	if (AMG_wp::ScenarioTrg[AMG_wp::StageNum] == true) {
		isOperate = false;
	}
	else {
		isOperate = true;
	}

	//�X�e�[�W�i�s�֘A
	changeMap = false;
	stageTrg = false;
	gameOver = false;
	dead = false;
	gameClear = false;

	//���֘A������
	isJump = false;
	isLand = false;
	isFloat = false;
	isJust = false;
	isWalk = false;
	isStealth = true;
	isHitWall = false;

	AMG_wp::CheckUseCol = false;
}

Player::~Player() {
    ObjectBase::~ObjectBase();
	//�n���h���̍폜
	DeleteEffekseerEffect(ef_handle);
	MV1DeleteModel(handle);
}

bool Player::Initialize() {

	// ���(ObjectBase)�N���X �� Load �֐��������ă��[�h����
	//handle = Load(_T("res/Player/PLSumple.mv1"), 1);
	handle = Load(_T("res/Player/chara_motion_data_0228.mv1"), 1);
	
	// ���f���̈ʒu��ݒ�
	if (ModeServer::GetInstance()->Get("Game") != NULL) {

		if (AMG_wp::StageNum < STAGE_MAX) {

			_vPos = AMG_wp::StartPos[AMG_wp::StageNum];
			//�f�t�H��-�������Ă�
			_vDir = VGet(1, 0, 0);
		}
	}
	else {
	
		_vPos = VGet(100, -120, 150);
		//�J�����̕� ���F���E�@�^��:�����@�E-���s 150, -80, 150
		_vDir = VSub(VGet(0,0,300),_vPos);
	}

	_collision_r = 20.f;

	//�J�v�Z���̈ʒu������
	CapsuleTop = VGet(_vPos.x, _vPos.y + CUPSLE_UPOS, _vPos.z);
	CapsuleRow = VGet(_vPos.x, _vPos.y + CUPSLE_DPOS, _vPos.z);

	return true;
}

bool Player::Process() {

	//�f�o�b�O�p
	if (AMG_wp::CheckUseCol == true) {
		grvNum = JUMP_POWER * 3;
		muveNum = PL_MOVE_SPEED * 3;
	}
	else {
		grvNum = JUMP_POWER;
		muveNum = PL_MOVE_SPEED;
	}

	VECTOR oldPlPos = _vPos;

	//�������F�X
	OldCheckGrv = CheckGravity;

	//�v���Z�X�F�X
	if (isOperate == true) {

		//���암��
		ProcessOperate();
	}
	
	//�A�j���[�V����
	Animetion();

	//�ړ�����̏���
	//�d�́��ǂƂ̓����蔻��i�J�v�Z���j���n�ʂƂ̓����蔻��i���C�j
	ProcessGrv();
	ProcessCapsule();
	ProcessRand();
	ProcessMove();
	
	//�e
	MakeShadow();
	

	//�G�̒e�Ƃ̓����蔻��
	if (Enemy::GetENInstance() != nullptr) {
		for (int n = 0; n < BULLET_MAX; n++) {
			int a = Enemy::GetENInstance()->GetCheckActive(n);
			if (Enemy::GetENInstance()->GetCheckActive(n) != 0) {
				
				VECTOR FlBlPos = Enemy::GetENInstance()->GetBlPos(n);
				float flBlR = Enemy::GetENInstance()->GetBlR(n);

				if (CheckHitPointCapsule(CapsuleTop, CapsuleRow, FlBlPos, 30, flBlR) == true) {

					//�G�l���M�[-1�i1�_���[�W�j
					plUseEnergy = 1;

					//�_���[�W�󂯂��g���K�I��
					isDamage = true;

					//�c�����G�l���M�[��0�������ꍇ���S����
					if (restEnergy == 0) {
						dead = true;
						isOperate  =   false;
						int ran = rand() % 2;
						if (ran == 0)
						{
							gGlobal._sndServer.Get("VOICE_ACT_PL_15.wav")->Play();
						}
						if (ran == 1)
						{
							gGlobal._sndServer.Get("VOICE_ACT_PL_16.wav")->Play();
						}

						break;
					}
				}
			}
		}
	}

	//�G�l���M�[�񕜂��邩
	if (!CheckGravity) {

		checkHealEnergy = 1;
	}
	else {

		checkHealEnergy = 0;
	}

	//�W�����v������
	CheckJump = JUMP_TYPE_NONE;

	//�J�v�Z���̈ʒu�X�V
	CapsuleTop = VGet(_vPos.x, _vPos.y + CUPSLE_UPOS, _vPos.z);
	CapsuleRow = VGet(_vPos.x, _vPos.y + CUPSLE_DPOS, _vPos.z);

	return true;
}

bool Player::Render(int type) { 

	//3D�`��
	type = 1;

	// �ʒu
	MV1SetPosition(handle, _vPos);
	
	// ��������Y����]���Z�o
	// ���f�����W���łǂ���������Ă��邩�Ŏ����ς��(�����-z�������Ă���ꍇ)
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);		
	MV1SetRotationXYZ(handle, vRot);

	//�����x
	MV1SetOpacityRate(handle, cherOpacity);


	Rend(type);

	//�e
	if (CheckMakeShadow != false) {
		for (int i = CIRCLE_CORNAR - 2; i > 0; i--) {
			DrawTriangle3D(ShadowPos, ShadowCircle[i + 1], ShadowCircle[i], GetColor(80, 80, 80), TRUE);
		}
	}

	//�f�o�b�O�p�\���֘A
	if (AMG_wp::CheckUseCol == true) {
		DrawCapsule3D(CapsuleRow, CapsuleTop, _collision_r, 16,
			GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);


		DrawFormatString(0, 0, GetColor(0, 0, 0), "posX = %f posY = %f posZ = %f", _vPos.x, _vPos.y, _vPos.z);
		DrawFormatString(0, 20, GetColor(0, 0, 0), "RestEnergy = %d", restEnergy);
		DrawFormatString(0, 40, GetColor(0, 0, 0), "JustTime=%d", jumpTime);
		DrawFormatString(0, 60, GetColor(0, 0, 0), "STATUS = %d", _status);
		DrawFormatString(0, 80, GetColor(255, 0, 0), "CheckMakeShadow = %d", CheckMakeShadow);
		DrawFormatString(0, 100, GetColor(255, 0, 0), "CheckGravity = %d", OldCheckGrv);
		DrawFormatString(0, 120, GetColor(255, 0, 0), "StageNum = %d", AMG_wp::StageNum);
		
		DrawLine3D(VGet(_vPos.x, _vPos.y, _vPos.z + 0), CP, Cr);
		//DrawLine3D(VAdd(_vPos, VGet(0, _colSubY, 0)), VAdd(_vPos, VGet(0, -99999999.0f, 0)), GetColor(255, 0, 0));
		
	}

	return true;
}

//�^�C�g���p�v���Z�X
bool Player::ProcessForTitle() {

	Animetion();
	return true;
}


//�^�C�g���p�`��
bool Player::RenderForTitle() {

	// �ʒu
	MV1SetPosition(handle, _vPos);

	// ��������Y����]���Z�o
	// ���f�����W���łǂ���������Ă��邩�Ŏ����ς��(�����-z�������Ă���ꍇ)
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);
	MV1SetRotationXYZ(handle, vRot);

	MV1DrawModel(handle);
	return true;
}

//�e�̏���
bool Player::MakeShadow() {
	
	//�������F�X
	MV1_COLL_RESULT_POLY MadeShadow;
	VECTOR Len = VGet(-1, -1, -1);
	VECTOR OldLen = VGet(0, 0, 0);
	int HitNum = 0;

	//�X�e�[�W�̃C�e���[�^�[����
	for (auto ite : Stage::GetSTInstance()->GetBlockPositions()) {

		//�v���C���[�̍����瑫���Ɍ����ă��C���o��
		MadeShadow = MV1CollCheck_Line((ite).handle, (ite).Collision,
			VAdd(_vPos, VGet(0, _colSubY, 0)),
			VAdd(_vPos, VGet(0, -99999999.0f, 0)));
		//����������
		if (MadeShadow.HitFlag == 1) {

			//�e�o���g���K�I��
			CheckMakeShadow = true;

			//����������+1
			HitNum++;

			//��������������ۑ����Ă���
			OldLen = Len;

			//�����X�V
			Len = VSub(MadeShadow.HitPosition, _vPos);
			
			//��ԋ������߂��n���h���ɉe�𗎂Ƃ�
			if (HitNum == 1 || VSize(OldLen) > VSize(Len)) {

				//�}�b�v���f���Ɣ��Ȃ��悤�ɉe�̈ʒu������������
				ShadowPos = VGet(MadeShadow.HitPosition.x, MadeShadow.HitPosition.y + 1.0f,
					MadeShadow.HitPosition.z);

				//�ӂ̐��ɉ����Ċp�̈ʒu���w�肷��
				for (int i = 0; i < CIRCLE_CORNAR; i++) {

					ShadowCircle[i] = VGet(ShadowPos.x + cos(i * 0.105) * _collision_r, ShadowPos.y,
						ShadowPos.z + sin(i * 0.105) * _collision_r);
				}
			}
		}
	}

	if (HitNum == 0) {

		CheckMakeShadow = false;
	}
	return true;
}


//���[�v�W�����v�̏����鏈��
bool Player::Vanish(VECTOR GoalPos, VECTOR StartPos) {

	float size = 0.5f; //1�t�����Ƃɏ��������鐔
	AvoidTime++;

	//�T�C�Y�����ȏ�Ȃ珬��������
	if (VSize(MV1GetScale(handle)) > 0.000001f) {

		PlSize = VScale(PlSize, size);
		MV1SetScale(handle, PlSize);
	}

	//�ڕW�n�_�܂ł̋���
	VECTOR len = VSub(GoalPos, StartPos);

	//�ړ���
	float MoveNum = VSize(len) / FRAME_TO_MOVE;

	//������P�ʉ��������
	VECTOR Nlen = VNorm(len);

	//�w�肳�ꂽ���Ԗ����Ȃ�ڈړ����邷��
	if (AvoidTime < FRAME_TO_MOVE) {
		_vPos = VAdd(_vPos, VScale(Nlen, MoveNum));
	}
	else {
		
		CheckAvoid = 2;
		AvoidTime = 0;
	}

	return true;
}

//�o�����鏈��
bool Player::arrival() {

	//1�t�����Ƃɑ傫�����鐔
	float size = 1.8f;

	//���{�����Ȃ�g�傷��
	if (VSize(MV1GetScale(handle)) < 1) {

		PlSize = VScale(PlSize, size);
		MV1SetScale(handle, PlSize);
	}
	else {
		MV1SetScale(handle, VGet(1, 1, 1));
		EffectManager::GetEFInstance()->Add(EF_GATHER, _vPos);
		CheckAvoid = 0;
		gravity = JUMP_POWER / 2;
	}

	return true;
}

//�v���C���[�̑��암��
bool Player::ProcessOperate() {

	VECTOR v = { 0,0,0 };	// �ړ���

	//�p�b�h���͂��擾   
	GamePad::MULTIPAD pad  = GamePad::GetInstance()->GetMultiPad(PLAYER_1);

	//�J�����̈ʒu������
	camPos = Camera::GetCamInstance()->GetCamPos();
	camTgtPos = Camera::GetCamInstance()->GetTgtPos();


	if(pad.LX != 0 || pad.LY != 0)//���͂�������
	{
		VECTOR lstick = { 0,0,0 };	// ���E�A�i���O�X�e�B�b�N�̍��W
		float analogMin = 0.3f;	// �A�i���O臒l

		lstick.x = pad.LX;
		lstick.y = pad.LY;

		//���͂�ϊ�
		lstick = VNorm(lstick);

		// �J�����̌����Ă���p�x���擾
		float sx = camPos.x - camTgtPos.x;
		float sz = camPos.z - camTgtPos.z;
		float camrad = atan2(sz, sx);

		// �L�����ړ�(�J�����ݒ�ɍ��킹��)
		float mvSpeed = muveNum;

		// �A�i���O���X�e�B�b�N�p
		float length = sqrt(lstick.x * lstick.x + lstick.y * lstick.y);
		float rad = atan2(lstick.x, lstick.y);
		if(length < analogMin) {
			// ���͂������������瓮���Ȃ��������Ƃɂ���
			length = 0.f;
		}
		else {
			length = mvSpeed;
		}

		// v��rad����]&�ړ�������
		v.x = cos(rad + camrad) * length;
		v.z = sin(rad + camrad) * length;

		//����W�����v�͈ړ�������Ȃ��߂���ȊO�͈ړ��l�ۑ�
		if(CheckAvoid == 0) {

			OldPos = _vPos;
		}
	}

	//�f�o�b�O���[�h���S�[����O�Ƀ��[�v�ł���悤��
	if (AMG_wp::CheckUseCol) {
		if (pad.trg & PAD_INPUT_5) {
			_vPos = VGet(-8566,45451,8564);
		}
	}
	
	//�G�l���M�[���c���ĂăW�����v��CT�������Ă���Ȃ�
	if(JumpCast == 0) {
		if(restEnergy > 0) {

			//�ʏ�W�����v
			if(pad.trg & PAD_INPUT_1) {
				plUseEnergy = 2;
				if (EnergyGauge::GetEPInstance()->GetHaveNowEnergy() >= plUseEnergy)
				{
					CheckJump = JUMP_TYPE_NORMAL;
					CheckGravity = true;
					jumptype = 2;
					Srow = 0;
					gravity = grvNum;
					JumpCast = JUMP_CT;
					justTimeMax = 20;
					EffectManager::GetEFInstance()->Add(EF_JUMP_NORMAL, _vPos);

					if (jumpTime < justTimeMax && jumpTime > 0) {
						isJust = true;
						if (restEnergy < 10) {
							chargeEnergy = 1;
						}
					}
					jumpTime = 60;
				}
				else
				{
					plUseEnergy = 0;
				}
			}

			//������藎����W�����v
			if(pad.trg & PAD_INPUT_2 && AMG_wp::StageNum >= 2) {
				plUseEnergy = 4;
				if (EnergyGauge::GetEPInstance()->GetHaveNowEnergy() >= plUseEnergy)
				{
					CheckJump = JUMP_TYPE_FLOAT;
					CheckGravity = true;
					jumptype = 1;
					Srow = 1;
					gravity = grvNum + GRVADD_NUM;
					JumpCast = JUMP_CT;
					justTimeMax = 20;
					EffectManager::GetEFInstance()->Add(EF_JUMP_FLOAT, _vPos);

					if (jumpTime < justTimeMax && jumpTime > 0) {
						isJust = true;
						if (restEnergy < 10) {
							chargeEnergy = 1;
						}
					}
					jumpTime = 60;
				}
				else
				{
					plUseEnergy = 0;
				}
			}

			//����W�����v
			if(pad.trg & PAD_INPUT_4 && AMG_wp::StageNum >= 3) {
				plUseEnergy = 8;
				if (EnergyGauge::GetEPInstance()->GetHaveNowEnergy() >= plUseEnergy)
				{
					CheckJump = JUMP_TYPE_AVOID;
					CheckGravity = true;
					jumptype = 0;
					Srow = 0;
					CheckAvoid = 1;

					gravity = grvNum;
					JumpCast = JUMP_CT;
					justTimeMax = 20;
					EffectManager::GetEFInstance()->Add(EF_JUMP_AVOID, _vPos);
					EffectManager::GetEFInstance()->Add(EF_DIFFUSE, _vPos);
					EffectManager::GetEFInstance()->Add(EF_TORNADO_MOVE, _vPos);

					if (jumpTime < justTimeMax && jumpTime > 0) {
						isJust = true;
						if (restEnergy < 10) {
							chargeEnergy = 1;
						}
					}
					jumpTime = 60;
				}
				else
				{
					plUseEnergy = 0;
				}
			}

			//�U���W�����v
			if(pad.trg & PAD_INPUT_3 && AMG_wp::StageNum >= 3) {
				plUseEnergy = 4;
				if (EnergyGauge::GetEPInstance()->GetHaveNowEnergy() >= plUseEnergy)
				{
					CheckJump = JUMP_TYPE_ATK;
					CheckAtack = 1;
					CheckGravity = true;
					jumptype = 3;
					Srow = 0;
					gravity = grvNum;
					JumpCast = JUMP_CT;
					justTimeMax = 20;
					EffectManager::GetEFInstance()->Add(EF_JUMP_ATTACK, _vPos);

					if (jumpTime < justTimeMax && jumpTime > 0) {
						isJust = true;
						if (restEnergy < 10) {
							chargeEnergy = 1;
						}
					}
					jumpTime = 60;
				}
				else
				{
					plUseEnergy = 0;
				}
			}
		}
	}
	if(JumpCast>0)
	{
		JumpCast--;
	}
	if (jumpTime > 0) {
		jumpTime--;
	}

	MoveV = v;
	return true;
}


bool Player::ProcessGrv() {

	VECTOR v = MoveV;

	//�d�͏���
	{
		//�����O�̏d�͒l�ۑ�
		float OldG = gravity;
		//�Ƃ肠�����d�͉�����
		//�����̒l�ς���Έړ��l�ς��
		gravity -= CHANGE_GRV_NUM;

		//�d�͂������󋵂Ȃ�d�͏���
		if (CheckGravity) {

			//�����Œl���
			v.y += gravity;
			//���͂��������W�����v�Ȃ�d�͂̉����l������
			if (Srow == 1) {
				if (gravity <= -5) {

					gravity = OldG;
				}
			}
		}
		else {
			//�d�͏������Ȃ��Ȃ�d�͒l���O�ɂ��Ă���
			gravity = 0;
			//�W�����v���Ă��Ȃ��̂�-1�ɂ��Ă���
			jumptype = -1;
		}
	}

	MoveV = v;
	return true;
}


bool Player::ProcessCapsule() {

	VECTOR v = MoveV;
	//�����蔻��Ɏg���J�v�Z���̏��
	MV1_COLL_RESULT_POLY_DIM HitPolyCapsule;
	VECTOR CapsulePos1 = CapsuleRow;
	CapsulePos1 = VAdd(CapsulePos1, v);

	VECTOR CapsulePos2 = CapsuleTop;
	CapsulePos2 = VAdd(CapsulePos2, v);

	//�}�b�v�̃C�e���[�^�[
	for (auto& ite : Stage::GetSTInstance()->GetBlockPositions()) {

		// ���f���ƃ}�b�v�Ƃ̓����蔻��
		HitPolyCapsule = MV1CollCheck_Capsule((ite).handle, (ite).Collision,
			CapsulePos1, CapsulePos2, _collision_r);

		if (HitPolyCapsule.HitNum >= 1) {

			const int HitNum = HitPolyCapsule.HitNum;
			int i = 0;

			// ���������|���S���̐������J��Ԃ�
			for (i = 0; i < HitPolyCapsule.HitNum; i++)
			{

				//�����ƎO�p�`�̍ŋߓ_����炤
				tagSEGMENT_TRIANGLE_RESULT resurt;
				Segment_Triangle_Analyse(&CapsulePos1, &CapsulePos2, &HitPolyCapsule.Dim[i].Position[0],
					&HitPolyCapsule.Dim[i].Position[1], &HitPolyCapsule.Dim[i].Position[2], &resurt);

				//�x�N�g���̈����Z�ŉ����o���p�x�����
				VECTOR PushDir = VSub(resurt.Seg_MinDist_Pos, resurt.Tri_MinDist_Pos);

				//�����ƎO�p�`�̍ŋߓ_��
				float NearestPoints = Segment_Triangle_MinLength(CapsulePos1, CapsulePos2,
					HitPolyCapsule.Dim[i].Position[0], HitPolyCapsule.Dim[i].Position[1], HitPolyCapsule.Dim[i].Position[2]);
				//�v���C���[�̉�����������
				float SubLen = _collision_r - NearestPoints;

				//�@�������ɏ�ŋ��߂��������|����
				VECTOR PushVec = VScale(VNorm(PushDir), SubLen + 1.0f);

				//�ړ�
				_vPos = VAdd(_vPos, PushVec);
				//�J�v�Z���̈ʒu�X�V
				CapsulePos1 = VAdd(CapsulePos1, PushVec);
				CapsulePos2 = VAdd(CapsulePos2, PushVec);
			}
		}

		else {

		}

		MV1CollResultPolyDimTerminate(HitPolyCapsule);
	}


	MoveV = v;
	return true;
}

bool Player::ProcessRand() {

	VECTOR v = MoveV;

	//�ړ��㏈��
	//�W�����v�p���C
	MV1_COLL_RESULT_POLY hitPolyG;
	int CheckHit = 0;
	int Num = 0;
	float HitPosY = 0.0f;
	float OldHitPosY = 0.0f;

	for (auto& ite : Stage::GetSTInstance()->GetBlockPositions()) {

		//�󒆂ɂ���Ƃ��i�d�͂��g���Ă�Ƃ��j�ƒn��ɂ���Ƃ��ŏ���������
		if (!CheckGravity) {
			//�n��ɂ���Ƃ�
			//�W�����v�Ɉ���������Ȃ��悤�ɃW�����v�͂�菭���Z�����C�o��
			hitPolyG = MV1CollCheck_Line((ite).handle, (ite).Collision,
				VAdd(_vPos, VGet(0, _colSubY, 0)),
				VAdd(_vPos, VGet(0, -JUMP_POWER + 1.0f, 0)));

			CP = VAdd(_vPos, VGet(0, -1000.0f, 0));
			Cr = GetColor(255, 0, 0);
		}
		else {
			//�󒆂ɂ���Ƃ�
			//�n�ʂ̊ђʖh�~�ŏd�͂̒l�����̂܂ܓ����
			hitPolyG = MV1CollCheck_Line((ite).handle, (ite).Collision,
				VAdd(_vPos, VGet(0, _colSubY, 0)),
				VAdd(_vPos, VGet(0, gravity, 0)));

			CP = VAdd(_vPos, VGet(0, gravity, 0));
			Cr = GetColor(0, 0, 255);
		}

		//�����������̏����͋���
		if (hitPolyG.HitFlag == 1) {

			OldHitPosY = hitPolyG.HitPosition.y;
			CheckHit++;
			float len = _vPos.y - OldHitPosY;

			if (OldHitPosY > len || HitPosY == 0.0f) {

				HitPosY = hitPolyG.HitPosition.y;
				mapName = (ite).name;
				String = mapName.c_str();
				stageTrg = (ite).AdvanceStage;

				//�������񂾎�
				if ((ite).stepClassroom == true) {
					//�X�e�[�W���̃g���K�I�t
					Stage::GetSTInstance()->SetStepClassroom(Num, false);
					Stage::GetSTInstance()->SetAddStepClassroomNum(1);
					if (Stage::GetSTInstance()->GetStepClassroomNum() > 1) {
						isOperate = false;
						AMG_wp::StageNum = 4;
						//MoveV = VGet(0, 0, 0);
					}
				}
				check = AMG_wp::StageNum;
			}
		}
		
		//�N���X�^���Ƃ̋��������ȉ��Ȃ�X�e�[�W�i�߂�
		if ((ite).modelNo == 21 && (ite).AdvanceStage == true) {
			
			VECTOR len = VSub(_vPos,VGet(ite.x, ite.y, ite.z));
			if (VSize(StLen) == 0 || VSize(StLen) > VSize(len)) {

				//StLen = len;
				StPos = VGet(ite.x, ite.y, ite.z);
			}
			//���������ȉ��Ȃ�X�e�[�W�i�s
			if (VSize(VSub(_vPos, StPos)) < 1000) {
				//�X�e�[�W���̃g���K�I�t
				Stage::GetSTInstance()->SetAdvanceStage(Num, false);
				StLen = VGet(0, 0, 0);

				//�X�e�[�W���Ō�ȊO�Ȃ�`���[�g���A��
				if (AMG_wp::StageNum < STAGE_MAX - 1) {
					changeMap = true;
				}

				//�X�e�[�W��i�߂�
				if (AMG_wp::StageNum < STAGE_MAX - 1) {

					AMG_wp::ScenarioTrg[AMG_wp::StageNum] = false;
					AMG_wp::StageNum += 1;
				}
			}
		}
		Num++;
	}

	StLen = VSub(_vPos, StPos);

	if (CheckHit > 0) {

		// ��������
		// ��������Y�ʒu���L�������W�ɂ���
		_vPos.y = HitPosY;

		gravity = 0;
		CheckJump = JUMP_TYPE_NONE;
		CheckAtack = 0;
		CheckGravity = false;
		Srow = 0;
		FloatingTime = 0;
	}
	else {

		FloatingTime += 1;
	}

	if (FloatingTime >= 10) {

		CheckGravity = true;
		//FloatingTime = 0;
	}

	MoveV = v;
	return true;
}

//�ړ�����
bool Player::ProcessMove() {

	VECTOR v = MoveV;

	//���p�ړ�����
	switch (CheckAvoid)
	{
	case 1:
		Vanish(VGet(VAdd(OldPos, VScale(v, MOVIVG_FRONT)).x, OldPos.y + MOVIVG_HEIGHT, VAdd(OldPos, VScale(v, MOVIVG_FRONT)).z), OldPos);
		break;
	case 2:
		arrival();
		break;
	}

	//�ʏ�̈ړ�
	if (CheckAvoid == 0) {
		_vPos = VAdd(_vPos, v);
	}

	//�v���C���[�̌������w��
	if (v.x != 0 || v.z != 0) {

		_vDir = v;
	}

	//����\�Ȃ�ړ�
	if (!isOperate) {
		MoveV = v;
	}

	return true;
}

//�A�j���[�V����
bool Player::Animetion() {

	VECTOR v = MoveV;
	VECTOR Move = VGet(1, 1, 1);
	
	oldStatus = _status;

	//�����Ă��邩
	if (_status != STATUS::DOWN) {
		//�����t���[�����ŃW�����v���͂��Ȃ���
		if (CheckJump == JUMP_TYPE_NONE) {
			//�W�����v���Ă���ꍇ���؃��[�V�������I����Ă��邩
			if (_status != STATUS::JUMP_START_R) {
				if (_status != STATUS::JUMP_START_L) {

					//�ړ�����
					if (VSize(v) > 0.f) {

						//�~�܂��Ă����瑖��n�߂̃��[�V����
						if (oldStatus == STATUS::WAIT)
						{
							_status = STATUS::RUN_START;
						}
						else {

							_status = STATUS::RUN;
						}
					}
					else {

						//�ړ����ĂȂ��ꍇ�e�I���̃��[�V�����Ɍq����
						switch (oldStatus) {
						case STATUS::WAIT:
							_status = STATUS::WAIT;
							break;

						case STATUS::RUN:
							_status = STATUS::RUN_END;
							break;

						case STATUS::FALL_R:
							_status = STATUS::JUMP_END_R;
							break;

						case STATUS::FALL_L:
							_status = STATUS::JUMP_END_L;
							break;
						}
					}

					//�󒆂ɂ���Ԃ͗������[�V����
					if (CheckGravity) {
						if (oldStatus != STATUS::FALL_L) {
							_status = STATUS::FALL_R;
						}
						else {
							_status = STATUS::FALL_L;
						}
					}
				}
			}

		}
		//�A���ŃW�����v����ꍇ�͍��E���݂ɑ��𓥂ݏo��������
		else {
			if (_status == STATUS::FALL_L) {
				_status = STATUS::JUMP_START_R;
			}
			else {
				_status = STATUS::JUMP_START_L;
			}
		}

		//���n�C���p
		if (OldCheckGrv == true && CheckGravity == false) {
			if (_status == STATUS::FALL_R) {
				_status = STATUS::JUMP_END_R;
			}
			else {
				_status = STATUS::JUMP_END_L;
			}
		}
	}

	//����ł���Ȃ玀�S���[�V����
	if (dead == true) {
		_status = STATUS::DOWN;
	}

	//�ŏ��̓A�C�h�����[�V��������
	if (_status == STATUS::NONE) {
		_status = STATUS::WAIT;
	}


	// �Đ����Ԃ��A�j���[�V�����̑��Đ����ԂɒB������Đ����Ԃ��O�ɖ߂�
	if (playTime >= totalTime) {
		
		//�X�e�[�W�i�s�g���K�I��
		if (isOperate == false) {
			if (mapName == "Newkyo") {
				AMG_wp::StageNum += 1;
				gameClear = true;
			}
			else {
				changeMap = true;
			}
		}
		if (_status == STATUS::DOWN) {
			gameOver = true;
		}

		switch (_status) {
			//�e�X�^�[�g���炻�̃��[�V������
		case STATUS::JUMP_START_R:
			_status = STATUS::FALL_R;
			break;

		case STATUS::JUMP_START_L:
			_status = STATUS::FALL_L;
			break;

		case STATUS::RUN_START:

			_status = STATUS::RUN;
			break;

			//�I���Ȃ�A�C�h�����[�V������
		case STATUS::RUN_END:
		case STATUS::JUMP_END_R:
		case STATUS::JUMP_END_L:
		case STATUS::IDLE_2:
			_status = STATUS::WAIT;
			break;

			//���[�v���[�V�����Ȃ玞�Ԃ��ŏ���
		case STATUS::FALL_R:
		case STATUS::FALL_L:
		case STATUS::WAIT:
		case STATUS::RUN:

			playTime = 0.0f;
			break;
		}
	}

	//�A�C�h�����[�V�����̂܂܈�莞�ԓ��͂��Ȃ���Αҋ@�A�j���[�V��������
	if (_status == STATUS::WAIT) {
		if (idleTime > IDLE_DANCE_TIME) {
			idleTime = 0;
			_status = STATUS::IDLE_2;
			gGlobal._sndServer.Get("VOICE_ACT_PL_17.wav")->Play();
		}
		else {
			idleTime++;
		}
	}
	else {
		idleTime = 0;
	}

	// �X�e�[�^�X���ς���Ă��Ȃ����H
	if (oldStatus != _status) {

		//�u�����h�p�X�e�[�^�X�ɑO�̃X�e�[�^�X�ۑ�
		//stateBrend = oldStatus;
		animBrendIndex = MV1GetAttachAnim(handle, attachIndex);
		animBrendPlayTime = playTime;
		animBrendTotalTime = totalTime;
		animBrendRate = 1;


		if (attachIndex != -1) {
			MV1DetachAnim(handle, attachIndex);
			attachIndex = -1;
		}
		// �X�e�[�^�X�ɍ��킹�ăA�j���[�V�����̃A�^�b�`
		switch (_status) {

		case STATUS::WAIT:
			//_isidle = true;
			//_iswalk = false;
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle,"PC_IDLE"), -1, FALSE);
			break;

		case STATUS::RUN_START:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_RUN_START"), -1, FALSE);
			//_iswalk = true;
			break;

		case STATUS::RUN:
			//_iswalk = true;
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_RUN_ROOP"), -1, FALSE);
			break;

		case STATUS::RUN_END:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_RUN_END"), -1, FALSE);
			break;

		case STATUS::JUMP_START_R:
			isJump = true;
			//_iswalk = false;
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_SKIP_JUMP_RIGHT_START"), -1, FALSE);
			break;

		case STATUS::FALL_R:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_SKIP_JUMP_RIGHT_LOOP"), -1, FALSE);
			break;

		case STATUS::JUMP_END_R:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_SKIP_JUMP_RIGHT_END"), -1, FALSE);
			isLand = true;
			break;

		case STATUS::JUMP_START_L:
			isJump = true;
			//_iswalk = false;
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_SKIP_JUMP_LEFT_START"), -1, FALSE);
			break;

		case STATUS::FALL_L:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_SKIP_JUMP_LEFT_LOOP"), -1, FALSE);
			break;

		case STATUS::JUMP_END_L:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_SKIP_JUMP_LEFT_END"), -1, FALSE);
			isLand = true;
			break;

		case STATUS::DOWN:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_DOWN"), -1, FALSE);
			isLand = true;
			break;

		case STATUS::IDLE_2:
			attachIndex = MV1AttachAnim(handle, MV1GetAnimIndex(handle, "PC_DANCE"), -1, FALSE);
			isLand = true;
			break;
		}

		// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
		totalTime = MV1GetAttachAnimTotalTime(handle, attachIndex);

		// �Đ����Ԃ�������
		playTime = 0.0f;
	}
	else {

		// �Đ����Ԃ�i�߂�
		//�W�����v�̏��߂̏ꍇ�̂�CT�ƃ��[�V�����̒��������ݍ����悤�ɒ���
		if (_status == STATUS::JUMP_START_R || _status == STATUS::JUMP_START_L) {

			//�҂�����ɂ���ƂȂ񂩕ςȂ̂ł�����Ƒ���
			playTime += totalTime/ JUMP_CT + 0.5f;
			animBrendPlayTime += totalTime / JUMP_CT + 0.5f;
		}
		//����ȊO�Ȃ畁�ʂɗ���
		else {
			playTime += 1.f;
			animBrendPlayTime += 1.f;
		}
	}

	if (animBrendPlayTime >= animBrendTotalTime) {
		animBrendPlayTime = 0;
	}

	//����I���̏���������
	//�s�^�b�Ǝ~�܂�ƌ����ڂƍ���Ȃ����ߎ���
	if (_status == STATUS::RUN_END) {

		//�ړ��͂����Z����l
		auto animMoveNum = PL_MOVE_SPEED / totalTime;

		//���Z�l�����̃��[�V�������ԕ��傫������
		if (playTime * 3 < totalTime) {
			VECTOR Move = VScale(VNorm(VGet(_vDir.x, 0, _vDir.z)),
				animMoveNum * (totalTime - playTime * 3));
			/*_vPos = VAdd(_vPos,VSub(Move, v));*/

			v = VAdd(v, Move);
		}
	}


	//�A�j���[�V�������A�^�b�`����
	MV1SetAttachAnimTime(handle, attachIndex, playTime);
	
	MoveV = v;
	return true;
}