#pragma once 

#include "appframe.h"
#include "Sound/SoundServer.h"

class ApplicationGlobal
{
public:
	ApplicationGlobal();
	virtual ~ApplicationGlobal();

	bool Init();

public:

	int		_cgCursor;
	SoundServer	_sndServer;//�T�E���h�T�[�o�[�̎���
}; 

// ���̃\�[�X��gGlobal���g����悤��
extern ApplicationGlobal		gGlobal;
