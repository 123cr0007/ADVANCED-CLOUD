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
	_frequency = 0;		// �t�@�C���ɂ���ĕW�����Ⴄ�̂�
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
	// �񓯊����[�h���I����Ă��邩�H
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
	Set3DRadiusSoundMem(distance, _snd);					// �������鋗��
	Set3DPositionSoundMem(pos , _snd);	// �Đ��ʒu���L�����ʒu��
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
	// �Đ��O�ɉ��ʓ���ݒ�
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

	StreamLoad();	// �X�g���[���Đ��̂��̂����[�h����
	if (!IsPlay()) {	// BGM�͍Đ�����Ă��Ȃ��ꍇ�̂�

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

	if (IsPlay()) {	// BGM���Đ�����Ă���ꍇ�̂�
		
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

			if (_fadein == true) {		//�t�F�[�h�C���@�\���L���Ȃ�

				if (tic == 1)
				{
					int vol = GetVolume();	//���݂̉��ʂ��擾
					if (vol < _maxvol) {	//���ʂ��ő��菬�����̎��̂�

						SetVolume(vol + 1);	//���ʂ��グ��
					}
					else {	//�ő剹�ʂȂ�

						SetVolume(_maxvol);		//�C������

						_bgmmode = static_cast<int> (BgmMode::NORMAL);

					}
					tic = 0;
				}
				tic++;
			}

		}
		break;


	case static_cast<int> (BgmMode::FADEOUT) :

		if (_fadeout == true) {		//�t�F�[�h�A�E�g�@�\���L���Ȃ�

			if (tic == 1)
			{
				int vol = GetVolume();	//���݂̉��ʂ��擾

				if (vol > _minvol) {	//���ʂ��ŏ����傫�����̂�

					SetVolume(vol - 1);	//���ʂ�������

				}
				else {	//�ŏ����ʂȂ�

					SetVolume(_minvol);		//�C������

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
		// �Đ�����Ă�����Unload()����Ă���ꍇ�A�������폜
		_sndServer->Del(this);
	}
}