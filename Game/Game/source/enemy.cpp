//敵個体の処理
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
	
	//軽量化のためのもの。
	En_handle = MV1DuplicateModel(En_handle);
	
	_anim = MV1LoadModel("res/Enemy/hoba.mv1");//アニメーションモデルを読み込む
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

		//エネミー本体処理
	if(type == EN_TYPE_MOVE) {

		//エネミーが2点間を運動する関数
		switch(mode)
		{
		case static_cast<int>(MODE::search):
			//エネミーのテクスチャを変更
			MV1SetTextureGraphHandle(En_handle, 2, Tex_color[0], TRUE);
			//エネミーの回転
			Rotate();
			//プレイヤーを索敵
			MakeArea();
			//プレイヤーを見つけたら攻撃モードへ
			if(Search(target))
			{
				//エネミーのテクスチャを変更
				MV1SetTextureGraphHandle(En_handle, 2, Tex_color[1], TRUE);
				//プレイヤーを見つけたフラグを立てる
				IsFind = true;
				//攻撃モードへ
				mode = static_cast<int>(MODE::attack);
			}
			//プレイヤーが範囲外に出たらパトロールモードへ
			if(Move())
			{
				//パトロールモードへ
				mode = static_cast<int>(MODE::idle);
			}
			break;
		//攻撃モード
		case static_cast<int>(MODE::attack):
			//プレイヤー方向へ回転
			Rotate(target);
			//発見時、60f待つ
			if(shot == false && Wait(0, 1))
			{
				//敵のテクスチャを変更
				MV1SetTextureGraphHandle(En_handle, 2, Tex_color[2], TRUE);
				shot = true;
			}
			//いったん発射可能にする
			if(shot)
			{
				Reload();//ここで待ち時間を満たしたか確認
				if(!shot) {
					Shot(target);//プレイヤーへ弾を発射
				}
			}
			//プレイヤーが範囲外に出たら索敵モードへ
			if(SearchOut(target))
			{
				//敵のテクスチャを変更
				MV1SetTextureGraphHandle(En_handle, 2, Tex_color[0], TRUE);
				shot = false;
				Rot.x = 0;
				IsSearchOut = true;
				//索敵モードへ
				mode = static_cast<int>(MODE::search);
			}
			break;

			//ゴールへ着いたので旋回
		case static_cast<int>(MODE::idle):
			//座標交換
			PosSwap();
			IsPatrol = true;
			//旋回モードへ
			mode = static_cast<int>(MODE::rotate);
			break;

			//旋回モード
		case static_cast<int>(MODE::rotate):
			//ゴールへ回転
			if (RotToGoal())
			{
				//索敵モードへ
				mode = static_cast<int>(MODE::search);
			}
			break;
		}
		//アニメーション関連処理
		_play_time += 1.f;
		// アタッチしたアニメーションの総再生時間を取得する
		_total_time = MV1GetAttachAnimTotalTime(En_handle, _attach_index);
		// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
		if(_play_time >= _total_time)
		{
			_play_time = 0.0f;
		}

		//エネミーから打たれる弾の情報
		if(Fellow::GetFLInstance() != nullptr) {
			for(int n = 0; n < BULLET_MAX; n++) {

				//お供の弾もここで管理
				//お供の弾がアクティブなら
				if(Fellow::GetFLInstance()->GetCheckActive(n) != 0) {
					//お供の弾の座標と半径を取得
					FlBlPos = Fellow::GetFLInstance()->GetBlPos(n);
					flBlR = Fellow::GetFLInstance()->GetBlR(n);

					//被弾時処理
					if(CheckHitPointCapsule(VGet(_vPos.x, _vPos.y , _vPos.z), VGet(_vPos.x, _vPos.y + 50, _vPos.z),
						FlBlPos, 30, flBlR) == true) {
						//エネミー死亡
						ef_type = EF_ENEMY_BOMB;
						type = EN_TYPE_DEAD;
						//死亡処理
						Dead();
					}
				}
			}
		}
	}

	//エネミー弾処理
	for(int j = 0; j < BULLET_MAX; j++){
		//弾がアクティブなら
		CheckBlActive[j] = bullet.GetActive(j);
		if (CheckBlActive[j] != 0) {
			//弾の処理を回す
			bullet.BulletProcess(j);
			//弾の座標と半径を取得
			BLPos[j] = bullet.GetPos(j);
			CheckBlActive[j] = bullet.GetR(j);
		}
		//プレイヤー被弾時処理
		if (CheckHitPointCapsule(plPos, VGet(plPos.x, plPos.y , plPos.z),
			bullet.GetPos(j), plRad, bullet.GetR(j)) == true) {
			//弾が当たっているので
			if (!bullet.GetCheckHit(j)) 
			{
				//プレイヤーのエネルギーを減らす
				bullet.SetCheckHit(j, true);
				UseEnergy = 1;
			}
		}
		else{
			//弾が当たっていない
			bullet.SetCheckHit(j, false);
		}
	}
	return true;
}

bool Enemy::Render()
{
	if (type == EN_TYPE_MOVE)
	{
		MV1SetAttachAnimTime(En_handle, _attach_index, _play_time);		// アニメーションをモデルにアタッチ
		MV1SetRotationXYZ(En_handle, Rot);//エネミー回転指定
		MV1SetPosition(En_handle, _vPos);//エネミーポジション指定
		MV1DrawModel(En_handle);//エネミー描画
		
		//デバッグ用
		if (AMG_wp::CheckUseCol == true) 
		{
			DrawCone3D(_tPos, _aPos, r, 16, GetColor(0, 0, 255), GetColor(255, 255, 255), FALSE); 
			DrawFormatString(0, 30, GetColor(0, 0, 255), "_aPos.x=%f,_aPos.y=%f,_aPos.z=%f", _aPos.x, _aPos.y, _aPos.z);
			DrawFormatString(0, 50, GetColor(0, 0, 255), "shot=%d", shot); 
		}
		
	}

	//弾描画
	for (int i = 0; i < BULLET_MAX; i++) {
		bullet.BulletRender(i);
	}
	return true;
}

bool Enemy::Move()	//エネミーが2点間を移動する関数
{
	if(PosMagnitude(_vPos, _gPos) <= 1)//ゴール座標近くに到達したら
	{
		_vPos = _gPos;//強引にゴール座標位置へもっていく
		return true;
	}
	else
	{
		//ゴール座標へ向かって移動
		_vPos = VSub(_vPos, RotateToTarget(_gPos, _vPos));
		return false;
	}
}
	
void Enemy::PosSwap()	//座標交換
{
	VECTOR temp = _sPos;
	_sPos = _gPos;
	_gPos = temp;
}

void Enemy::Rotate()	//ゴール座標へ向く
{
	_vDir = RotateToTarget(_gPos, _vPos);//ゴール座標へ向くベクトルを作成
	Rot.y = atan2(_vDir.x, _vDir.z);
}

void Enemy::Rotate(VECTOR target_pos)	//プレイヤーへ向く
{
	_vDir = RotateToTarget(target_pos, _vPos);//プレイヤーへ向くベクトルを作成

	Rot.x = -atan2(_vDir.y, hypot(_vDir.x, _vDir.z));
	Rot.y = atan2(_vDir.x, _vDir.z);
}

bool Enemy::Search(VECTOR target_pos)	//プレイヤーを索敵
{
	// 作ったベクトルを「単位ベクトル」（スカラが1のベクトル）にする
	_tPos = VGet(_vPos.x, _vPos.y + 60, _vPos.z);
	VECTOR _vA=VGet(0,0,0);
	float angle = Rot.y;

	//円錐の頂点設定
	_aPos = VSub(_aPos, _vPos);
	_vA.x = _aPos.x * cos(angle) - _aPos.z * sin(angle);
	_vA.z = _aPos.x * sin(angle) - _aPos.z * cos(angle);

	_vA=VAdd(_vA,_tPos);
	_aPos = _vA;
	//円錐と点の当たり判定
	if(CheckHitPointCorn(_vPos, _aPos, r, target_pos))
	{
		return true;
	}
	return false;
}

bool Enemy::SearchOut(VECTOR target_pos)	//プレイヤーが範囲外へ逃亡
{
	//プレイヤーが範囲外に出たかどうか
	if(PosMagnitude(target_pos, _vPos) >= ENEMY_SEARCH_AREA * 2)
	{
		return true;
	}
	return false;
}

void Enemy::Shot(VECTOR target_pos)	//プレイヤーへ攻撃
{
	//プレイヤーが死んでいないかどうか
	bool IsPlDead = Player::GetPLInstance()->GetPlayerDeath();
	//プレイヤーが死んでいないなら
	if(!IsPlDead) 
	{
		//弾を発射
		IsShot = true;
		bullet.AddBullet(VGet(_vPos.x, _vPos.y + 30, _vPos.z), target_pos, BULLET_TEX_ENEMY);
	}
	shot = false;
}

void Enemy::Reload()	//弾のリロード
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

bool Enemy::Wait(int i,int second)	//待ち時間
{
	int fps = 60;
	//カウント進める
	count[i]++;
	//時間が来た
	if(count[i] >= second * fps)
	{
		count[i] = 0;
		return true;
	}
	return false;
}

void Enemy::MakeArea()	//索敵範囲の作成
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

bool Enemy::RotToGoal()	//ゴールへ旋回
{
	_vDir = RotateToTarget(_gPos, _vPos);
	float rot_y = atan2(_vDir.x, _vDir.z);
	Rot.y += DegToRad(1);
	RotateCnt++;
	//回転が180度になったら
	if(RotateCnt==180)
	{
		Rotate();
		RotateCnt = 0;
		return true;
	}
	return false;
}

bool Enemy::Dead()	//死亡
{
	if(type==EN_TYPE_DEAD)
	{
		//爆発エフェクトを死亡地点に生成
		EffectManager::GetEFInstance()->Add(EF_ENEMY_BOMB,_vPos);
		return true;
	}
	else
	{
		return false;
	}
}