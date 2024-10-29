#pragma once
#include "ObjectBase.h"
#include "appframe.h"

#include <string>
#include <vector>

const auto JUMP_CT = 30;

const auto JUMP_TYPE_NONE = -1;     //ジャンプしてない
const auto JUMP_TYPE_NORMAL = 1;	//通常ジャンプ
const auto JUMP_TYPE_FLOAT =  2;	//ふわジャン
const auto JUMP_TYPE_AVOID = 3;	    //回避ジャンプ
const auto JUMP_TYPE_ATK = 4;       //攻撃ジャンプ

const auto CHANGE_GRV_NUM  = 0.25f; //下げる重力値
const auto JUMP_POWER = 10.0f;         //ジャンプ力
const auto GRVADD_NUM = 12.0f;      //ふわジャンの追加するジャンプ力
const auto PL_MOVE_SPEED = 10.0f;   //移動力

const auto CUPSLE_DPOS = 50.f;      //カプセルの下の高さ（プレイヤーの足元からあげる位置）
const auto CUPSLE_UPOS = 150.0f;    //カプセルの上の高さ（プレイヤーの足元からあげる位置）

const auto FRAME_TO_MOVE = 15;      //回避ジャンプの時プレイヤーが何フレームで移動するか
const auto MOVIVG_HEIGHT = 200;     //回避ジャンプの時プレイヤーがどれだけ高く移動するか
const auto MOVIVG_FRONT = 100;      //回避ジャンプの時プレイヤーがどれだけ前に移動するか

const auto CIRCLE_CORNAR = 64;      //足元に出す丸影の辺の数

const auto IDLE_DANCE_TIME = 20 * 60;  //アイドルの時どのくらいの時間で特殊モーション再生するか
const auto ANIM_BREND_TIME = 30;    //何フレームでブレンド完了するか

class Player : public ObjectBase {
public:
    Player();
    virtual ~Player();

    static Player* PLInstance;
    static Player* GetPLInstance() { return (Player*)PLInstance; }

    bool Initialize();  //初期化
    bool Process() override;//処理
    bool Render(int type) override;//描画
   
    bool ProcessForTitle(); //タイトル用の処理
    bool RenderForTitle();  //タイトル用の描画

    int GetJumpType(){ return jumptype; }       //ジャンプの種類のゲッター
    int GetReleaseJump(){ return release_jump; }//ジャンプのリリースのゲッター
    int GetJumpTime(){ return jumpTime; }       //ジャンプの時間のゲッター
    int GetJumpCast() { return JumpCast; }   //ジャンプのキャストタイムのゲッター    
    int GetJustTime() { return justTimeMax; } //ジャストの時間のゲッター
    int GetCheckAtack() { return CheckAtack; }; //攻撃しているかのゲッター
    int GetUseEnergy() { return plUseEnergy; }; //エネルギーを使うかのゲッター
    int GetChargeEnergy() { return chargeEnergy; }; //エネルギーを回復するかのゲッター
    int GetHealEnergy() { return checkHealEnergy; }; //エネルギーを回復するかのゲッター
    bool GetChangeMap() { return changeMap; }; //マップを変えるかのゲッター
    bool GetGameOver() { return gameOver; } ; //ゲームオーバーかのゲッター
    bool GetGameClear() { return gameClear; }   ; //ゲームクリアかのゲッター
    bool GetPlayerDeath() { return dead; }  ; //プレイヤーが死んでいるかのゲッター
    VECTOR GetCapsuleTop() {return CapsuleTop; }    //カプセルの上の位置のゲッター
    VECTOR GetCapsuleRow() { return CapsuleRow; }   //カプセルの下の位置のゲッター

    void SetRestEnergy(int Energy) { restEnergy = Energy; };//エネルギーの残りのセッター
    void SetUseEnergy(int Energy) { plUseEnergy = Energy; };  //エネルギーを使うかのセッター
    void SetChargeEnergy(int Energy) { chargeEnergy = Energy; };//エネルギーを回復するかのセッター
    void SetCheckAtack(int atk) { CheckAtack = atk; };
    void SetChangeMap(bool change) { changeMap = change; };
    void SetIsOperate(bool Operate) { isOperate = Operate; };
    void SetIsStealth(bool Stealth) { isStealth = Stealth; };
    void SetIsComShot(bool iscomshot) { isComShot = iscomshot; };
    void SetCherOpacity(float Opacity) { cherOpacity = Opacity; };

    //してるかどうか
    bool IsJump() { return isJump; };
    bool IsLand() { return isLand; };
    bool IsJust() { return isJust; };
    bool IsWalk() { return isWalk; };
    bool IsIdle() { return isIdle; };
    bool IsComShot() { return isComShot; };
    bool IsDamage() { return isDamage; };

    void SetIsJump(bool isjump) { isJump = isjump; };
    void SetIsLand(bool island) { isLand = island; };
    void SetIsJust(bool isjust) { isJust = isjust; };
    void SetIsWalk(bool iswalk) { isWalk = iswalk; };
    void SetIsIdle(bool isidel) { isIdle = isidel; };
    void SetIsDamage(bool isdamage) { isDamage = isdamage; };
    
 
private:


    //プレイヤー操作部分
    bool ProcessOperate();
    //移動部分
    bool ProcessMove();

    //回避ジャンプの消えるのと出現
    bool Vanish(VECTOR GoalPos, VECTOR StartPos);
    bool arrival();
    
    //
    bool ProcessGrv();      //重力処理
    bool ProcessCapsule();  //カプセルの当たり判定処理
    bool ProcessRand();     //地面との当たり判定処理

    //アニメーション
    bool Animetion();
    
    //影
    bool MakeShadow();

    //移動値等セット用
    int grvNum;
    int muveNum;

    //移動量保存用
    VECTOR MoveV;
    VECTOR oldMoveV;

    //キャラの透明度
    float   cherOpacity;

    // コリジョン判定時のY位置(腰位置）
    float   _colSubY;

    //影関連
    VECTOR ShadowPos;       //影を出す位置
    VECTOR ShadowCircle[CIRCLE_CORNAR]; 
    bool CheckMakeShadow;   //影を出すかの判定

    //重力関連
    float	gravity;        //現在の重力値
    bool	CheckGravity;   //重力を使うかどうか
    bool    OldCheckGrv;    //処理する前の重力を使うかどうか
    int		CheckJump;      //じゃんぷをしているか
    int		Srow;           //重力を軽減するか
    int     CheckAtack;     //攻撃してるか
    int     FloatingTime;   //空中にいる時間

    //カメラ関連
    VECTOR camPos;
    VECTOR camTgtPos;


	//ジャンプ用変数
    int     jumptype;       //使用中のジャンプの種類
    int     JumpCast;       //ジャンプのキャストタイム
    int     jumpTime;       //UI用
    int     justTimeMax;    //同じく


    //Effekseer用ハンドル
    int     ef_handle;      //ロード用ハンドル
    int     play_Test;      //再生用ハンドル

   
    //避けるやつ
    int     CheckAvoid;     //回避するときのフェーズ分け
    int     AvoidTime;      //何フレーム回避したか
    VECTOR  PlSize;         //プレイヤーのサイズ
    VECTOR  OldPos;         //座標指定するためのやつ入れとくとこ

    
    //エネルギー関連
    int     restEnergy;     //残りのエネルギー
    int     plUseEnergy;    //使うエネルギー
    int     chargeEnergy;   //エネルギー回復量
    int     maxEnergy;      //今の最大エネルギー
    int     checkHealEnergy;//エネルギーを回復するか

	//状況確認用
    //サウンドで使うよう
    bool    isJump;         
    bool    isLand;
    bool    isJust;
    bool    isWalk;
    bool    isDamage;
    bool    isFloat;
    bool    isIdle;
    bool    isComShot;
    bool    isStealth;
    bool    isHitWall;

    
    //アニメーション関連
    int     attachIndex;    //アタッチ用インデックス
    float   totalTime;      //アニメーションの合計時間
    float   playTime;       //アニメーションの再生時間
    // モーションブレンド用
    float   animBrendRate;     //ブレンド率
    int     animBrendIndex;    //ブレンド用インデックス
    float   animBrendPlayTime; //ブレンド再生時間
    float   animBrendTotalTime;//ブレンド合計時間
    int     idleTime;		   //アイドルの時間

    //影用ハンドル
    int     shadowHandle;

    //アニメーション切り替え用ステータス
     enum class STATUS {
        NONE,
        WAIT,       //1  
        FALL_R,       //2
        FALL_L,       //3
        
        RUN,        //4
        RUN_START,  //5
        RUN_END,    //6

        JUMP_START_R, //7
        JUMP_END_R,   //8
        JUMP_START_L, //7
        JUMP_END_L,   //8

        DOWN,
        IDLE_2,
        _EOT_       //
    };
    STATUS  _status;
    STATUS  oldStatus;
    STATUS  stateBrend;

    bool    ModePhoto;

    //ステージ進行用
    bool        changeMap;  //ステージ進行するか
    bool        stageTrg;   //ステージ側のトリガ保存用
    std::string mapName;    //マップの名前
    const char* String;     //名前描画用
    bool        isOperate;  //ステージ進行の際プレイヤーに操作させないため
    bool        gameOver;   //ゲームオーバートリガ
    bool        dead;       //プレイヤーが死んでるかのトリガ
    bool        gameClear;  //ゲームクリアのトリガ
    int         stepClassroomNum;   //教室を踏んだか

    //デバッグ用
    int check = 0;
    int CheckNum;
    VECTOR CP;
    unsigned int Cr = GetColor(0, 0, 0);

    VECTOR StPos;
    VECTOR StLen;
};
