#pragma once

#include "nlohmann/json.hpp"

#include <fstream>
#include <vector>



class SoundJson
{
public:

	SoundJson();
	virtual ~SoundJson();

	bool Initialize();

	void Process();

	bool LoadData();
	bool SetData();

protected:

	std::string path = "res/Sound/";

	struct Sound
	{
		std::string _type;
		std::string	_filename;
		int		_maxvol;
		int		_minvol;
		bool    _fadein;
		bool	_fadeout;
		bool	_use3dsound;
	};

	Sound _data;

	std::vector<Sound> _vSound;

};