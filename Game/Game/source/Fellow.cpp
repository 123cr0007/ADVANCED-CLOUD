#include "DxLib.h"
#include "Fellow.h"
#include "enemy.h"
#include "EnemyManager.h"
#include "ApplicationMain.h"


// 派生クラスのコンストラクタに : で続けて基底クラスのコンストラクタを呼べる
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

	

	// 基底(ObjectBase)クラス の Load 関数をつかってロードする
	handle = Load(_T("res/OtomoRobo/OtomoRobo.mv1"), 1);

	//プレイヤーの位置から一定数離して配置
	_vPos = VGet(PlPos.x - 50, PlPos.y + POS_UP, PlPos.z * -1);
	CenterPos = _vPos;

	//デフォで-ｚ向いてる
	_vDir = VGet(0, 0, -1);

	//プレイヤーとの距離のデフォルトを取っておく
	NormalLengh = VSub(_vPos, PlPos);
	NormalRad = atan2(VSquareSize(PlDir), VSquareSize(_vPos));

	//当たり判定
	_collision_r = 30.f;

	//ターゲットのハンドル
	RockonHandle = LoadGraph("res/OtomoRobo/target.PNG");

	return true;
}


bool Fellow::Process(VECTOR Ppos, VECTOR PlDir, VECTOR MoveValue) {

	//敵との判定行う数
	int checkNum = 0;

	//敵との距離保存用
	VECTOR distToEnemy = VGet(0, 0, 0);

	//敵のイテレーター
	for (auto& vEn : EnemyManager::GetInstance()->GetEnemy()) {
		
		//敵の位置取得
		VECTOR enPos = vEn->GetPos();

		//射程距離外ならスキップ
		if (VSize(VSub(_vPos, enPos)) > 1000) {
			continue;
		}
		else {

			//敵の数を数える
			checkNum++;
			//敵との距離
			VECTOR len = VSub(_vPos, enPos);

			//今の距離が前保存した距離より短いなら敵の位置更新
			if (VSize(len) < VSize(distToEnemy) || VSize(distToEnemy) == 0) {

				distToEnemy = len;
				Epos = vEn->GetPos();
			}
		}
	}

	//射程距離内に敵がいないならターゲットを消す
	useHandle = (checkNum != 0);

	
	//プレイヤー情報
	VECTOR PlUp = VGet(Ppos.x, Ppos.y + 50, Ppos.z);			//プレイヤーのカプセルの上
	FireRate = Player::GetPLInstance()->GetJumpCast();			//発射レート（ジャンプCT）
	int FlCheckAtk = Player::GetPLInstance()->GetCheckAtack();	//攻撃していいかどうか


	//プレイヤーが攻撃ジャンプを押したら攻撃フェーズ
	if (FlCheckAtk == 1 && checkNum >= 1) {

		//攻撃フェーズ
		PhaseAttack(PlUp, PlDir);
	}
	//プレイヤーが攻撃ジャンプを押してないなら移動フェーズ
	else if (FireRate <= 0 || FlCheckAtk == 0) {

		Player::GetPLInstance()->SetCheckAtack(0);
		FiredNum = 0;
		PhaseMove(Ppos, PlDir);
	}

	//弾の処理
	for (int i = 0; i < BULLET_MAX; i++) {

		//弾がアクティブなら処理
		CheckBlActive[i] = bullet.GetActive(i);
		if (!CheckBlActive[i]) {

			//弾の処理
			bullet.BulletProcess(i);
			BLPos[i] = bullet.GetPos(i);
			CheckBlActive[i] = bullet.GetR(i);
		}
	}
		
	return true;
}


bool Fellow::Render(int type) {

	type = 1;

	// 再生時間をセットする
	MV1SetAttachAnimTime(handle, _attach_index, _play_time);

	// 位置
	MV1SetPosition(handle, _vPos);

	// 向きからY軸回転を算出
	VECTOR vRot = { 0,0,0 };

	// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
	vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);
	MV1SetRotationXYZ(handle, vRot);

	Rend(type);

	//ターゲット
	if(useHandle != false){
		DrawBillboard3D(Epos, 0.5f, 0.5f, 100, 0.0f, RockonHandle, TRUE);
	}

	//弾
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
	
	//向きを敵に向けて、位置をプレイヤーと敵を結んだ線上に
	_vDir = VSub(Epos, _vPos);
	_vPos = VAdd(PlUp, VScale(VNorm(VSub(Epos, PlUp)), VSize(NormalLengh)));

	//一度だけ玉発射するように
	if (FiredNum == 0) {

		FiredNum = 1;
		bullet.AddBullet(_vPos, Epos, BULLET_TEX_FELLOW);
	}
}

void Fellow::PhaseMove(VECTOR Ppos, VECTOR PlDir) {

	//プレイヤーと向いてる向きを同期させる
	_vDir = PlDir;

	//目標決めるための下準備
	VECTOR SanplePos = _vPos;
	float rad = atan2(PlDir.x, PlDir.z);

	//極座標でお供の移動先を保存しておく
	SanplePos.x = Ppos.x + cos(-rad) * DISTANCE;
	SanplePos.z = Ppos.z + sin(-rad) * DISTANCE;
	SanplePos.y = Ppos.y + POS_UP;

	//移動先と今の座標で移動用ベクトル作る
	VECTOR Len = VSub(SanplePos, _vPos);
	if (VSize(Len) > 5) {
		//さっき保存した場所との距離をだんだん縮めてく
		//最後の数字を変えると移動速度変えられる
		_vPos = VAdd(_vPos, VScale(VNorm(Len), VSize(Len) / 10));
	}
}