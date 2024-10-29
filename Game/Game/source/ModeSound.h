#pragma once
#include "appframe.h"
#include "ApplicationGlobal.h"
#include "Sound/LoadSound.h"
#include <string>


// ÉÇÅ[Éh
class ModeSound : public ModeBase
{
	typedef ModeBase base;
public:

	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

private:

	SoundJson* _lodsound;

	int jumpcount[4];
};