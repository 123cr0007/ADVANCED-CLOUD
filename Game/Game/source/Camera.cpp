#include "Camera.h"
#include "Player.h"

Camera* Camera::CamInstance = nullptr;
Camera::Camera() {

	CamInstance = this;
	// �J�����̐ݒ�i�킩��₷���ʒu�Ɂj
	
	_clipNear = 10.f;
	_clipFar = 500000.f;
	_vDirection = 0.f;

	radY = DegToRad(180);	//Y����]
	radH = 0.3f;		//�����̊p�x
	CamDist = DEFAULT_DIST;	//�J�����Ƃ̋���
	ChengeDist = 0.f;	//�J�����Ƃ̋�����L�k����Ƃ��̒l�����悤

	//�L�����̓����x�Z�b�g�p
	//���̐����Ɍ��݂̊p�x�|OPACITY_START�̒l����Ďg��
	opacityRate = static_cast<float>(1) / (OPACITY_END - OPACITY_START);
}

Camera::~Camera() {


}

bool Camera::Initialize() {

	// �J�����̏����ʒu�ݒ�
	if (ModeServer::GetInstance()->Get("Game") != NULL) {

		if (AMG_wp::StageNum < STAGE_MAX) {
			_vPos = VGet(AMG_wp::StartPos[AMG_wp::StageNum].x - DEFAULT_DIST, AMG_wp::StartPos[AMG_wp::StageNum].y + 700.f
				, AMG_wp::StartPos[AMG_wp::StageNum].z);
			_vTarget = VGet(AMG_wp::StartPos[AMG_wp::StageNum].x, AMG_wp::StartPos[AMG_wp::StageNum].y + 50.f
				, AMG_wp::StartPos[AMG_wp::StageNum].z);
		}
	}
	else {
		_vTarget = VGet(0,0,0);
		_vPos = VGet(0,0,300);
	}
	
	return true;
}

bool Camera::Process() {

	GamePad::MULTIPAD pad = GamePad::GetInstance()->GetMultiPad(PLAYER_1);

	float rx, ry,lx,ly;	// ���E�A�i���O�X�e�B�b�N�̍��W
	float analogMin = 0.3f;	// �A�i���O臒l
	// Logicool�p�b�h�̏ꍇ
	rx = pad.RX / analogMin;
	ry = pad.RY / analogMin;
	lx = pad.LX / analogMin;
	ly = pad.LY / analogMin;

	// �J��������
	{
		// �e���Ń^�[�Q�b�g�i�v���C���[�j�Ƃ̋������Ƃ�
		float sx = _vPos.x - _vTarget.x;
		float sz = _vPos.z - _vTarget.z;
		float sy = _vPos.y - _vTarget.y;

		//���ʍ��W�ŃJ�����ƃv���C���[�̋������Ƃ�
		float lengthY = sqrt(sz * sz + sx * sx);

		//�ォ�猩�����ʍ��W��Y������]����
		if (rx > analogMin) { radY -= 0.05f; }
		if (rx < -analogMin) { radY += 0.05f; }

		//Y������
		_vPos.x = _vTarget.x + cos(radY) * lengthY;
		_vPos.z = _vTarget.z + sin(radY) * lengthY;

		//Y���̉�]�ʂƂǂꂾ�������邩�̊p�x�ŃJ�����̈ʒu��ݒ肷��i��]���Ă���킯�ł͂Ȃ��j
		if (radH <= 1.49) {
			if (ry > -analogMin) {

				radH += 0.05f;
			}
		}
		if (radH >= -1.49) {
			if (ry < analogMin) {

				radH -= 0.05f;
			}
		}
		
		//L1�{�^���ŃJ�����̋�����ς���
		if (pad.key & PAD_INPUT_5) {

			if (CamDist > 0) {
				CamDist -= 1.f;
			}
		}

		//R1�{�^���ŃJ�����̋�����ς���
		if (pad.key & PAD_INPUT_6) {

			if (CamDist < DEFAULT_DIST) {
				CamDist += 1.f;
			}
		}
		//Y���̉�]�iradY�j��X���̉�]�iradX�j�𑫂��āA�������������ʒu�ɃJ������u�� CamDist sin(radY)
		//�ɍ��W�̕ϊ�
		// x = r*cos��*cos��
		// z = r*cos��*sin��
		// y = r*sin��
		//�{���͂��Ƃ����t�A�������_����̊p�x�Ȃ̂�sin�ɂȂ��Ă�
		_vPos.x = _vTarget.x + CamDist * cos(radH) * cos(radY);
		_vPos.z = _vTarget.z + CamDist * cos(radH) * sin(radY);
		_vPos.y = _vTarget.y + CamDist * sin(radH);

		//�J�����̊p�x�����ăv���C���[�̓����x��[�肷��
		if (RadToDeg(radH) < -OPACITY_START) {

			float chageRate = (-OPACITY_START - RadToDeg(radH));
			Player::GetPLInstance()->SetCherOpacity(1 - (opacityRate * chageRate));
		}

		//�J�����ƃv���C���[�̊Ԃɏ�Q��������΂��̑O�ɃJ�����u������
		MV1_COLL_RESULT_POLY hitPolyPos;
		for (auto ite = Stage::GetSTInstance()->GetBlockPositions().begin();
			ite != Stage::GetSTInstance()->GetBlockPositions().end(); ++ite) {

			hitPolyPos = MV1CollCheck_Line((*ite).handle, (*ite).Collision,
				_vTarget,_vPos);

			if (hitPolyPos.HitFlag == 1) {

				_vPos = hitPolyPos.HitPosition;
				break;
			}

		}
	}

	return true;
}

bool Camera::Render() {

	// �J�����ݒ�X�V
	SetCameraPositionAndTarget_UpVecY(_vPos, _vTarget);
	SetCameraNearFar(_clipNear, _clipFar);

	DrawFormatString(1800, 0, GetColor(255, 0, 0), "%d", AMG_wp::CheckUseCol);

	return true;
}