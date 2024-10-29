#pragma once

#include "SoundItem.h"

#include <string>
#include <map>

class SoundServer
{
public:
	SoundServer();
	virtual ~SoundServer();

	void Clear();
	void Add(SoundItemOneShot* snd, int maxvol, int minvol);
	void Add(std::string name, SoundItemBase* snd, int maxvol, int minvol);
	void Add(std::string name, SoundItemBase* snd, int maxvol, int minvol, bool fadein, bool fadeout);
	bool Del(std::string name);
	bool Del(SoundItemBase* snd);

	int GetBGMVolume() {return _bgmvolume;}
	int GetSEVolume() {return _sevolume;}
	int GetVOICEVolume() {return _voicevolume;}
	int GetONESHOTVolume() {return _oneshotvolume;}

	void SetBGMVolume(int volume);
	void SetSEVolume(int volume);
	void SetVOICEVolume(int volume);
	void SetONESHOTVolume(int volume);

	//int GetBGMVolume();
	//int GetSEVolume();
	//int GetVOICEVolume();

	SoundItemBase* Get(std::string name);

	std::map<std::string, SoundItemBase*>& GetVector() { return _v; }

	void StopType(SoundItemBase::TYPE type);

	void Update();

private:
	bool	_isUpdate;
	std::map<std::string, SoundItemBase*> _v;

	std::map<std::string, SoundItemBase*> _vAdd;
	std::map<std::string, SoundItemBase*> _vDel;

	int		_cntOneShot;

	int _bgmvolume;
	int _sevolume;
	int _voicevolume;
	int _oneshotvolume;

};