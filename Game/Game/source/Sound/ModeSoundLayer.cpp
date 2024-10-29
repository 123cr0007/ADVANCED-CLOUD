#include "ModeSoundLayer.h"
#include "..//ApplicationGlobal.h"

bool ModeSoundLayer::Initialize() {
	if (!base::Initialize()) { return false; }

	_lodsound = new SoundJson();


	// 非同期読み込み設定
	SetUseASyncLoadFlag(TRUE);

	int timems = GetNowCount();
	_asyncLoadBaseTimeMs = timems;	// 非同期ロード開始時間
	_asyncLoadTimeMs = 0;

	// 音データファイルを読み込む
	_lodsound->Process();
	
	// 読み込み時間
	_loadTimeMs = GetNowCount() - timems;

	// 同期読み込み設定
	SetUseASyncLoadFlag(FALSE);

	// 選択リスト用
	_listCur = 0;
	_listViewStart = 0;
	_listViewNum = 20;


#ifdef _DEBUG // メニュー処理＆表示の有効無効

	// メニュー処理＆表示の有効
	_enableMenu = true;

#else

	// メニュー処理＆表示の無効
	_enableMenu = false;

#endif // _DEBUG

	return true;
}

bool ModeSoundLayer::Terminate() {
	base::Terminate();

	delete _lodsound;//ロードjsonを削除

	return true;
}

bool ModeSoundLayer::Process() {
	base::Process();

	int key = GamePad::GetInstance()->GetKey(0);
	int trg = GamePad::GetInstance()->GetTrg(0);

	// サウンドサーバ更新
	gGlobal._sndServer.Update();

	// 非同期ロードは終わったか？
	if (GetASyncLoadNum() == 0) {
		if (_asyncLoadTimeMs == 0) {
			// 非同期ロードにかかった時間
			_asyncLoadTimeMs = GetNowCount() - _asyncLoadBaseTimeMs;
		}
	}

	// メニュー処理は有効か？
	if (!_enableMenu) {
		return true;	// 無効。
	}

	// このモードより下のレイヤーはProcess()を呼ばない
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	// ESCキーで閉じる
	bool close = false;
	if (trg & PAD_INPUT_9) {
		close = true;
	}


	// キー[A][S]を押して居ないか
	if ((key & (PAD_INPUT_4 | PAD_INPUT_5)) == 0) {
		// カーソル選択
		int itemNum = gGlobal._sndServer.GetVector().size();	// 項目数
		if (trg & PAD_INPUT_UP) { _listCur--; }
		if (trg & PAD_INPUT_DOWN) { _listCur++; }
		if (trg & PAD_INPUT_LEFT) {
			_listCur -= _listViewNum;
			if (_listCur < 0) { _listCur = 0; }
		}
		if (trg & PAD_INPUT_RIGHT) {
			_listCur += _listViewNum;
			if (_listCur >= itemNum) { _listCur = itemNum - 1; }
		}
		_listCur = (_listCur + itemNum) % itemNum;		// 項目数でカーソルループ
		// カーソル位置は表示内に入っているか？
		while (true) {
			if (_listCur < _listViewStart) { _listViewStart--; continue; }
			if (_listCur >= _listViewStart + _listViewNum) { _listViewStart++; continue; }
			break;
		}
	}

	// カーソル位置のデータを取得
	SoundItemBase* sndItem = NULL;
	std::string sndItemName = "";
	int n = 0;
	for (auto&& e : gGlobal._sndServer.GetVector()) {
		if (n == _listCur) {
			sndItem = e.second;
			sndItemName = e.first;
		}
		n++;
	}



	// キー[A][S]を押して居ないか
	if ((key & (PAD_INPUT_4 | PAD_INPUT_5)) == 0) {
		// キー入力[Z]でカーソル位置のデータ再生
		if (trg & PAD_INPUT_1) {
			if (sndItem && sndItem->IsLoad()) {
				sndItem->Play();
				_msg = sndItemName + " 再生";
			}
			else {
				_msg = sndItemName + " がまだロードされていない";
			}
		}
		// キー入力[X]でカーソル位置のデータ停止
		if (trg & PAD_INPUT_2) {
			if (sndItem && sndItem->IsPlay()) {
				sndItem->Stop();
				_msg = sndItemName + " 停止";
			}
		}
		// キー入力[C]でカーソル位置のデータをOneShot再生
		if (trg & PAD_INPUT_3) {
			if (sndItem) {
				// ファイル名を取り出す
				std::string filename = sndItem->GetFileName();
				// OneShot再生としてサーバに登録し、再生
				SoundItemOneShot* snd = new SoundItemOneShot(filename);
				// OneShot再生でvolumeやpanを変えるサンプル
				snd->SetVolume(255);
				snd->SetPan(-255);
				gGlobal._sndServer.Add(snd,255,255);		// サーバに登録した瞬間に再生される
			}
		}
	}
	else if (key & PAD_INPUT_4) {
		// キー入力[A]を押しながら上下左右でVolume/Panを変更
		// キー[X]で、Volume/Panリセット
		if (sndItem) {
			int vol = sndItem->GetVolume();
			int pan = sndItem->GetPan();
			if (trg & PAD_INPUT_2) {
				vol = 160;
				pan = 0;
			}
			// volume
			if (key & PAD_INPUT_UP) { vol++; }
			if (key & PAD_INPUT_DOWN) { vol--; }
			if (vol < 0) { vol = 0; }
			if (vol > 255) { vol = 255; }
			sndItem->SetVolume(vol);
			// pan
			if (key & PAD_INPUT_RIGHT) { pan++; }
			if (key & PAD_INPUT_LEFT) { pan--; }
			if (pan < -255) { pan = -255; }
			if (pan > 255) { pan = 255; }
			sndItem->SetPan(pan);
		}
	}
	else if (key & PAD_INPUT_5) {
		// キー入力[S]を押しながら上下左右でFrequencyを変更
		// キー入力[X]で、Frequencyリセット
		if (sndItem) {
			int freq = sndItem->GetFrequency();
			if (freq > 0) {		// 0の場合、まだ設定できない
				if (trg & PAD_INPUT_2) {
					sndItem->ResetFrequency();
					freq = sndItem->GetFrequency();
				}
				int freqstep = 10;
				if (key & PAD_INPUT_UP) { freq += freqstep; }
				if (key & PAD_INPUT_DOWN) { freq -= freqstep; }
				if (key & PAD_INPUT_RIGHT) { freq += freqstep * 10; }
				if (key & PAD_INPUT_LEFT) { freq -= freqstep * 10; }
				if (freq < 1000) { freq = 1000; }
				if (freq > 100000) { freq = 100000; }
				sndItem->SetFrequency(freq);
			}
		}
	}

	// メニュー終了
	if (close) {
		_enableMenu = false;
	}

	return true;
}


void ModeSoundLayer::PrintSoundItem(int x, int y, std::string name, SoundItemBase* snd) {
	int isLoad = 0;
	int isPlay = 0;
	int isOnMemory = 0;
	if (snd->IsLoad()) { isLoad = 1; }
	if (snd->IsPlay()) { isPlay = 1; }
	if (snd->GetSoundHandle() != -1) { isOnMemory = 1; }
	char strIsLoaded[][128] = { "(非同期ロード中)", "" };
	char strIsPlay[][128] = { "", "(再生中)" };
	char strIsOnMemory[][128] = { "", "(オンメモリ)" };
	DrawFormatString(32, y, GetColor(255, 255, 255),
		"%s  : vol=%d, pan=%d, freq=%d,  %s  %s%s%s"
		, name.c_str()
		, snd->GetVolume()
		, snd->GetPan()
		, snd->GetFrequency()
		, snd->GetFileName().c_str()
		, strIsLoaded[isLoad]
		, strIsPlay[isPlay]
		, strIsOnMemory[isOnMemory]
	);
}

bool ModeSoundLayer::Render() {
	base::Render();

	// メニュー処理は有効か？
	if (!_enableMenu) {
		return true;	// 無効。
	}

	// 下地
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawFillBox(0, 0, 1280, 720, GetColor(0, 0, 0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	int y = 0, size = 16;
	DrawFormatString(0, y, GetColor(255, 0, 0), "ロード時間(ms): %d", _loadTimeMs); y += size;
	DrawFormatString(0, y, GetColor(255, 0, 0), "非同期読み込み中ファイル数: %d", GetASyncLoadNum()); y += size;
	DrawFormatString(0, y, GetColor(255, 0, 0), "非同期ロード時間(ms): %d", _asyncLoadTimeMs); y += size;
	DrawFormatString(0, y, GetColor(255, 255, 255), "msg: %s", _msg.c_str()); y += size;

	// 選択リスト＆カーソル表示
	y = 100;
	int n = 0;
	for (auto&& e : gGlobal._sndServer.GetVector()) {
		if (n >= _listViewStart && n < _listViewStart + _listViewNum) {
			if (n == _listCur) {
				DrawFormatString(8, y, GetColor(255, 255, 255), "→");
			}
			PrintSoundItem(32, y, e.first, e.second);
			y += size;
		}
		n++;
	}

	// OneShotリスト表示
	y += size;
	DrawFormatString(0, y, GetColor(255, 0, 0), "--- ONESHOT ---", _loadTimeMs); y += size;
	for (auto&& e : gGlobal._sndServer.GetVector()) {
		if (e.second->GetType() == SoundItemBase::TYPE::ONESHOT) {
			PrintSoundItem(32, y, e.first, e.second);
			y += size;
		}
	}

	return true;
}