#pragma once
#include "AppFrame.h"
#include "Bullet.h"
#include "ObjectBase.h"
#include "Player.h"

//#include <string>
//#include <vector>
//#include <math.h>
//#include <stdio.h>

//ステータス管理用
constexpr auto ENEMY_SHOT_RANGE = 1000;	//敵の正面方向の探知範囲
constexpr auto ENEMY_COUNT_MAX = 10;	//複数のタイマーを制御するため
constexpr auto ENEMY_SEARCH_AREA = 1000;//敵の探知用円錐底面の半径
constexpr auto ENEMY_SHOT_WAIT = 3;
//この下いじらない
constexpr auto EN_TYPE_DEAD = 10;//なし
constexpr auto EN_TYPE_MOVE = 11;//2点間移動

class Enemy :public ObjectBase 
{
public:

	static Enemy* ENInstance;
	static Enemy* GetENInstance() { return (Enemy*)ENInstance; }

	Enemy(int _id, VECTOR sPos, VECTOR gPos);
	~Enemy();
	bool Initialize();
	bool Process(VECTOR target_pos);
	bool Render();

	bool Move();//移動
	void PosSwap();//座標交換
	void Rotate();//回転
	void Rotate(VECTOR target_pos);//プレイヤーへ回転
	bool Search(VECTOR target_pos);//プレイヤー索敵
	bool SearchOut(VECTOR target_pos);//プレイヤーが範囲外へ逃亡
	void Shot(VECTOR target_pos);//プレイヤーへ攻撃
	void Reload();//弾のリロード
	bool Wait(int i,int second);//待ち時間
	void MakeArea();//判定エリアの作成
	bool RotToGoal();//ゴールへ向かう
	bool Dead();//死亡

	
	int GetUseEnrgy() { return UseEnergy; };//攻撃ヒットで消費させるエネルギーを取得
	VECTOR GetBlPos(int i) { return BLPos[i]; }//弾の座標を取得
	float GetBlR(int i) { return BlRadius[i]; }//弾の半径を取得
	int GetCheckActive(int i) { return CheckBlActive[i]; }//弾のアクティブ状態を取得

	void SetIsPatrol(bool Patrol) { IsPatrol = Patrol; }//パトロール中かどうかを設定
	void SetIsFind(bool Find) { IsFind = Find; }//プレイヤーを発見したかどうかを設定
	void SetIsVoice(bool Voice) { IsVoice = Voice; }//音を鳴らすかどうかを設定
	void SetIsShot(bool Shot) { IsShot = Shot; }//攻撃中かどうかを設定


	//音関連
	bool GetIsPatrol() { return IsPatrol; };//パトロール中かどうかを取得
	bool GetIsFind() { return IsFind; };//プレイヤーを発見したかどうかを取得
	bool GetIsVoise() { return IsVoice; };//音を鳴らすかどうかを取得
	bool GetIsShot() { return IsShot; };//攻撃中かどうかを取得


	void SetRestEnergy(int Energy) { RestEnergy = Energy; };//残りのエネルギーを設定
	void SetUseEnergy(int Energy) { UseEnergy = Energy; }; //使うエネルギーを設定
	void SetStartPos(VECTOR sPos) { _sPos = sPos; }//スタート座標を設定
	void SetGoalPos(VECTOR gPos) { _gPos = gPos; }//ゴール座標を設定
	
	int id;
	VECTOR _sPos;	//敵スタート座標
	VECTOR _gPos;	//敵ゴール座標
	VECTOR Rot;		//回転
	int RotateCnt = 0;
	VECTOR _aPos;	//判定エリアの底面
	VECTOR _tPos;	//判定エリアの頂点
	float r;		//判定エリアの半径
	int level;		//敵のレベル
	int spd;		//敵の移動速度（現状使えてない）
	int type;		//敵の動きを変更するためのもの
	bool shot = false;	//弾制御用

	int count[ENEMY_COUNT_MAX];	//待ち時間カウンター(何個も使いたい)

protected:
	//こっからアニメーション
	float _total_time;
	float _play_time;
	int _attach_index;

	//モデル関連
	int Tex_color[3];
	int En_handle;//敵本体モデル
	int _anim;
	//エネルギー関連
	int RestEnergy; //残りのエネルギー
	int UseEnergy;  //使うエネルギー
	float Check;

	//音関連
	bool IsPatrol;//パトロール中かどうか
	bool IsFind;//プレイヤーを発見したかどうか
	bool IsVoice;//音を鳴らすかどうか
	bool IsShot;//攻撃中かどうか
	bool IsSearchOut;//プレイヤーが範囲外へ逃亡したかどうか

	//他クラス関連
	VECTOR FlBlPos;//弾の座標
	float flBlR;//弾の半径

	VECTOR BLPos[BULLET_MAX];
	float BlRadius[BULLET_MAX];
	int CheckBlActive[BULLET_MAX];
	int mode;

private:

	BulletClass bullet;
};

enum class MODE:int
{
	search,
	attack,
	idle,
	rotate
};