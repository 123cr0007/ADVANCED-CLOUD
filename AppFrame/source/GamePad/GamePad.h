#pragma once	// .hの先頭に記述。#includeでこのファイルを何度読み込みしても、1度しか読み込まない
#include "Dxlib.h"

constexpr int MULTIPAD_MAX = 10;	// 最大で、パッド9個＋キーボードの10個

constexpr int PLAYER_0 = 0; //キーボード
constexpr int PLAYER_1 = 1;	//プレイヤー１

class GamePad {

public:

	GamePad();			// コンストラクタ
	virtual ~GamePad();	// デストラクタ

	void Input();		// 入力更新

	//実体
	static GamePad* Instance;
	static GamePad* GetInstance() { return Instance; }

	// 複数ゲームパッド用の場合
	struct MULTIPAD {
		int key;			// キー入力の情報
		int trg;			// トリガ入力の情報
		int rel;			// リリース入力の情報
		float LX;		//左スティックのX軸情報
		float LY;		//左スティックのY軸情報
		float RX;		//右スティックのX軸情報
		float RY;		//右スティックのY軸情報
	};

	MULTIPAD GetMultiPad(int pl_num) {return pad[pl_num];};
	int GetKey(int pl_num) { return pad[pl_num].key; }
	int GetTrg(int pl_num) { return pad[pl_num].trg; }
	int GetLX(int pl_num) { return pad[pl_num].LX; }
	int GetLY(int pl_num) { return pad[pl_num].LY; }
	int GetRX(int pl_num) { return pad[pl_num].RX; }
	int GetRY(int pl_num) { return pad[pl_num].RY; }
	int GetGamePadNum() { return GamePadNum; };

protected:

	// キーボード＆ゲームパッド1のどちらでも使える場合
	int key;			// キー入力の情報
	int trg;			// トリガ入力の情報
	int rel;			// リリース入力の情報

	//ゲームパッドの接続数管理用
	int GamePadNum;

	MULTIPAD	pad[MULTIPAD_MAX];


	//Xbox360コントローラーの生データ
	XINPUT_STATE xinput;

	//ダイレクトインプットコントローラーの生データ
	DINPUT_JOYSTATE dinput;
};

