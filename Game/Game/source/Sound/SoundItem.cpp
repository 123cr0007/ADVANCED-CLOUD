#include "SoundItem.h"
#include "SoundServer.h"

SoundItemBase::SoundItemBase(std::string filename, int flg) {
	_filename = filename;
	_flg = flg;
	_snd = -1;
	_volume = 0;
	_maxvol = 0;
	_minvol = 0;
	_pan = 0;
	_frequency = 0;		// ファイルによって標準が違うので
	_sndServer = NULL;
	_fadein = false;
	_fadeout = false;
	_bgmmode = static_cast<int> (BgmMode::NORMAL); 
}	
SoundItemBase::~SoundItemBase() {
	Unload();
}

void	SoundItemBase::Unload() {
	if (_snd != -1) {
		DeleteSoundMem(_snd);
		_snd = -1;
	}
}

bool	SoundItemBase::IsLoad() {
	// 非同期ロードが終わっているか？
	if (_snd != -1 && CheckHandleASyncLoad(_snd) == FALSE) {
		return true;
	}
	return false;
}

bool	SoundItemBase::IsPlay() {
	if (_snd != -1 && CheckSoundMem(_snd) == 1) {
		return true;
	}
	return false;
}

void	SoundItemBase::Stop() {
	if (_snd != -1) {
		StopSoundMem(_snd);
	}
}

void SoundItemBase::Set3DSound(VECTOR pos,float distance)
{
	Set3DRadiusSoundMem(distance, _snd);					// 聞こえる距離
	Set3DPositionSoundMem(pos , _snd);	// 再生位置をキャラ位置に
}

int		SoundItemBase::GetVolume() { return _volume; }
void	SoundItemBase::SetVolume(int volume) {
	_volume = volume;
	if (_snd != -1) {
		ChangeVolumeSoundMem(_volume, _snd);
	}
}
int		SoundItemBase::GetMaxVolume() { return _maxvol; }
void	SoundItemBase::SetMaxVolume(int vol) {_maxvol = vol;}
int		SoundItemBase::GetMinVolume() { return _minvol; }
void	SoundItemBase::SetMinVolume(int vol) {_minvol = vol;}

int		SoundItemBase::GetPan() { return _pan; }
void	SoundItemBase::SetPan(int pan) {
	_pan = pan;
	if (_snd != -1) {
		ChangePanSoundMem(_pan, _snd);
	}
}

int		SoundItemBase::GetFrequency() { return _frequency; }
void	SoundItemBase::SetFrequency(int frequency) {
	_frequency = frequency; 
	if (_snd != -1 && _frequency != 0) {
		SetFrequencySoundMem(_frequency, _snd);
	}
}
void	SoundItemBase::ResetFrequency() {
	_frequency = 0;
	if (_snd != -1) {
		ResetFrequencySoundMem(_snd);
		_frequency = GetFrequencySoundMem(_snd);
	}
}

void	SoundItemBase::PlayMem(int flg) {
	// 再生前に音量等を設定
	SetVolume(_volume);
	SetPan(_pan);
	SetFrequency(_frequency);
	PlaySoundMem(_snd, flg, TRUE);
}

int		SoundItemBase::LoadMem(std::string filename) {
	if (_flg & FLG_3D) {
		SetCreate3DSoundFlag(TRUE);
	}
	int snd = LoadSoundMem(filename.c_str());
	SetCreate3DSoundFlag(FALSE);
	return snd;
}

void	SoundItemBGM::Play() {

	StreamLoad();	// ストリーム再生のものをロードする
	if (!IsPlay()) {	// BGMは再生されていない場合のみ

		if(_sndServer) {
			_sndServer->StopType(TYPE::BGM);
		}

		PlayMem(DX_PLAYTYPE_LOOP);
		
		tic = 0;
		if (_fadein) 
		{
			SetVolume(_minvol);
			_bgmmode = static_cast<int> (BgmMode::FADEIN);
		}
	}
}

void	SoundItemBGM::Stop() {

	if (IsPlay()) {	// BGMが再生されている場合のみ
		
		tic = 0;
		if (_fadeout)
		{ 
			_bgmmode = static_cast<int> (BgmMode::FADEOUT);
		}
		else
		{
			if (_snd != -1) {
				StopSoundMem(_snd);
			}
		}
	}
	
}
void  SoundItemBGM::Update() {
	base::Update();
	
	switch (_bgmmode)
	{
	case static_cast<int> (BgmMode::NORMAL) :

		break;

	case static_cast<int> (BgmMode::FADEIN):

		if (IsPlay() == true) {

			if (_fadein == true) {		//フェードイン機能が有効なら

				if (tic == 1)
				{
					int vol = GetVolume();	//現在の音量を取得
					if (vol < _maxvol) {	//音量が最大より小さいの時のみ

						SetVolume(vol + 1);	//音量を上げる
					}
					else {	//最大音量なら

						SetVolume(_maxvol);		//修正する

						_bgmmode = static_cast<int> (BgmMode::NORMAL);

					}
					tic = 0;
				}
				tic++;
			}

		}
		break;


	case static_cast<int> (BgmMode::FADEOUT) :

		if (_fadeout == true) {		//フェードアウト機能が有効なら

			if (tic == 1)
			{
				int vol = GetVolume();	//現在の音量を取得

				if (vol > _minvol) {	//音量が最小より大きい時のみ

					SetVolume(vol - 1);	//音量を下げる

				}
				else {	//最小音量なら

					SetVolume(_minvol);		//修正する

					_bgmmode = static_cast<int> (BgmMode::NORMAL);

					if (_snd != -1) {
						StopSoundMem(_snd);
					}
				}
				tic = 0;
			}
			tic++;
		}
		break;
	}
}

void	SoundItemOneShot::Update() {
	int snd = _snd;
	base::Update();

	if (snd != -1 && _snd == -1) {
		// 再生されていたがUnload()されている場合、自分を削除
		_sndServer->Del(this);
	}
}