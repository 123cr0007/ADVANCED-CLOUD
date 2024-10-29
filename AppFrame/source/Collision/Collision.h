#pragma once
#include "DxLib.h"
#include <string>
#include <vector>
#include "stdlib.h"

//角123が鈍角か調べる
float CheckAngle(VECTOR p1, VECTOR p2, VECTOR p3);

// 平行関係か調べる
// 0だったら平行
bool isParallel(VECTOR p1, VECTOR p2);

void clamp01(float& v);

//点と直線の最短距離
float CheckDistPointLine(VECTOR p1, VECTOR p2a, VECTOR p2b, VECTOR* h, float* t);

//点と線分の最短距離
float CheckDistPointSegment(VECTOR p1, VECTOR p2a, VECTOR p2b, VECTOR* h, float* t);

//直線の直線の最短距離
float CheckDistLineLine(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, VECTOR* h1, VECTOR* h2, float* t1, float* t2);

//線分同士の最短距離
float CheckDistSegment(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, VECTOR* h1, VECTOR* h2, float* t1, float* t2);

//カプセル同士が当たってるかどうか
bool CheckHitCapsule(VECTOR p1a, VECTOR p1b, VECTOR p2a, VECTOR p2b, float t1, float t2);

//点とカプセル
bool CheckHitPointCapsule(VECTOR p1a, VECTOR p1b, VECTOR p2, float t1, float t2);

//カプセル同士の押し戻し処理
VECTOR CheckCapsule(VECTOR p1a, VECTOR p1b, float r1, VECTOR p2a, VECTOR p2b, float r2);

//壁擦りベクトルの作成
VECTOR MakeWallLap(MV1_COLL_RESULT_POLY HitNorm, VECTOR MoveV);

//abs関数を使ってベクトルの絶対値を求める
float VectorAbs(VECTOR v);

//点と円錐の当たり判定
bool CheckHitPointCorn(VECTOR top,VECTOR bottom,float r,VECTOR p);