//フェードイン・フェードアウト処理（未完成）
#include"FadeInOut.h"
Fade::Fade()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
	fadeType = 0;
	fadeFrame = 0;
	count = 0;
	num = 0;
}

Fade::~Fade()
{

}

void Fade::Process()
{
	switch(fadeType)
	{

	case 1:
		/*strR += num;
		strG += num;
		strB += num;
		strA += num;*/
		a += num;
		break;
	case 2:
		/*strR -= num;
		strG -= num;
		strB -= num;
		strA -= num;*/
		a -= num;
		break;
	}
	if(IsFading())
	{
		++count;
	}
}

void Fade::Render()
{
	
}

void Fade::ColorMask()
{

}

bool Fade::FadeModeChange(int frame,int type)
{
	//フェードのタイプで処理を分ける
	switch(type)
	{
		//フェードしない
	case 0:
		NotChangeFade(frame);
		break;

		//フェードイン
	case 1:
		BlackFadeIn(frame);
		break;

		//フェードアウト
	case 2:
		BlackFadeOut(frame);
		break;
	}
	return false;
}

bool Fade::NotChangeFade(int frame)
{
	//前がフェードインだった
	if(fadeType == 1)
	{
		a = 255;//フェードイン完了時の透明度
		fadeType = 0;
		fadeFrame = 0;
		count = 0;
	}
	//前がフェードアウトだった
	if(fadeType == 2)
	{
		a = 0;//フェードアウト完了時の透明度
		fadeType = 0;
		fadeFrame = 0;
		count = 0;
	}
	return false;
}

bool Fade::BlackFadeIn(int frame)
{
	//スタート時のステータス設定
	strR = 0;
	strG = 0;
	strB = 0;
	strA = 255;
	//フェードイン完了時
	endR = 255;
	endG = 255;
	endB = 255;
	endA = 0;
	//カウント初期化
	fadeFrame = frame;
	num = 255 / frame;
	count = 0;
	fadeType = 1;
	return true;
}	

bool Fade::BlackFadeOut(int frame)
{
	//スタート時のステータス設定
	strR = 255;
	strG = 255;
	strB = 255;
	strA = 0;
	//フェードアウト完了時
	endR = 0;
	endG = 0;
	endB = 0;
	endA = 255;
	//カウント初期化
	fadeFrame = frame;
	num = 255 / frame;
	count = 0;
	fadeType = 2;
	return true;
}

void Fade::ResetFadeValue()	//フェード値リセット
{
	a = 0;
	fadeType = 0;
	fadeFrame = 0;
	count = 0;
}

bool Fade::IsFading()
{
	//フェード中
	if(count < fadeFrame) {
		return true;
	}
	//フェードしてない
	else
	{
		return false;
	}
}