#include "DxLib.h"
#include "Fellow.h"
#include "enemy.h"
#include "EnemyManager.h"
#include "ApplicationMain.h"


// �h���N���X�̃R���X�g���N�^�� : �ő����Ċ��N���X�̃R���X�g���N�^���Ăׂ�
Fellow* Fellow::FLInstance = nullptr;
Fellow::Fellow() : ObjectBase() {

	FLInstance = this;

	//_en = new Enemy(this);

	_anim1 = 0;
	_anim2 = 0;
	_attach_index = 0;
	_total_time = 0;
	_play_time = 0;

	CenterPos = VGet(0, 0, 0);
	CheckV = VGet(0, 0, 0);
	NormalLengh = VGet(0, 0, 0);
	NormalRad = 0;

	time = 0;
	CheckMove = 0;
	MoveV = VGet(0, 0, 0);

	FireRate = 0;
	FiredNum = 0;
	Epos = VGet(0, 0, 0);
	
	RockonHandle = -1;

	useHandle = true;

	for (int i = 0; i < BULLET_MAX; i++) {
		BLPos[i] = VGet(0, 0, 0);
		BlRadius[i] = 0.0f;
		CheckBlActive[i] = false;
	}
}


Fellow::~Fellow() {
	ObjectBase::~ObjectBase();

}

bool Fellow::Initialize(VECTOR PlPos, VECTOR PlDir) {

	

	// ���(ObjectBase)�N���X �� Load �֐��������ă��[�h����
	handle = Load(_T("res/OtomoRobo/OtomoRobo.mv1"), 1);

	//�v���C���[�̈ʒu�����萔�����Ĕz�u
	_vPos = VGet(PlPos.x - 50, PlPos.y + POS_UP, PlPos.z * -1);
	CenterPos = _vPos;

	//�f�t�H��-�������Ă�
	_vDir = VGet(0, 0, -1);

	//�v���C���[�Ƃ̋����̃f�t�H���g������Ă���
	NormalLengh = VSub(_vPos, PlPos);
	NormalRad = atan2(VSquareSize(PlDir), VSquareSize(_vPos));

	//�����蔻��
	_collision_r = 30.f;

	//�^�[�Q�b�g�̃n���h��
	RockonHandle = LoadGraph("res/OtomoRobo/target.PNG");

	return true;
}


bool Fellow::Process(VECTOR Ppos, VECTOR PlDir, VECTOR MoveValue) {

	//�G�Ƃ̔���s����
	int checkNum = 0;

	//�G�Ƃ̋����ۑ��p
	VECTOR distToEnemy = VGet(0, 0, 0);

	//�G�̃C�e���[�^�[
	for (auto& vEn : EnemyManager::GetInstance()->GetEnemy()) {
		
		//�G�̈ʒu�擾
		VECTOR enPos = vEn->GetPos();

		//�˒������O�Ȃ�X�L�b�v
		if (VSize(VSub(_vPos, enPos)) > 1000) {
			continue;
		}
		else {

			//�G�̐��𐔂���
			checkNum++;
			//�G�Ƃ̋���
			VECTOR len = VSub(_vPos, enPos);

			//���̋������O�ۑ������������Z���Ȃ�G�̈ʒu�X�V
			if (VSize(len) < VSize(distToEnemy) || VSize(distToEnemy) == 0) {

				distToEnemy = len;
				Epos = vEn->GetPos();
			}
		}
	}

	//�˒��������ɓG�����Ȃ��Ȃ�^�[�Q�b�g������
	useHandle = (checkNum != 0);

	
	//�v���C���[���
	VECTOR PlUp = VGet(Ppos.x, Ppos.y + 50, Ppos.z);			//�v���C���[�̃J�v�Z���̏�
	FireRate = Player::GetPLInstance()->GetJumpCast();			//���˃��[�g�i�W�����vCT�j
	int FlCheckAtk = Player::GetPLInstance()->GetCheckAtack();	//�U�����Ă������ǂ���


	//�v���C���[���U���W�����v����������U���t�F�[�Y
	if (FlCheckAtk == 1 && checkNum >= 1) {

		//�U���t�F�[�Y
		PhaseAttack(PlUp, PlDir);
	}
	//�v���C���[���U���W�����v�������ĂȂ��Ȃ�ړ��t�F�[�Y
	else if (FireRate <= 0 || FlCheckAtk == 0) {

		Player::GetPLInstance()->SetCheckAtack(0);
		FiredNum = 0;
		PhaseMove(Ppos, PlDir);
	}

	//�e�̏���
	for (int i = 0; i < BULLET_MAX; i++) {

		//�e���A�N�e�B�u�Ȃ珈��
		CheckBlActive[i] = bullet.GetActive(i);
		if (!CheckBlActive[i]) {

			//�e�̏���
			bullet.BulletProcess(i);
			BLPos[i] = bullet.GetPos(i);
			CheckBlActive[i] = bullet.GetR(i);
		}
	}
		
	return true;
}


bool Fellow::Render(int type) {

	type = 1;

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(handle, _attach_index, _play_time);

	// �ʒu
	MV1SetPosition(handle, _vPos);

	// ��������Y����]���Z�o
	VECTOR vRot = { 0,0,0 };

	// ���f�����W���łǂ���������Ă��邩�Ŏ����ς��(�����-z�������Ă���ꍇ)
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);
	MV1SetRotationXYZ(handle, vRot);

	Rend(type);

	//�^�[�Q�b�g
	if(useHandle != false){
		DrawBillboard3D(Epos, 0.5f, 0.5f, 100, 0.0f, RockonHandle, TRUE);
	}

	//�e
	for (int i = 0; i < 100; i++) {
		bullet.BulletRender(i);
	}

	if (AMG_wp::CheckUseCol == true) {

		DrawFormatString(0, 700, GetColor(255, 0, 0), "Pposx = %f", CheckV.x);
		DrawFormatString(0, 720, GetColor(255, 0, 0), "Pposy = %f", CheckV.y);
		DrawFormatString(0, 740, GetColor(255, 0, 0), "Pposz = %f", CheckV.z);
		DrawFormatString(0, 600, GetColor(255, 0, 0), "time = %d", time);
		DrawFormatString(0, 650, GetColor(255, 0, 0), "CheckMove = %d", CheckMove);

		DrawLine3D(CheckV, CheckV2, GetColor(255, 255, 255));
		DrawLine3D(CheckV, _vPos, GetColor(0, 0, 0));
	}

	return true;
}


void Fellow::PhaseAttack(VECTOR PlUp, VECTOR PlDir) {
	
	//������G�Ɍ����āA�ʒu���v���C���[�ƓG�����񂾐����
	_vDir = VSub(Epos, _vPos);
	_vPos = VAdd(PlUp, VScale(VNorm(VSub(Epos, PlUp)), VSize(NormalLengh)));

	//��x�����ʔ��˂���悤��
	if (FiredNum == 0) {

		FiredNum = 1;
		bullet.AddBullet(_vPos, Epos, BULLET_TEX_FELLOW);
	}
}

void Fellow::PhaseMove(VECTOR Ppos, VECTOR PlDir) {

	//�v���C���[�ƌ����Ă�����𓯊�������
	_vDir = PlDir;

	//�ڕW���߂邽�߂̉�����
	VECTOR SanplePos = _vPos;
	float rad = atan2(PlDir.x, PlDir.z);

	//�ɍ��W�ł����̈ړ����ۑ����Ă���
	SanplePos.x = Ppos.x + cos(-rad) * DISTANCE;
	SanplePos.z = Ppos.z + sin(-rad) * DISTANCE;
	SanplePos.y = Ppos.y + POS_UP;

	//�ړ���ƍ��̍��W�ňړ��p�x�N�g�����
	VECTOR Len = VSub(SanplePos, _vPos);
	if (VSize(Len) > 5) {
		//�������ۑ������ꏊ�Ƃ̋��������񂾂�k�߂Ă�
		//�Ō�̐�����ς���ƈړ����x�ς�����
		_vPos = VAdd(_vPos, VScale(VNorm(Len), VSize(Len) / 10));
	}
}