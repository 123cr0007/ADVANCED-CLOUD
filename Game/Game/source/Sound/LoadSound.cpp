#include "DxLib.h"
#include "LoadSound.h"
#include "SoundServer.h"
#include "..//ApplicationGlobal.h"

SoundJson::SoundJson()
{




}

SoundJson::~SoundJson()
{




}

bool SoundJson::Initialize()
{


	return true;
}

void SoundJson::Process()
{
	//jsonをロード
	LoadData();

	//読み込んだデータをSoundServerに追加する
	SetData();

}

bool SoundJson::LoadData() 
{

	std::ifstream file("res/Sound/Sound.json");
	nlohmann::json json;
	file >> json;

	const nlohmann::json datas = json.at("data");

	for (auto& data : datas)
	{
		if (data.find("bgm") != data.end()) {

			data.at("bgm").get_to(_data._type);
			data.at("filename").get_to(_data._filename);
			data.at("maxvolume").get_to(_data._maxvol);
			data.at("minvolume").get_to(_data._minvol);

			_data._use3dsound = false;

			data.at("fadein").get_to(_data._fadein);
			data.at("fadeout").get_to(_data._fadeout);	
		}
		if (data.find("se") != data.end()) {

			data.at("se").get_to(_data._type);
			data.at("filename").get_to(_data._filename);
			data.at("maxvolume").get_to(_data._maxvol);
			data.at("minvolume").get_to(_data._minvol);
			data.at("3dsound").get_to(_data._use3dsound);

			_data._fadein = false;
			_data._fadeout = false;


		}
		if (data.find("voice") != data.end()) {

			data.at("voice").get_to(_data._type);
			data.at("filename").get_to(_data._filename);
			data.at("maxvolume").get_to(_data._maxvol);
			data.at("minvolume").get_to(_data._minvol);
			data.at("3dsound").get_to(_data._use3dsound);

			_data._fadein=false;
			_data._fadeout= false;

			
		}
		_vSound.push_back(_data);
	}
	return true;
}

bool SoundJson::SetData()
{
	std::vector<Sound>::iterator ite;
	ite = _vSound.begin();
	for(; ite != _vSound.end();ite++)
	{
		if ((ite)->_type == "bgm") {
			gGlobal._sndServer.Add((ite)->_filename, new SoundItemBGM(path + "BGM/" +(ite)->_filename), (ite)->_maxvol, (ite)->_minvol, (ite)->_fadein, (ite)->_fadeout);
		}
		else if ((ite)->_type == "se") {

			if((ite)->_use3dsound) {
				gGlobal._sndServer.Add((ite)->_filename, new SoundItemSE(path + "SE/" + (ite)->_filename, SoundItemBase::FLG_3D), (ite)->_maxvol, (ite)->_minvol);
			}
			else if(!(ite)->_use3dsound){
				gGlobal._sndServer.Add((ite)->_filename, new SoundItemSE(path + "SE/" + (ite)->_filename), (ite)->_maxvol, (ite)->_minvol);
			}

		}
		else if ((ite)->_type == "voice") {

			if((ite)->_use3dsound) {
				gGlobal._sndServer.Add((ite)->_filename, new SoundItemVOICE(path + "VOICE/" + (ite)->_filename, SoundItemBase::FLG_3D), (ite)->_maxvol, (ite)->_minvol);
			}
			else if(!(ite)->_use3dsound){
				gGlobal._sndServer.Add((ite)->_filename, new SoundItemVOICE(path + "VOICE/" + (ite)->_filename), (ite)->_maxvol, (ite)->_minvol);
			}

		}
		else { (ite)->_type = "error"; return false; }
	}
	return true;
}