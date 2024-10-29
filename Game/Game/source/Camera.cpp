#include "Camera.h"
#include "Player.h"

Camera* Camera::CamInstance = nullptr;
Camera::Camera() {

	CamInstance = this;
	// カメラの設定（わかりやすい位置に）
	
	_clipNear = 10.f;
	_clipFar = 500000.f;
	_vDirection = 0.f;

	radY = DegToRad(180);	//Y軸回転
	radH = 0.3f;		//高さの角度
	CamDist = DEFAULT_DIST;	//カメラとの距離
	ChengeDist = 0.f;	//カメラとの距離を伸縮するときの値入れるよう

	//キャラの透明度セット用
	//この数字に現在の角度－OPACITY_STARTの値入れて使う
	opacityRate = static_cast<float>(1) / (OPACITY_END - OPACITY_START);
}

Camera::~Camera() {


}

bool Camera::Initialize() {

	// カメラの初期位置設定
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

	float rx, ry,lx,ly;	// 左右アナログスティックの座標
	float analogMin = 0.3f;	// アナログ閾値
	// Logicoolパッドの場合
	rx = pad.RX / analogMin;
	ry = pad.RY / analogMin;
	lx = pad.LX / analogMin;
	ly = pad.LY / analogMin;

	// カメラ操作
	{
		// 各軸でターゲット（プレイヤー）との距離をとる
		float sx = _vPos.x - _vTarget.x;
		float sz = _vPos.z - _vTarget.z;
		float sy = _vPos.y - _vTarget.y;

		//平面座標でカメラとプレイヤーの距離をとる
		float lengthY = sqrt(sz * sz + sx * sx);

		//上から見た平面座標でY軸を回転する
		if (rx > analogMin) { radY -= 0.05f; }
		if (rx < -analogMin) { radY += 0.05f; }

		//Y軸を回す
		_vPos.x = _vTarget.x + cos(radY) * lengthY;
		_vPos.z = _vTarget.z + sin(radY) * lengthY;

		//Y軸の回転量とどれだけあげるかの角度でカメラの位置を設定する（回転しているわけではない）
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
		
		//L1ボタンでカメラの距離を変える
		if (pad.key & PAD_INPUT_5) {

			if (CamDist > 0) {
				CamDist -= 1.f;
			}
		}

		//R1ボタンでカメラの距離を変える
		if (pad.key & PAD_INPUT_6) {

			if (CamDist < DEFAULT_DIST) {
				CamDist += 1.f;
			}
		}
		//Y軸の回転（radY）にX軸の回転（radX）を足して、距離をかけた位置にカメラを置く CamDist sin(radY)
		//極座標の変換
		// x = r*cosθ*cosΦ
		// z = r*cosθ*sinΦ
		// y = r*sinθ
		//本来はｚとｙが逆、ｙが原点からの角度なのでsinになってる
		_vPos.x = _vTarget.x + CamDist * cos(radH) * cos(radY);
		_vPos.z = _vTarget.z + CamDist * cos(radH) * sin(radY);
		_vPos.y = _vTarget.y + CamDist * sin(radH);

		//カメラの角度を見てプレイヤーの透明度を措定する
		if (RadToDeg(radH) < -OPACITY_START) {

			float chageRate = (-OPACITY_START - RadToDeg(radH));
			Player::GetPLInstance()->SetCherOpacity(1 - (opacityRate * chageRate));
		}

		//カメラとプレイヤーの間に障害物があればその前にカメラ置く処理
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

	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_vPos, _vTarget);
	SetCameraNearFar(_clipNear, _clipFar);

	DrawFormatString(1800, 0, GetColor(255, 0, 0), "%d", AMG_wp::CheckUseCol);

	return true;
}