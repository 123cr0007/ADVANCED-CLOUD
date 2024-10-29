#include "Dxlib.h"
#include "GamePad.h"

GamePad *GamePad::Instance = NULL;

GamePad::GamePad() {

	Instance = this;
	key = 0;
	trg = 0;
	rel = 0;

	// 複数ゲームパッド用
	for(int i = 0; i < MULTIPAD_MAX; i++) {
		pad[i].key = 0;
		pad[i].trg = 0;
		pad[i].rel = 0;
		pad[i].LX = 0;
		pad[i].LY = 0;
		pad[i].RX = 0;
		pad[i].RY = 0;
	}

	//接続されていたコントローラー数を保管
	GamePadNum = GetJoypadNum();

	xinput = { 0 };

	dinput = { 0 };

}

GamePad::~GamePad() {
	
}

void GamePad::Input() {
	//// キー入力、トリガ入力、リリース入力を得る
	//int keyold = key;
	//
	//key =  GetJoypadInputState(DX_INPUT_PAD1);		// キー入力を取得
	//trg = (key ^ keyold) & key;
	//rel = (key ^ keyold) & ~key;

	// 複数ゲームパッド用
	int padTbl[] = {
		DX_INPUT_KEY,	// 0:キー入力
		DX_INPUT_PAD1,	// 1:パッド1
		DX_INPUT_PAD2,	// 2:パッド2
		DX_INPUT_PAD3,	// 3:パッド3
		DX_INPUT_PAD4,	// 4:パッド4
		DX_INPUT_PAD5,	// 5:パッド5
		DX_INPUT_PAD6,	// 6:パッド6
		DX_INPUT_PAD7,	// 7:パッド7
		DX_INPUT_PAD8,	// 8:パッド8
		DX_INPUT_PAD9,	// 9:パッド9

	};

	//ゲームパッドが接続されているなら
	if (GamePadNum > 0 ) {

		//処理

		//ゲームパッドの接続数が上限未満なら
		if(GamePadNum < MULTIPAD_MAX){

			//処理

			for (int i = 0; i < MULTIPAD_MAX; i++)
			{
				if (CheckJoypadXInput(padTbl[i])) {

					int keyold = pad[i].key;
					pad[i].key = GetJoypadInputState(padTbl[i]);
					pad[i].trg = (pad[i].key ^ keyold) & pad[i].key;
					pad[i].rel = (pad[i].key ^ keyold) & ~pad[i].key;

					GetJoypadXInputState(padTbl[i], &xinput);
					pad[i].LX = xinput.ThumbLX;
					pad[i].LY = -xinput.ThumbLY;
					pad[i].RX = xinput.ThumbRX;
					pad[i].RY = -xinput.ThumbRY;

				}
				else
				{
					int keyold = pad[i].key;
					pad[i].key = GetJoypadInputState(padTbl[i]);
					GetJoypadDirectInputState(padTbl[i], &dinput);
					pad[i].trg = (pad[i].key ^ keyold) & pad[i].key;
					pad[i].rel = (pad[i].key ^ keyold) & ~pad[i].key;
					pad[i].LX = dinput.X;
					pad[i].LY = dinput.Y;

				}

			}
		}
	}
}


