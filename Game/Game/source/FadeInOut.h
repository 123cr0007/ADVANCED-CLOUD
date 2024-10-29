#pragma once
#include "appframe.h"
class Fade
{
public:
	Fade();
	~Fade();
	void Process();
	void Render();
	void ColorMask();	//カラーマスク
	bool FadeModeChange(int frame,int type);	//フェードの仕方を変える
	bool NotChangeFade(int frame);	//フェードしない
	bool BlackFadeIn(int frame);	//黒画面からフェードイン
	bool BlackFadeOut(int frame);	//黒画面からフェードアウト
	void ResetFadeValue();	//フェードの値をリセット
	bool IsFading();	//フェード中かどうか
	int GetAlpha() { return a; }	//透過度を取得
private:
	int r, g, b, a;	//3色＋透過度
	int strR, strG, strB, strA;	//フェード開始時のステータス
	int endR, endG, endB, endA;	//フェード終了時のステータス
	int fadeType;	//0:通常画面、1:フェードイン、２フェードアウト
	int fadeFrame;	//フェードにかかるフレーム数
	int count;	//フレームカウント
	int num;	//増減値
};
