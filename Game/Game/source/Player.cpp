#include "DxLib.h"
#include "ApplicationMain.h"

#include "Player.h"
#include "enemy.h"
#include "Stage.h"
#include "Camera.h"
#include "Effekseer.h"
#include "EnergyGauge.h"


// 派生クラスのコンストラクタに : で続けて基底クラスのコンストラクタを呼べる
Player* Player::PLInstance = nullptr;
Player::Player() : ObjectBase() {

	// プレイヤーのインスタンスを設定
	PLInstance = this;

	//重力関連初期化
	gravity = 0;
	OldCheckGrv = false;
	CheckGravity = false;
	CheckJump = JUMP_TYPE_NONE;
	Srow = 0;
	FloatingTime = 0;

	//カメラ関連初期化
	camPos = VGet(0, 0, 0);
	camTgtPos = VGet(0, 0, 0);

	//移動値関連初期化
	MoveV = VGet(0, 0, 0);
	oldMoveV = VGet(0, 0, 0);

	//ジャンプ関連初期化
	jumptype = -1;//0番から使いたいので-1。
	jumpTime = 60;
	release_jump = 0;
	CheckAtack = 0;

	//透明度初期化
	cherOpacity = 1;

	//mにメーション関連初期化
	animBrendRate = 0;
	animBrendIndex = -1;
	idleTime = 0;

	Cr = GetColor(0, 0, 0);
	
	//ここキャラの高さによって変える
	_colSubY = 50.f;

	//ワープ関連
	CheckAvoid = 0;
	AvoidTime = 0;
	PlSize = VGet(1, 1, 1);
	OldPos = VGet(0, 0, 0);

	//影
	ShadowPos = VGet(0, 0, 0);
	CheckMakeShadow = false;

	//ジャンプ関連	初期化
	JumpCast = 0;

	//エネルギー関連初期化
	restEnergy = 10;
	plUseEnergy = 0;
	jumpTime = 0;
	chargeEnergy = 0;
	checkHealEnergy = 0;

	ChHight = 150;

	//デバッグ関連初期化
	CP = VGet(0, 0, 0);
	CheckNum = 0;
	grvNum = 0;
	muveNum = 0;

	ModePhoto = false;

	//操作できるか
	if (AMG_wp::ScenarioTrg[AMG_wp::StageNum] == true) {
		isOperate = false;
	}
	else {
		isOperate = true;
	}

	//ステージ進行関連
	changeMap = false;
	stageTrg = false;
	gameOver = false;
	dead = false;
	gameClear = false;

	//音関連初期化
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
	//ハンドルの削除
	DeleteEffekseerEffect(ef_handle);
	MV1DeleteModel(handle);
}

bool Player::Initialize() {

	// 基底(ObjectBase)クラス の Load 関数をつかってロードする
	//handle = Load(_T("res/Player/PLSumple.mv1"), 1);
	handle = Load(_T("res/Player/chara_motion_data_0228.mv1"), 1);
	
	// モデルの位置を設定
	if (ModeServer::GetInstance()->Get("Game") != NULL) {

		if (AMG_wp::StageNum < STAGE_MAX) {

			_vPos = AMG_wp::StartPos[AMG_wp::StageNum];
			//デフォで-ｚ向いてる
			_vDir = VGet(1, 0, 0);
		}
	}
	else {
	
		_vPos = VGet(100, -120, 150);
		//カメラの方 左：左右　真ん中:高さ　右-奥行 150, -80, 150
		_vDir = VSub(VGet(0,0,300),_vPos);
	}

	_collision_r = 20.f;

	//カプセルの位置初期化
	CapsuleTop = VGet(_vPos.x, _vPos.y + CUPSLE_UPOS, _vPos.z);
	CapsuleRow = VGet(_vPos.x, _vPos.y + CUPSLE_DPOS, _vPos.z);

	return true;
}

bool Player::Process() {

	//デバッグ用
	if (AMG_wp::CheckUseCol == true) {
		grvNum = JUMP_POWER * 3;
		muveNum = PL_MOVE_SPEED * 3;
	}
	else {
		grvNum = JUMP_POWER;
		muveNum = PL_MOVE_SPEED;
	}

	VECTOR oldPlPos = _vPos;

	//初期化色々
	OldCheckGrv = CheckGravity;

	//プロセス色々
	if (isOperate == true) {

		//操作部分
		ProcessOperate();
	}
	
	//アニメーション
	Animetion();

	//移動周りの処理
	//重力→壁との当たり判定（カプセル）→地面との当たり判定（レイ）
	ProcessGrv();
	ProcessCapsule();
	ProcessRand();
	ProcessMove();
	
	//影
	MakeShadow();
	

	//敵の弾との当たり判定
	if (Enemy::GetENInstance() != nullptr) {
		for (int n = 0; n < BULLET_MAX; n++) {
			int a = Enemy::GetENInstance()->GetCheckActive(n);
			if (Enemy::GetENInstance()->GetCheckActive(n) != 0) {
				
				VECTOR FlBlPos = Enemy::GetENInstance()->GetBlPos(n);
				float flBlR = Enemy::GetENInstance()->GetBlR(n);

				if (CheckHitPointCapsule(CapsuleTop, CapsuleRow, FlBlPos, 30, flBlR) == true) {

					//エネルギー-1（1ダメージ）
					plUseEnergy = 1;

					//ダメージ受けたトリガオン
					isDamage = true;

					//残ったエネルギーが0だった場合死亡処理
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

	//エネルギー回復するか
	if (!CheckGravity) {

		checkHealEnergy = 1;
	}
	else {

		checkHealEnergy = 0;
	}

	//ジャンプ初期化
	CheckJump = JUMP_TYPE_NONE;

	//カプセルの位置更新
	CapsuleTop = VGet(_vPos.x, _vPos.y + CUPSLE_UPOS, _vPos.z);
	CapsuleRow = VGet(_vPos.x, _vPos.y + CUPSLE_DPOS, _vPos.z);

	return true;
}

bool Player::Render(int type) { 

	//3D描画
	type = 1;

	// 位置
	MV1SetPosition(handle, _vPos);
	
	// 向きからY軸回転を算出
	// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);		
	MV1SetRotationXYZ(handle, vRot);

	//透明度
	MV1SetOpacityRate(handle, cherOpacity);


	Rend(type);

	//影
	if (CheckMakeShadow != false) {
		for (int i = CIRCLE_CORNAR - 2; i > 0; i--) {
			DrawTriangle3D(ShadowPos, ShadowCircle[i + 1], ShadowCircle[i], GetColor(80, 80, 80), TRUE);
		}
	}

	//デバッグ用表示関連
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

//タイトル用プロセス
bool Player::ProcessForTitle() {

	Animetion();
	return true;
}


//タイトル用描画
bool Player::RenderForTitle() {

	// 位置
	MV1SetPosition(handle, _vPos);

	// 向きからY軸回転を算出
	// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	VECTOR vRot = { 0,0,0 };
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);
	MV1SetRotationXYZ(handle, vRot);

	MV1DrawModel(handle);
	return true;
}

//影の処理
bool Player::MakeShadow() {
	
	//初期化色々
	MV1_COLL_RESULT_POLY MadeShadow;
	VECTOR Len = VGet(-1, -1, -1);
	VECTOR OldLen = VGet(0, 0, 0);
	int HitNum = 0;

	//ステージのイテレーターを回す
	for (auto ite : Stage::GetSTInstance()->GetBlockPositions()) {

		//プレイヤーの腰から足元に向けてレイを出す
		MadeShadow = MV1CollCheck_Line((ite).handle, (ite).Collision,
			VAdd(_vPos, VGet(0, _colSubY, 0)),
			VAdd(_vPos, VGet(0, -99999999.0f, 0)));
		//当たったら
		if (MadeShadow.HitFlag == 1) {

			//影出すトリガオン
			CheckMakeShadow = true;

			//あたった数+1
			HitNum++;

			//当たった距離を保存しておく
			OldLen = Len;

			//距離更新
			Len = VSub(MadeShadow.HitPosition, _vPos);
			
			//一番距離が近いハンドルに影を落とす
			if (HitNum == 1 || VSize(OldLen) > VSize(Len)) {

				//マップモデルと被らないように影の位置を少しあげる
				ShadowPos = VGet(MadeShadow.HitPosition.x, MadeShadow.HitPosition.y + 1.0f,
					MadeShadow.HitPosition.z);

				//辺の数に応じて角の位置を指定する
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


//ワープジャンプの消える処理
bool Player::Vanish(VECTOR GoalPos, VECTOR StartPos) {

	float size = 0.5f; //1フレごとに小さくする数
	AvoidTime++;

	//サイズが一定以上なら小さくする
	if (VSize(MV1GetScale(handle)) > 0.000001f) {

		PlSize = VScale(PlSize, size);
		MV1SetScale(handle, PlSize);
	}

	//目標地点までの距離
	VECTOR len = VSub(GoalPos, StartPos);

	//移動量
	float MoveNum = VSize(len) / FRAME_TO_MOVE;

	//長さを単位化したやつ
	VECTOR Nlen = VNorm(len);

	//指定された時間未満なら移移動するする
	if (AvoidTime < FRAME_TO_MOVE) {
		_vPos = VAdd(_vPos, VScale(Nlen, MoveNum));
	}
	else {
		
		CheckAvoid = 2;
		AvoidTime = 0;
	}

	return true;
}

//出現する処理
bool Player::arrival() {

	//1フレごとに大きくする数
	float size = 1.8f;

	//等倍未満なら拡大する
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

//プレイヤーの操作部分
bool Player::ProcessOperate() {

	VECTOR v = { 0,0,0 };	// 移動量

	//パッド入力を取得   
	GamePad::MULTIPAD pad  = GamePad::GetInstance()->GetMultiPad(PLAYER_1);

	//カメラの位置を所得
	camPos = Camera::GetCamInstance()->GetCamPos();
	camTgtPos = Camera::GetCamInstance()->GetTgtPos();


	if(pad.LX != 0 || pad.LY != 0)//入力があった
	{
		VECTOR lstick = { 0,0,0 };	// 左右アナログスティックの座標
		float analogMin = 0.3f;	// アナログ閾値

		lstick.x = pad.LX;
		lstick.y = pad.LY;

		//入力を変換
		lstick = VNorm(lstick);

		// カメラの向いている角度を取得
		float sx = camPos.x - camTgtPos.x;
		float sz = camPos.z - camTgtPos.z;
		float camrad = atan2(sz, sx);

		// キャラ移動(カメラ設定に合わせて)
		float mvSpeed = muveNum;

		// アナログ左スティック用
		float length = sqrt(lstick.x * lstick.x + lstick.y * lstick.y);
		float rad = atan2(lstick.x, lstick.y);
		if(length < analogMin) {
			// 入力が小さかったら動かなかったことにする
			length = 0.f;
		}
		else {
			length = mvSpeed;
		}

		// vをrad分回転&移動させる
		v.x = cos(rad + camrad) * length;
		v.z = sin(rad + camrad) * length;

		//回避ジャンプは移動が特殊なためそれ以外は移動値保存
		if(CheckAvoid == 0) {

			OldPos = _vPos;
		}
	}

	//デバッグモード中ゴール手前にワープできるように
	if (AMG_wp::CheckUseCol) {
		if (pad.trg & PAD_INPUT_5) {
			_vPos = VGet(-8566,45451,8564);
		}
	}
	
	//エネルギーが残っててジャンプのCTがあけているなら
	if(JumpCast == 0) {
		if(restEnergy > 0) {

			//通常ジャンプ
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

			//ゆっくり落ちるジャンプ
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

			//回避ジャンプ
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

			//攻撃ジャンプ
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

	//重力処理
	{
		//処理前の重力値保存
		float OldG = gravity;
		//とりあえず重力下げる
		//ここの値変えれば移動値変わる
		gravity -= CHANGE_GRV_NUM;

		//重力が働く状況なら重力処理
		if (CheckGravity) {

			//ここで値代入
			v.y += gravity;
			//入力がゆっくりジャンプなら重力の下限値をつける
			if (Srow == 1) {
				if (gravity <= -5) {

					gravity = OldG;
				}
			}
		}
		else {
			//重力処理しないなら重力値を０にしておく
			gravity = 0;
			//ジャンプしていないので-1にしておく
			jumptype = -1;
		}
	}

	MoveV = v;
	return true;
}


bool Player::ProcessCapsule() {

	VECTOR v = MoveV;
	//当たり判定に使うカプセルの情報
	MV1_COLL_RESULT_POLY_DIM HitPolyCapsule;
	VECTOR CapsulePos1 = CapsuleRow;
	CapsulePos1 = VAdd(CapsulePos1, v);

	VECTOR CapsulePos2 = CapsuleTop;
	CapsulePos2 = VAdd(CapsulePos2, v);

	//マップのイテレーター
	for (auto& ite : Stage::GetSTInstance()->GetBlockPositions()) {

		// モデルとマップとの当たり判定
		HitPolyCapsule = MV1CollCheck_Capsule((ite).handle, (ite).Collision,
			CapsulePos1, CapsulePos2, _collision_r);

		if (HitPolyCapsule.HitNum >= 1) {

			const int HitNum = HitPolyCapsule.HitNum;
			int i = 0;

			// 当たったポリゴンの数だけ繰り返し
			for (i = 0; i < HitPolyCapsule.HitNum; i++)
			{

				//線分と三角形の最近点二つもらう
				tagSEGMENT_TRIANGLE_RESULT resurt;
				Segment_Triangle_Analyse(&CapsulePos1, &CapsulePos2, &HitPolyCapsule.Dim[i].Position[0],
					&HitPolyCapsule.Dim[i].Position[1], &HitPolyCapsule.Dim[i].Position[2], &resurt);

				//ベクトルの引き算で押し出す角度を取る
				VECTOR PushDir = VSub(resurt.Seg_MinDist_Pos, resurt.Tri_MinDist_Pos);

				//線分と三角形の最近点間
				float NearestPoints = Segment_Triangle_MinLength(CapsulePos1, CapsulePos2,
					HitPolyCapsule.Dim[i].Position[0], HitPolyCapsule.Dim[i].Position[1], HitPolyCapsule.Dim[i].Position[2]);
				//プレイヤーの押しだす距離
				float SubLen = _collision_r - NearestPoints;

				//法線方向に上で求めた距離を掛ける
				VECTOR PushVec = VScale(VNorm(PushDir), SubLen + 1.0f);

				//移動
				_vPos = VAdd(_vPos, PushVec);
				//カプセルの位置更新
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

	//移動後処理
	//ジャンプ用レイ
	MV1_COLL_RESULT_POLY hitPolyG;
	int CheckHit = 0;
	int Num = 0;
	float HitPosY = 0.0f;
	float OldHitPosY = 0.0f;

	for (auto& ite : Stage::GetSTInstance()->GetBlockPositions()) {

		//空中にいるとき（重力を使ってるとき）と地上にいるときで処理かえる
		if (!CheckGravity) {
			//地上にいるとき
			//ジャンプに引っかからないようにジャンプ力より少し短いレイ出す
			hitPolyG = MV1CollCheck_Line((ite).handle, (ite).Collision,
				VAdd(_vPos, VGet(0, _colSubY, 0)),
				VAdd(_vPos, VGet(0, -JUMP_POWER + 1.0f, 0)));

			CP = VAdd(_vPos, VGet(0, -1000.0f, 0));
			Cr = GetColor(255, 0, 0);
		}
		else {
			//空中にいるとき
			//地面の貫通防止で重力の値をそのまま入れる
			hitPolyG = MV1CollCheck_Line((ite).handle, (ite).Collision,
				VAdd(_vPos, VGet(0, _colSubY, 0)),
				VAdd(_vPos, VGet(0, gravity, 0)));

			CP = VAdd(_vPos, VGet(0, gravity, 0));
			Cr = GetColor(0, 0, 255);
		}

		//当たった時の処理は共通
		if (hitPolyG.HitFlag == 1) {

			OldHitPosY = hitPolyG.HitPosition.y;
			CheckHit++;
			float len = _vPos.y - OldHitPosY;

			if (OldHitPosY > len || HitPosY == 0.0f) {

				HitPosY = hitPolyG.HitPosition.y;
				mapName = (ite).name;
				String = mapName.c_str();
				stageTrg = (ite).AdvanceStage;

				//教室踏んだ時
				if ((ite).stepClassroom == true) {
					//ステージ側のトリガオフ
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
		
		//クリスタルとの距離が一定以下ならステージ進める
		if ((ite).modelNo == 21 && (ite).AdvanceStage == true) {
			
			VECTOR len = VSub(_vPos,VGet(ite.x, ite.y, ite.z));
			if (VSize(StLen) == 0 || VSize(StLen) > VSize(len)) {

				//StLen = len;
				StPos = VGet(ite.x, ite.y, ite.z);
			}
			//距離が一定以下ならステージ進行
			if (VSize(VSub(_vPos, StPos)) < 1000) {
				//ステージ側のトリガオフ
				Stage::GetSTInstance()->SetAdvanceStage(Num, false);
				StLen = VGet(0, 0, 0);

				//ステージが最後以外ならチュートリアル
				if (AMG_wp::StageNum < STAGE_MAX - 1) {
					changeMap = true;
				}

				//ステージを進める
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

		// 当たった
		// 当たったY位置をキャラ座標にする
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

//移動処理
bool Player::ProcessMove() {

	VECTOR v = MoveV;

	//回避用移動処理
	switch (CheckAvoid)
	{
	case 1:
		Vanish(VGet(VAdd(OldPos, VScale(v, MOVIVG_FRONT)).x, OldPos.y + MOVIVG_HEIGHT, VAdd(OldPos, VScale(v, MOVIVG_FRONT)).z), OldPos);
		break;
	case 2:
		arrival();
		break;
	}

	//通常の移動
	if (CheckAvoid == 0) {
		_vPos = VAdd(_vPos, v);
	}

	//プレイヤーの向きを指定
	if (v.x != 0 || v.z != 0) {

		_vDir = v;
	}

	//操作可能なら移動
	if (!isOperate) {
		MoveV = v;
	}

	return true;
}

//アニメーション
bool Player::Animetion() {

	VECTOR v = MoveV;
	VECTOR Move = VGet(1, 1, 1);
	
	oldStatus = _status;

	//生きているか
	if (_status != STATUS::DOWN) {
		//同じフレーム内でジャンプ入力がないか
		if (CheckJump == JUMP_TYPE_NONE) {
			//ジャンプしている場合踏切モーションが終わっているか
			if (_status != STATUS::JUMP_START_R) {
				if (_status != STATUS::JUMP_START_L) {

					//移動中か
					if (VSize(v) > 0.f) {

						//止まっていたら走り始めのモーション
						if (oldStatus == STATUS::WAIT)
						{
							_status = STATUS::RUN_START;
						}
						else {

							_status = STATUS::RUN;
						}
					}
					else {

						//移動してない場合各終わりのモーションに繋げる
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

					//空中にいる間は落下モーション
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
		//連続でジャンプする場合は左右交互に足を踏み出す感じに
		else {
			if (_status == STATUS::FALL_L) {
				_status = STATUS::JUMP_START_R;
			}
			else {
				_status = STATUS::JUMP_START_L;
			}
		}

		//着地修正用
		if (OldCheckGrv == true && CheckGravity == false) {
			if (_status == STATUS::FALL_R) {
				_status = STATUS::JUMP_END_R;
			}
			else {
				_status = STATUS::JUMP_END_L;
			}
		}
	}

	//死んでいるなら死亡モーション
	if (dead == true) {
		_status = STATUS::DOWN;
	}

	//最初はアイドルモーションから
	if (_status == STATUS::NONE) {
		_status = STATUS::WAIT;
	}


	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (playTime >= totalTime) {
		
		//ステージ進行トリガオン
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
			//各スタートからそのモーションへ
		case STATUS::JUMP_START_R:
			_status = STATUS::FALL_R;
			break;

		case STATUS::JUMP_START_L:
			_status = STATUS::FALL_L;
			break;

		case STATUS::RUN_START:

			_status = STATUS::RUN;
			break;

			//終わりならアイドルモーションへ
		case STATUS::RUN_END:
		case STATUS::JUMP_END_R:
		case STATUS::JUMP_END_L:
		case STATUS::IDLE_2:
			_status = STATUS::WAIT;
			break;

			//ループモーションなら時間を最初に
		case STATUS::FALL_R:
		case STATUS::FALL_L:
		case STATUS::WAIT:
		case STATUS::RUN:

			playTime = 0.0f;
			break;
		}
	}

	//アイドルモーションのまま一定時間入力がなければ待機アニメーション流す
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

	// ステータスが変わっていないか？
	if (oldStatus != _status) {

		//ブレンド用ステータスに前のステータス保存
		//stateBrend = oldStatus;
		animBrendIndex = MV1GetAttachAnim(handle, attachIndex);
		animBrendPlayTime = playTime;
		animBrendTotalTime = totalTime;
		animBrendRate = 1;


		if (attachIndex != -1) {
			MV1DetachAnim(handle, attachIndex);
			attachIndex = -1;
		}
		// ステータスに合わせてアニメーションのアタッチ
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

		// アタッチしたアニメーションの総再生時間を取得する
		totalTime = MV1GetAttachAnimTotalTime(handle, attachIndex);

		// 再生時間を初期化
		playTime = 0.0f;
	}
	else {

		// 再生時間を進める
		//ジャンプの初めの場合のみCTとモーションの長さが噛み合うように調整
		if (_status == STATUS::JUMP_START_R || _status == STATUS::JUMP_START_L) {

			//ぴったりにするとなんか変なのでちょっと足す
			playTime += totalTime/ JUMP_CT + 0.5f;
			animBrendPlayTime += totalTime / JUMP_CT + 0.5f;
		}
		//それ以外なら普通に流す
		else {
			playTime += 1.f;
			animBrendPlayTime += 1.f;
		}
	}

	if (animBrendPlayTime >= animBrendTotalTime) {
		animBrendPlayTime = 0;
	}

	//走り終わりの少し滑るやつ
	//ピタッと止まると見た目と合わないため実装
	if (_status == STATUS::RUN_END) {

		//移動力を減算する値
		auto animMoveNum = PL_MOVE_SPEED / totalTime;

		//減算値を今のモーション時間分大きくする
		if (playTime * 3 < totalTime) {
			VECTOR Move = VScale(VNorm(VGet(_vDir.x, 0, _vDir.z)),
				animMoveNum * (totalTime - playTime * 3));
			/*_vPos = VAdd(_vPos,VSub(Move, v));*/

			v = VAdd(v, Move);
		}
	}


	//アニメーションをアタッチする
	MV1SetAttachAnimTime(handle, attachIndex, playTime);
	
	MoveV = v;
	return true;
}