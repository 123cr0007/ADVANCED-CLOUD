#include "Utility.h"
#include "..//Mymath/mymath.h"
#include <math.h>
//マウスでの視点移動
void ViewMouseMove()
{

}

//2点間の大きさ計測
float PosMagnitude(VECTOR target_pos, VECTOR my_pos)
{
	VECTOR v= VSub(target_pos, my_pos);
	float magnitude = sqrt(VSquareSize(v));
	return magnitude;
}

//ターゲットに向く
VECTOR RotateToTarget(VECTOR target_pos,VECTOR my_pos)
{
	//成分を求める
	//ベクトルの正規化
	if(my_pos.x != target_pos.x || my_pos.y != target_pos.y || my_pos.z != target_pos.z)
	{
		return VNorm(VSub(my_pos, target_pos));
	}
	else 
	{ 
		return VGet(0, 0, 0); 
	}
}

//中身を入れ替える(VECTOR用)
void swapVECTOR(VECTOR pos1,VECTOR pos2)
{
	VECTOR tmp = pos1;
	pos1 = pos2;
	pos2 = tmp;
}

//中身を入れ替える(int用)
void swapInt(int x, int y)
{
	int tmp = x;
	x = y;
	y = tmp;
}

//小数を百分率へ
float DeciToPer(float i)
{
	float per = i * 100;
	return per;
}

//百分率を小数へ
float PerToDeci(float i)
{
	float deci = i / 100;
	return deci;
}

//Particle
VECTOR Deg3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed)
{

	float rad_xy = DegToRad(deg_xy);
	float rad_zx = DegToRad(deg_zx);

	//現在の角度でベクトルを作る
	VECTOR vDir = VGet(0, 0, 0);


	VECTOR willpos = VScale(position, speed);

	float r = VSize(VSub(willpos, position));

	vDir.x = r * sin(rad_xy) * cos(rad_zx);
	vDir.y = r * cos(rad_xy);
	vDir.z = r * sin(rad_xy) * sin(rad_zx);

	// 作ったベクトルを「単位ベクトル」（スカラが1のベクトル）にする
	VECTOR vUnit = VNorm(vDir);

	// 単位ベクトルに移動速度をかけた、移動ベクトルを作る
	VECTOR vMove = VScale(vUnit, speed);
	//移動ベクトルを位置に加算する
	position = VAdd(position, vMove);

	return	position;
}