#pragma once
#include "DxLib.h"


//マウスでの視点移動
void ViewMouseMove();

//2点間の大きさ計測
float PosMagnitude(VECTOR target_pos,VECTOR my_pos);//ターゲットに向く

VECTOR RotateToTarget(VECTOR target_pos,VECTOR my_pos);

//中身を入れ替える(ベクトル用)
void swapVECTOR(VECTOR pos1, VECTOR pos2);
//中身を入れ替える(int用)
void swapInt(int x, int y);

////degree角からradian角へ
//float	DegToRad(int x);
////radian角からdegree角へ
//float	RadToDeg(float x);

//小数から百分率へ
float DeciToPer(float x);
//百分率から小数へ
float PerToDeci(float x);
//Particle
VECTOR Deg3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed);