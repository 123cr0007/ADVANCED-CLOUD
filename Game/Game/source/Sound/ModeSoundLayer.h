#pragma once

#include "appframe.h"
#include "SoundItem.h"
#include "LoadSound.h"
#include <string>


// ���[�h
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


	int		_loadTimeMs;		// ���[�h�o�^�ɂ�����������
	int		_asyncLoadBaseTimeMs;	// �񓯊����[�h�ɂ�����������(�v�Z�p)
	int		_asyncLoadTimeMs;		// �񓯊����[�h�ɂ�����������

	std::string		_msg;			// �������b�Z�[�W�\���p

	int		_listCur;			// �I�����X�g�J�[�\���ʒu
	int		_listViewStart, _listViewNum;	// �I�����X�g�\���֘A
	SoundJson* _lodsound;
};