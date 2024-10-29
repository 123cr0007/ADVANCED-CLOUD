//敵とお供が使用する弾の処理
#include "AppFrame.h"
#include "Bullet.h"
#include "ApplicationMain.h"

BulletClass::BulletClass()
{
	//初期化
	for (int i = 0; i < BULLET_MAX; i++)
	{
		bl[i].pos = VGet(0.0f, 0.0f, 0.0f);
		bl[i].rot = VGet(0.0f, 0.0f, 0.0f);
		bl[i].active = false;
	}
	handle = MV1LoadModel("res/Enemy/Bullet/Bullet_v01.MV1");
	handle = MV1DuplicateModel(handle);
	tex_en = LoadGraph("res/Enemy/Bullet/Bullet_Texture_v01.png");
	tex_fl = LoadGraph("res/Enemy/Bullet/Bullet_Texture_v01.png");
}

BulletClass::~BulletClass()
{

}

//弾追加
int BulletClass::AddBullet(VECTOR my_pos, VECTOR target_pos,int handle_num)
{
	//空いている配列を探す
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//空いている配列を見つけたら
		if (!bl[i].active)
		{
			//発射する親の情報を代入
			bl[i]._sPos = my_pos;//スタート座標
			bl[i].tPos = target_pos;//ターゲット座標

			//弾のテクスチャを変更（実装なし）
			switch(handle_num)
			{
			case 0:
				MV1SetTextureGraphHandle(handle,0,tex_en, TRUE);
				break;
			case 1:
				MV1SetTextureGraphHandle(handle, 0, tex_fl, TRUE);
				break;
			}
			//ターゲットを再指定したかった（偏差撃ちのため）
			bl[i].tPos.x + float(rand() % 100);
			bl[i].tPos.y + float(rand() % 100);
			bl[i].tPos.z + float(rand() % 100);
			bl[i].CollisionR = 30;
			bl[i].CheckHit = false;

			//弾を敵本体の座標へ
			bl[i].pos = bl[i]._sPos;

			//弾の使用をアクティブに
			bl[i].active = true;

			break;
		}
	}
	return 0;
}

//弾処理
void BulletClass::BulletProcess(int i)
{
	Move(i);//移動
	Rotate(i);//回転
	Destroy(i);//消滅
}

//弾描画
void BulletClass::BulletRender(int i)
{
	for (int i = 0; i < BULLET_MAX; i++)
	{
		//アクティブな弾だけ描画
		if (bl[i].active)
		{
			MV1SetPosition(handle, bl[i].pos);	//弾の座標
			MV1SetScale(handle, VGet(5,5,5));	//弾の大きさ
			MV1SetRotationXYZ(handle,bl[i].rot);	//弾の回転
			MV1DrawModel(handle);	//弾描画
		}
	}
}

void BulletClass::Move(int i)	//弾移動
{
	//向きベクトル生成
	VECTOR dir = VGet(0.f, 0.f, 0.f);
	//成分を取得
	VECTOR comp = VScale(VSub(bl[i]._sPos, bl[i].tPos), 20);
	//ベクトルの大きさを取得
	float mag = PosMagnitude(bl[i]._sPos, bl[i].tPos);
	dir.x = comp.x / mag;
	dir.y = comp.y / mag;
	dir.z = comp.z / mag;
	//移動
	bl[i].pos = VSub(bl[i].pos, dir);
}

void BulletClass::Destroy(int i)	//弾消滅
{
	//弾が一定距離離れたら消滅
	if(PosMagnitude(bl[i].tPos, bl[i].pos) >= 2000/*||MV1CollCheck_Sphere()*/)
	{
		bl[i].active = false;
	}
}

void BulletClass::Rotate(int i)	//発射方向回転
{
	VECTOR _vDir = VGet(0, 0, 0);
	_vDir = RotateToTarget(bl[i].tPos, bl[i]._sPos);	//ターゲット座標へ向くベクトルを作成

	bl[i].rot.x = -atan2(_vDir.y, hypot(_vDir.x, _vDir.z));
	bl[i].rot.y = atan2(_vDir.x, _vDir.z);
}
