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
	SoundServer	_sndServer;//サウンドサーバーの実体
}; 

// 他のソースでgGlobalを使えるように
extern ApplicationGlobal		gGlobal;
