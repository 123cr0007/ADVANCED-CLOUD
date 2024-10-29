#pragma once
#include "DxLib.h"
#include "ApplicationMain.h"
#include "Stage.h"

//デフォルトのプレイヤーとカメラの距離
const auto DEFAULT_DIST = 500.f;

//キャラを透明化させ始める角度と完全に透明になる角度
//XZ平面に平行の時を0として、そこからどれだけ下げるか
const auto OPACITY_START = 20;
const auto OPACITY_END = 60;


class Camera {
public:

	static Camera* CamInstance;
	static Camera* GetCamInstance() { return (Camera*)CamInstance; }

	Camera();
	virtual ~Camera();

	static Camera* Instance;
	static Camera* GetInstance() { return (Camera*)Instance; }

	bool Initialize();
	bool Process();
	bool Render();

	//ゲッター
	VECTOR GetCamPos() { return _vPos; };
	VECTOR GetTgtPos() { return _vTarget; };
	float  GetRadH() { return radH; };
	float  GetRadY() { return radY; };
	float  GetClipFar() { return _clipFar; };

	//セッター
	void SetCamPos(VECTOR pos) { _vPos = pos; };
	void SetTgtPos(VECTOR pos) { _vTarget = pos; };

protected:
	VECTOR	_vPos;					// 位置
	VECTOR	_vTarget;				// 距離
	float	_vDirection;			// 角度
	float	_clipNear, _clipFar;	// クリップ

	VECTOR	_vOldPos;					// 位置
	VECTOR	_vOldTarget;				// 距離

	//上から見た時にX軸から何度ずれてるか（何度Y軸を回したか）
	float radY;
	//横から見た時にY=0から何度上がっているか
	float radH;
	//プレイヤーとカメラの距離
	float CamDist;
	float ChengeDist;
	bool OldCheck;

	//キャラの透明度セット用
	float opacityRate;
};