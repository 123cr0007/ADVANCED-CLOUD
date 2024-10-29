#pragma once

#include "ObjectBase.h"
#include "appframe.h"
#include "DxLib.h"
#include "EnergyGauge.h"
#include "Player.h"

#include "Bullet.h"

#include <string>
#include <vector>

//プレイヤーの足元からどれだけ上げるか
const auto POS_UP = 130;
//どれだけプレイヤーと離すか
const auto DISTANCE = 100;

//class Enemy;
class Fellow : public ObjectBase {
public:

  static Fellow* FLInstance;
  static Fellow* GetFLInstance() { return (Fellow*)FLInstance; }

    Fellow();
    virtual ~Fellow();

    bool Initialize(VECTOR PlPos, VECTOR PlDir);
    bool Process(VECTOR PlPos, VECTOR PlDir, VECTOR MoveValue);
    bool Render(int type) override;

    void PhaseAttack(VECTOR PlPos, VECTOR PlDir);
    void PhaseMove(VECTOR PlPos, VECTOR PlDir);

    VECTOR GetBlPos(int i) { return BLPos[i]; }
    float GetBlR(int i){ return BlRadius[i]; }
    int GetCheckActive(int i) { return CheckBlActive[i]; }
private:

    //ターゲットのやつ
    int RockonHandle;

    //アニメーション関連
    int     _anim1;
    int     _anim2;
    int     _attach_index;
    float   _total_time;
    float   _play_time;

    //プレイヤーの位置
    VECTOR  CenterPos;

    //止まってる時のお供とプレイヤーの距離
    VECTOR  NormalLengh;
    float   NormalRad;

    //追っかける処理用
    int     time;
    int     CheckMove;
    VECTOR  MoveV;

    //弾発射関連
    int FireRate;
    int FiredNum;
    VECTOR Epos;  //敵ポジション
   

    //お供から発射された玉の情報
    VECTOR BLPos[BULLET_MAX];
    float BlRadius[BULLET_MAX];
    bool CheckBlActive[BULLET_MAX];

    //ターゲットのハンドルを使うかどうか
    bool useHandle;

    //デバッグ用
    VECTOR CheckV;
    VECTOR CheckV2;

    //他クラス関連
    BulletClass bullet;
};