#pragma once
#include"AppFrame.h"

//ステータス管理用
constexpr auto EF_SCALE = 10;

//この下触らない
constexpr auto EF_JUMP_FLOAT = 100;		//浮遊ジャンプ
constexpr auto EF_JUMP_NORMAL = 102;	//通常ジャンプ
constexpr auto EF_JUMP_AVOID = 103;		//回避ジャンプ
constexpr auto EF_JUMP_ATTACK = 104;	//攻撃ジャンプ
constexpr auto EF_ENEMY_BOMB = 105;		//爆発
constexpr auto EF_CRYSTAL = 106;		//柱と、らせん状に渦巻く光（緑）
constexpr auto EF_DIAMOND = 107;		//大きなダイヤ型の光が消えていく（黄）
constexpr auto EF_RECOVERY = 108;		//縦向きのリングと放散する光（緑）
constexpr auto EF_DIFFUSE = 109;		//放散する光（緑）	回避の1
constexpr auto EF_TORNADO_MOVE = 110;	//らせんを描く光（緑）	回避の２　
constexpr auto EF_TORNADO = 111;		//らせんを描く光（緑）	プレイヤー追従なし
constexpr auto EF_GATHER = 112;			//光が集まる（緑）	回避の３

constexpr auto EFFECT_PLAYER = 0;	//プレイヤー用エフェクト識別用
constexpr auto EFFECT_ENEMY = 1;	//エネミー用エフェクト識別用
constexpr auto EFFECT_OTHER = 2;	//その他オブジェクト用エフェクト識別用

class Effect
{
public:
 	Effect(int Effect, VECTOR pos);
	~Effect();
	void Process(VECTOR pl_pos,VECTOR en_pos, VECTOR item_pos);
	void Render();

public:
	bool isPlay;//エフェクトを再生するか

protected:
	int handle;
	int play_handle;	//再生エフェクト指定ハンドル
	VECTOR efPos;		//エフェクト表示座標
	bool isMove;		//エフェクトは追従するか
	int ef_controller;	//エフェクトの所有者
};