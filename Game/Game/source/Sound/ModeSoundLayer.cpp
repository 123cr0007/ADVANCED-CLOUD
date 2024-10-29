#include "ModeSoundLayer.h"
#include "..//ApplicationGlobal.h"

bool ModeSoundLayer::Initialize() {
	if (!base::Initialize()) { return false; }

	_lodsound = new SoundJson();


	// �񓯊��ǂݍ��ݐݒ�
	SetUseASyncLoadFlag(TRUE);

	int timems = GetNowCount();
	_asyncLoadBaseTimeMs = timems;	// �񓯊����[�h�J�n����
	_asyncLoadTimeMs = 0;

	// ���f�[�^�t�@�C����ǂݍ���
	_lodsound->Process();
	
	// �ǂݍ��ݎ���
	_loadTimeMs = GetNowCount() - timems;

	// �����ǂݍ��ݐݒ�
	SetUseASyncLoadFlag(FALSE);

	// �I�����X�g�p
	_listCur = 0;
	_listViewStart = 0;
	_listViewNum = 20;


#ifdef _DEBUG // ���j���[�������\���̗L������

	// ���j���[�������\���̗L��
	_enableMenu = true;

#else

	// ���j���[�������\���̖���
	_enableMenu = false;

#endif // _DEBUG

	return true;
}

bool ModeSoundLayer::Terminate() {
	base::Terminate();

	delete _lodsound;//���[�hjson���폜

	return true;
}

bool ModeSoundLayer::Process() {
	base::Process();

	int key = GamePad::GetInstance()->GetKey(0);
	int trg = GamePad::GetInstance()->GetTrg(0);

	// �T�E���h�T�[�o�X�V
	gGlobal._sndServer.Update();

	// �񓯊����[�h�͏I��������H
	if (GetASyncLoadNum() == 0) {
		if (_asyncLoadTimeMs == 0) {
			// �񓯊����[�h�ɂ�����������
			_asyncLoadTimeMs = GetNowCount() - _asyncLoadBaseTimeMs;
		}
	}

	// ���j���[�����͗L�����H
	if (!_enableMenu) {
		return true;	// �����B
	}

	// ���̃��[�h��艺�̃��C���[��Process()���Ă΂Ȃ�
	ModeServer::GetInstance()->SkipProcessUnderLayer();

	// ESC�L�[�ŕ���
	bool close = false;
	if (trg & PAD_INPUT_9) {
		close = true;
	}


	// �L�[[A][S]�������ċ��Ȃ���
	if ((key & (PAD_INPUT_4 | PAD_INPUT_5)) == 0) {
		// �J�[�\���I��
		int itemNum = gGlobal._sndServer.GetVector().size();	// ���ڐ�
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
		_listCur = (_listCur + itemNum) % itemNum;		// ���ڐ��ŃJ�[�\�����[�v
		// �J�[�\���ʒu�͕\�����ɓ����Ă��邩�H
		while (true) {
			if (_listCur < _listViewStart) { _listViewStart--; continue; }
			if (_listCur >= _listViewStart + _listViewNum) { _listViewStart++; continue; }
			break;
		}
	}

	// �J�[�\���ʒu�̃f�[�^���擾
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



	// �L�[[A][S]�������ċ��Ȃ���
	if ((key & (PAD_INPUT_4 | PAD_INPUT_5)) == 0) {
		// �L�[����[Z]�ŃJ�[�\���ʒu�̃f�[�^�Đ�
		if (trg & PAD_INPUT_1) {
			if (sndItem && sndItem->IsLoad()) {
				sndItem->Play();
				_msg = sndItemName + " �Đ�";
			}
			else {
				_msg = sndItemName + " ���܂����[�h����Ă��Ȃ�";
			}
		}
		// �L�[����[X]�ŃJ�[�\���ʒu�̃f�[�^��~
		if (trg & PAD_INPUT_2) {
			if (sndItem && sndItem->IsPlay()) {
				sndItem->Stop();
				_msg = sndItemName + " ��~";
			}
		}
		// �L�[����[C]�ŃJ�[�\���ʒu�̃f�[�^��OneShot�Đ�
		if (trg & PAD_INPUT_3) {
			if (sndItem) {
				// �t�@�C���������o��
				std::string filename = sndItem->GetFileName();
				// OneShot�Đ��Ƃ��ăT�[�o�ɓo�^���A�Đ�
				SoundItemOneShot* snd = new SoundItemOneShot(filename);
				// OneShot�Đ���volume��pan��ς���T���v��
				snd->SetVolume(255);
				snd->SetPan(-255);
				gGlobal._sndServer.Add(snd,255,255);		// �T�[�o�ɓo�^�����u�ԂɍĐ������
			}
		}
	}
	else if (key & PAD_INPUT_4) {
		// �L�[����[A]�������Ȃ���㉺���E��Volume/Pan��ύX
		// �L�[[X]�ŁAVolume/Pan���Z�b�g
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
		// �L�[����[S]�������Ȃ���㉺���E��Frequency��ύX
		// �L�[����[X]�ŁAFrequency���Z�b�g
		if (sndItem) {
			int freq = sndItem->GetFrequency();
			if (freq > 0) {		// 0�̏ꍇ�A�܂��ݒ�ł��Ȃ�
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

	// ���j���[�I��
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
	char strIsLoaded[][128] = { "(�񓯊����[�h��)", "" };
	char strIsPlay[][128] = { "", "(�Đ���)" };
	char strIsOnMemory[][128] = { "", "(�I��������)" };
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

	// ���j���[�����͗L�����H
	if (!_enableMenu) {
		return true;	// �����B
	}

	// ���n
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 192);
	DrawFillBox(0, 0, 1280, 720, GetColor(0, 0, 0));
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	int y = 0, size = 16;
	DrawFormatString(0, y, GetColor(255, 0, 0), "���[�h����(ms): %d", _loadTimeMs); y += size;
	DrawFormatString(0, y, GetColor(255, 0, 0), "�񓯊��ǂݍ��ݒ��t�@�C����: %d", GetASyncLoadNum()); y += size;
	DrawFormatString(0, y, GetColor(255, 0, 0), "�񓯊����[�h����(ms): %d", _asyncLoadTimeMs); y += size;
	DrawFormatString(0, y, GetColor(255, 255, 255), "msg: %s", _msg.c_str()); y += size;

	// �I�����X�g���J�[�\���\��
	y = 100;
	int n = 0;
	for (auto&& e : gGlobal._sndServer.GetVector()) {
		if (n >= _listViewStart && n < _listViewStart + _listViewNum) {
			if (n == _listCur) {
				DrawFormatString(8, y, GetColor(255, 255, 255), "��");
			}
			PrintSoundItem(32, y, e.first, e.second);
			y += size;
		}
		n++;
	}

	// OneShot���X�g�\��
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