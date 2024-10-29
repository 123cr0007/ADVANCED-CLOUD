#include "SoundServer.h"


SoundServer::SoundServer() {
	_isUpdate = false;
	_cntOneShot = 0;
	_bgmvolume = 100;
	_sevolume = 100;
	_voicevolume = 100;
	_oneshotvolume = 100;
}
SoundServer::~SoundServer() {
	Clear();
}

void SoundServer::Clear() {
	for (auto&& e : _v) {
		delete e.second;
	}
	_v.clear();
}

void SoundServer::Add(SoundItemOneShot* snd, int maxvol, int minvol) {

	snd->SetMaxVolume(maxvol);
	snd->SetMinVolume(minvol);
	snd->SetVolume(maxvol);
	std::string name = "__ONESHOT_SOUND_" + std::to_string(_cntOneShot);
	_cntOneShot++;
	Add(name, snd, maxvol,  minvol);
	snd->Play();		// OneShotは登録した瞬間再生する
}

void SoundServer::Add(std::string name, SoundItemBase* snd, int maxvol, int minvol) {

	snd->SetMaxVolume(maxvol);
	snd->SetMinVolume(minvol);
	snd->SetVolume(maxvol);

	if (_isUpdate) {
		_vAdd[name] = snd;
	}
	else {
		_v[name] = snd;
	}
	snd->SetSoundServer(this);
}

void SoundServer::Add(std::string name, SoundItemBase* snd, int maxvol, int minvol, bool fadein, bool fadeout) {

	snd->SetMaxVolume(maxvol);
	snd->SetVolume(maxvol);
	snd->SetMinVolume(minvol);
	snd->SetFadeIn(fadein);
	snd->SetFadeOut(fadeout);
	if (_isUpdate) {
		_vAdd[name] = snd;
	}
	else {
		_v[name] = snd;
	}
	snd->SetSoundServer(this);
}

bool SoundServer::Del(SoundItemBase* snd) {
	for (auto&& e : _v) {
		if (e.second == snd) {
			if (_isUpdate) {
				_vDel[e.first] = e.second;
			}
			else {
				std::string name = e.first;
				delete _v[name];
				_v.erase(name);
			}
			return true;
		}
	}
	return false;
}

void SoundServer::SetBGMVolume(int volume)
{
	int vol = volume;

	//最大で設定できる音量は100
	if (volume >= 100) {
		vol = 100;
	}

	//現在のボリュームを保存
	_bgmvolume = vol;

	float unitvol = 0;
	for (auto&& e : _v) {
	
		if(e.second->GetType() == SoundItemBase::TYPE::BGM) {

			unitvol = e.second->GetMaxVolume() / 100;
			int newvol = vol * unitvol;

				e.second->SetVolume(newvol);
			
		}
	}
}

void SoundServer::SetSEVolume(int volume)
{
	int vol = volume;

	//最大で設定できる音量は100
	if (volume >= 100) {
		vol = 100;
	}

	//現在のボリュームを保存
	_sevolume = vol;

	int unitvol = 0;

	for (auto&& e : _v) {

		if (e.second->GetType() == SoundItemBase::TYPE::SE) {

			unitvol = e.second->GetMaxVolume() / 100;
			int newvol = vol * unitvol;
			e.second->SetVolume(newvol);
		}
	}
}

void SoundServer::SetVOICEVolume(int volume)
{
	int vol = volume;

	//最大で設定できる音量は100
	if (volume >= 100) {
		vol = 100;
	}

	//現在のボリュームを保存
	_voicevolume = vol;

	int unitvol = 0;

	for (auto&& e : _v) {

		if(e.second->GetType()== SoundItemBase::TYPE::VOICE ) {

			unitvol = e.second->GetMaxVolume() / 100;
			int newvol = vol * unitvol;
			e.second->SetVolume(newvol);
		}
	}

}

void SoundServer::SetONESHOTVolume(int volume)
{
	int vol = volume;

	//最大で設定できる音量は100
	if (volume >= 100) {
		vol = 100;
	}

	//現在のボリュームを保存
	_oneshotvolume = vol;

	int unitvol = 0;

	for (auto&& e : _v) {

		if (e.second->GetType() == SoundItemBase::TYPE::ONESHOT) {

			unitvol = e.second->GetMaxVolume() / 100;
			int newvol = vol * unitvol;
			e.second->SetVolume(newvol);
		}
	}
}

//int SoundServer::GetBGMVolume()
//{
//	for(auto&& e : _v) {
//		if(e.second->GetType() == SoundItemBase::TYPE::BGM) {
//			return e.second->GetVolume();
//		}
//	}
//}
//
//int SoundServer::GetSEVolume()
//{
//	for(auto&& e : _v) {
//		if(e.second->GetType() == SoundItemBase::TYPE::SE) {
//			return e.second->GetVolume();
//		}
//	}
//}
//
//int SoundServer::GetVOICEVolume()
//{
//	for(auto&& e : _v) {
//		if(e.second->GetType() == SoundItemBase::TYPE::VOICE) {
//			return e.second->GetVolume();
//		}
//	}
//}

bool SoundServer::Del(std::string name) {
	if (_v.count(name) > 0) {
		return Del(_v[name]);
	}
	return false;
}

SoundItemBase* SoundServer::Get(std::string name) {
	if (_v.count(name) > 0) { return _v[name]; }
	return NULL;
}

void SoundServer::StopType(SoundItemBase::TYPE type) {
	for (auto&& e : _v) {
		
		if (e.second->GetType() == type) {
			e.second->Stop();
			
		}
	}
}

void SoundServer::Update() {
	// 追加リストのものを追加
	for (auto&& e : _vAdd) { Add(e.first, e.second, e.second->GetMaxVolume(), e.second->GetMinVolume()); }
	_vAdd.clear();

	// Updateを回す
	_isUpdate = true;
	for (auto&& e : _v) {
		e.second->Update();
	}
	_isUpdate = false;

	// 削除リストのものを削除
	for (auto&& e : _vDel) { Del(e.second); }
	_vDel.clear();
}