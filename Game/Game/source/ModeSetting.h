#pragma once
#include "AppFrame.h"
#include"ModeTitle.h"

constexpr auto SETTING_MENU_X = 960;//ç∂è„ÇÃXç¿ïW
constexpr auto SETTING_MENU_Y = 220;//ç∂è„ÇÃYç¿ïW
constexpr auto SETTING_MENU_INTERVAL = 120;
class ModeSetting : public ModeBase
{
	typedef ModeBase base;
public:
	ModeSetting();
	~ModeSetting();

	static ModeSetting* _SetInstance;
	static ModeSetting* GetInstance() { return (ModeSetting*)_SetInstance; }
	
	bool Initalize();
	bool Terminate();
	bool Process();
	bool Render();

	void CtrlMove(int trg);
	void ChangeVolume(int key);
	void ChangeScene();

	void DrawContents();
	void DrawExit(int x,int y);
	void DrawArrow();
	void DrawSEVolumeBar(int x, int y, int volume);
	void DrawBGMVolumeBar(int x, int y, int volume);
	void DrawVOICEVolumeBar(int x, int y, int volume);
protected:
	
	int handle[2];
	int handle_bar[2];
	int handle_button;
	int handle_cont[3];
	int handle_icon[2];
	int handle_exit[3];
	int handle_arrow;
	
	int back_num;//îwåiÇÃî‘çÜ
	int choice;//ëIÇÒÇ≈Ç¢ÇÈî‘çÜ
	int count;

	int isChoice;
	int draw_num;
	int oldCount;

	int se_vol;
	int bgm_vol;
	int voice_vol;
	
	int item_num;//çÄñ⁄êî
};