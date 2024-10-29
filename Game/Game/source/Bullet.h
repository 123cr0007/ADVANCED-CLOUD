#pragma once
#include "AppFrame.h"

constexpr auto BULLET_MAX = 20;
constexpr auto BULLET_TEX_ENEMY = 0;
constexpr auto BULLET_TEX_FELLOW = 1;
class BulletClass
{
public:
	BulletClass();
	~BulletClass();
	int AddBullet(VECTOR my_pos, VECTOR target_pos,int handle);//弾生成
	void BulletProcess(int i);//弾処理
	void BulletRender(int i);//弾描画

	void Move(int i);//弾移動
	void Destroy(int i);//弾消滅
	void Rotate(int i);//発射方向回転
	VECTOR GetPos(int i) { return bl[i].pos; };//弾の座標取得
	float GetR(int i) { return bl[i].CollisionR; };//弾の半径取得
	bool GetActive(int i) {return  bl[i].active; };//弾のアクティブ状態取得
	int GetCheckHit(int i) { return  bl[i].CheckHit; };//弾の当たり判定取得

	void SetCheckHit(int i, bool n) { bl[i].CheckHit = n; };//弾の当たり判定設定
	
private:
	struct Bullet
	{
		VECTOR tPos;//ターゲット座標
		VECTOR _sPos;//スタート座標
		VECTOR pos;//現座標	コリジョン付けるときはこれを基準にお願いします。
		VECTOR rot;//一応方向
		bool active;//弾の使用可否
		float CollisionR;//弾の半径（おおきさ）
		bool CheckHit;
	};
	struct Bullet bl[BULLET_MAX];
	int handle;
	int tex_en;
	int tex_fl;
};