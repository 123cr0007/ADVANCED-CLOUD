#pragma once
#include "Dxlib.h"
#include <math.h>
#include<stdio.h>
#include<time.h>

constexpr float PI = 3.141592653589793;

namespace calculation
{
	
	float RadToDeg(float rad);

	float DegToRad(float deg);

	/*
	角度からラジアンに変換して単位ベクトルを求める
	またスケールも決めれる
	単位ベクトルのまま使う場合scaleには0.0fを入れる
	*/
	VECTOR RadUnitVector(float deg, float scale);
	//ラジアンで移動位置を求める
	VECTOR RadPositionVector(VECTOR position,float deg, float speed);
	//3D
	VECTOR Rad3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed);

	//デバック用



}; 