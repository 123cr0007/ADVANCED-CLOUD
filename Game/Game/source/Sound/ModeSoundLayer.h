#pragma once

#include "appframe.h"
#include "SoundItem.h"
#include "LoadSound.h"
#include <string>


// モード
class ModeSoundLayer : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

	void SetMenuEnable(bool enable) { _enableMenu = enable; }
	bool GetMenuEnable() { return _enableMenu; }

private:
	void PrintSoundItem(int x, int y, std::string name, SoundItemBase* snd);

protected:
	bool	_enableMenu;


	int		_loadTimeMs;		// ロード登録にかかった時間
	int		_asyncLoadBaseTimeMs;	// 非同期ロードにかかった時間(計算用)
	int		_asyncLoadTimeMs;		// 非同期ロードにかかった時間

	std::string		_msg;			// 処理メッセージ表示用

	int		_listCur;			// 選択リストカーソル位置
	int		_listViewStart, _listViewNum;	// 選択リスト表示関連
	SoundJson* _lodsound;
};