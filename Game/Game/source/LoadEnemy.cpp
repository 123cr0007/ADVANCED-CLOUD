
#include "LoadEnemy.h"
#include "AppFrame.h"
#include <string>
#include <vector>

#include <nlohmann/json.hpp>	
#include <fstream>

LoadEnemy* LoadEnemy::LoadInstance = nullptr;
LoadEnemy::LoadEnemy()
{
	LoadInstance = this;

	LoadJson();
}

LoadEnemy::~LoadEnemy(){

}

bool LoadEnemy::Initialize(){
	return true;
}

bool LoadEnemy::Process(){
	
	return true;
}


bool LoadEnemy::LoadJson(){
	
	//json“Ç‚Ýž‚Ý
	std::ifstream file("res/Enemy/Endata.json");
	nlohmann::json json;
	file >> json;

	nlohmann::json Edata = json.at("data");
	for(auto&& data : Edata)
	{
		//Œ©–{
		//EnData en;
		//en.pos1.x = data.at("x1").get<float>();
		//data.at("x1").get_to(en.pos1.x);
		//‘å•¶Žš‚Æ¬•¶Žš‚É’ˆÓ
		data.at("num").get_to(_data.id);
		data.at("level").get_to(_data.level);
		data.at("x1").get_to(_data._sPos.x);
		data.at("y1").get_to(_data._sPos.y);
		data.at("z1").get_to(_data._sPos.z);
		data.at("x2").get_to(_data._gPos.x);
		data.at("y2").get_to(_data._gPos.y);
		data.at("z2").get_to(_data._gPos.z);

		if(_data.level != -1) 
		{
			float tmp = _data._sPos.y;
			_data._sPos.y = _data._sPos.z;
			_data._sPos.z = tmp;
			//SwapFloat(_data._sPos.y, _data._sPos.z);
			_data._sPos.z = _data._sPos.z * -1;

			tmp = _data._gPos.y;
			_data._gPos.y = _data._gPos.z;
			_data._gPos.z = tmp;
			//SwapFloat(_data._gPos.y, _data._gPos.z);
			_data._gPos.z = _data._gPos.z * -1;

			_vData.push_back(_data);
		}
	}
	return true;
}

bool LoadEnemy::SetJson(){
	//‚©‚¦‚Ä‚Ý‚½
	for(auto&& data:_vData)
	{
		Enemy* _en = new Enemy(data.id, data._sPos, data._gPos);
		EnemyManager::GetInstance()->Add(_en);
	}
	return true;
}


