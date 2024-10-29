#include"UI.h"

UI::UI()
{
	SetUseASyncLoadFlag(TRUE);

	ui_energy[0] = LoadGraph("res/UI/Energy/UI_Game_EnPoint_2.png");//ベース
	ui_energy[1] = LoadGraph("res/UI/Energy/half/UI_Game_EnPoint_1.png");//右光
	ui_energy[2] = LoadGraph("res/UI/Energy/half/UI_Game_EnPoint_2.png");//左光
	ui_energy[3] = LoadGraph("res/UI/Energy/half/UI_Game_EnPoint_4.png");//

	//配列0:上,配列1:右,配列2:左,配列3:下
	//0:月,1:曇,2:陽,3:星
	ui_jump_lock[0] = LoadGraph("res/UI/Jump/UI_Game_Skill_lock_0.png");//ロック
	ui_jump_lock[1] = LoadGraph("res/UI/Jump/UI_Game_Skill_lock_3.png");
	ui_jump_lock[2] = LoadGraph("res/UI/Jump/UI_Game_Skill_lock_1.png");
	ui_jump_lock[3] = LoadGraph("res/UI/Jump/UI_Game_Skill_lock_2.png");

	ui_jump_off[0] = LoadGraph("res/UI/Jump/UI_Game_Skill_off_0.png");//OFF
	ui_jump_off[1] = LoadGraph("res/UI/Jump/UI_Game_Skill_off_3.png");
	ui_jump_off[2] = LoadGraph("res/UI/Jump/UI_Game_Skill_off_1.png");
	ui_jump_off[3] = LoadGraph("res/UI/Jump/UI_Game_Skill_off_2.png");

	ui_jump_on[0] = LoadGraph("res/UI/Jump/UI_Game_Skill_on_0.png");//ON
	ui_jump_on[1] = LoadGraph("res/UI/Jump/UI_Game_Skill_on_3.png");
	ui_jump_on[2] = LoadGraph("res/UI/Jump/UI_Game_Skill_on_1.png");
	ui_jump_on[3] = LoadGraph("res/UI/Jump/UI_Game_Skill_on_2.png");

	ui_button[0] = LoadGraph("res/UI/Jump/UI_Game_Skill_Y.png");//ボタン
	ui_button[1] = LoadGraph("res/UI/Jump/UI_Game_Skill_B.png");
	ui_button[2] = LoadGraph("res/UI/Jump/UI_Game_Skill_A.png");
	ui_button[3] = LoadGraph("res/UI/Jump/UI_Game_Skill_X.png");

	ui_tk_base[0] = LoadGraph("res/UI/TimeKeeper/UI_Game_Time_1.png");
	ui_tk_base[1] = LoadGraph("res/UI/TimeKeeper/UI_Game_Time_l1.png");
	ui_tk_base[2] = LoadGraph("res/UI/TimeKeeper/UI_Game_Time_r1.png");

	ui_tk_bar[0] = LoadGraph("res/UI/TimeKeeper/half/UI_Game_time_l2.png");//タイムキーパー
	ui_tk_bar[1] = LoadGraph("res/UI/TimeKeeper/half/UI_Game_time_l3.png");
	ui_tk_bar[2] = LoadGraph("res/UI/TimeKeeper/half/UI_Game_time_r2.png");
	ui_tk_bar[3] = LoadGraph("res/UI/TimeKeeper/half/UI_Game_time_r3.png");
	ui_tk_icon[0] = LoadGraph("res/UI/TimeKeeper/UI_Game_time_jewel_1.png");//消灯
	ui_tk_icon[1] = LoadGraph("res/UI/TimeKeeper/UI_Game_time_jewel_2.png");//点灯
	ui_tk_icon[2] = LoadGraph("res/UI/TimeKeeper/UI_Game_time_jewel_3.png");//縁まで点灯

	SetUseASyncLoadFlag(FALSE);
}



UI::~UI()
{
	delete this;
}

void UI::Initialize(int Screen_W,int Screen_H)
{
	playerUIPos = VGet(0, 0, 0);
	jumpUICenterPos = VGet(0, 0, 0);
	
	/*VECTOR plPos = Player::GetPLInstance()->GetPos();
	VECTOR camPos = Camera::GetInstance()->GetCamPos();
	playerUIPos = SetPlayerUIPos(plPos, camPos);
	playerUIPos2D = ConvWorldPosToScreenPos(playerUIPos);*/

	SetJumpUIPos( Screen_W, Screen_H);
}

//チップ
int EnergyIcon[ENERGY_UI_MAX] =
{
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

void UI::Process(VECTOR player_pos, VECTOR camera_pos)
{
	SetPlayerUIPos(player_pos, camera_pos);
	playerUIPos2D = ConvWorldPosToScreenPos(player_pos);
}

void UI::Render(VECTOR player_pos,int max_en,int now_en, int Jumptime, int JustJumpTime)
{
	//タイムキーパー
	DrawTimekeeper(playerUIPos2D.x, playerUIPos2D.y, Jumptime, JustJumpTime);
	
	//エネルギーポイント
	max = max_en;
	now = now_en;
	DrawEnergyIcon();

	//ジャンプアイコン
	for(int i = 0; i < 4; i++)
	{
		//初期ジャンプ
		if(i == 2)
		{
			//通常ジャンプ
			status[i] = 2;//初期から使えるため
			if(jumptype == i)
			{
				status[i] = 1;
			}
			DrawJumpIcon(i, status[i]);
		}

		//解放ジャンプ
		else if (i == 1)
		{
			if(AMG_wp::StageNum>=2)
			{
				status[i] = 2;
			}
			//ふわふわ
			if (jumptype == i)
			{
				status[i] = 1;
			}
			DrawJumpIcon(i, status[i]);
		}
		else if (i == 0)
		{
			//回避ジャンプ
			if (AMG_wp::StageNum >= 3)
			{
				status[i] = 2;
			}
			if(jumptype == i)
			{
				status[i] = 1;
			}
			DrawJumpIcon(i, status[i]);
		}
		else if(i == 3)
		{
			//攻撃ジャンプ
			if (AMG_wp::StageNum >= 3)
			{
				status[i] = 2;
			}
			if(jumptype == i)
			{
				status[i] = 1;
			}
			DrawJumpIcon(i, status[i]);
		}
		//ここまで
	}
}

void UI::DrawEnergyIcon()
{
	max = max / 2;
	int baseX = playerUIPos2D.x - (UI_GRAPH_W*max/2);

	//ベース部分
	for(int x = 0; x < max; x++)
	{
		if(EnergyIcon[x] == 1)
		{
			DrawGraph(baseX + x * UI_GRAPH_W, playerUIPos2D.y + 50, ui_energy[0], TRUE);
		}
	}

	int px = 0;
	//変動部分
	for(int x = 0; x < now;)
	{		
		if(x % 2 == 0) 
		{
			px = x/2 ;
		}
		//とりあえず2枚出る		
		switch(x%2)
		{
		//左
		case 0:
			DrawGraph(baseX  + px * UI_GRAPH_W, playerUIPos2D.y + 50, ui_energy[2], TRUE);
			break;
		//右
		case 1:
			DrawGraph(baseX + px * UI_GRAPH_W, playerUIPos2D.y + 50, ui_energy[1], TRUE);
			break;
		};
		x++;
	}
}

//改善対象
void UI::DrawJumpIcon(int i, int status)
{
	switch (status)
	{
	case 0:
		DrawGraph(jumpUIPos[i].x, jumpUIPos[i].y, ui_jump_lock[i], TRUE);
		DrawGraph(ButtonUIPos[i].x, ButtonUIPos[i].y, ui_button[i], TRUE);
		break;
	case 1:
		DrawGraph(jumpUIPos[i].x, jumpUIPos[i].y, ui_jump_off[i], TRUE);
		DrawGraph(ButtonUIPos[i].x, ButtonUIPos[i].y, ui_button[i], TRUE);
		break;
	case 2:
		DrawGraph(jumpUIPos[i].x, jumpUIPos[i].y, ui_jump_on[i], TRUE);
		DrawGraph(ButtonUIPos[i].x, ButtonUIPos[i].y, ui_button[i], TRUE);
		break;
	}
}

VECTOR UI::SetPlayerUIPos(VECTOR player_pos, VECTOR camera_pos)
{
	//playerUIPos = ConvWorldPosToScreenPos(player_pos);
	// カメラの向いている角度を取得
	float sx = camera_pos.x - player_pos.x;
	float sz = camera_pos.z - player_pos.z;
	float rad = atan2(sz, sx);

	playerUIPos.x = player_pos.x + cos(rad) * 50;
	playerUIPos.z = player_pos.z + sin(rad) * 50;
	return playerUIPos;
}

void UI::SetJumpUIPos(int Screen_W, int Screen_H)
{
	jumpUICenterPos.x = Screen_W*0.9;
	jumpUICenterPos.y = Screen_H*0.8;
	for(int i = 0; i < 4; i++) 
	{
		GetGraphSize(ui_jump_lock[i], &size_x, &size_y);
		//上
		if(i==0)
		{
			jumpUIPos[i] = VGet(jumpUICenterPos.x - size_x *0.5, jumpUICenterPos.y - size_y * 1.19, 0);
		}
		//右
		else if(i==1)
		{
			jumpUIPos[i] = VGet(jumpUICenterPos.x + size_x *0.2 , jumpUICenterPos.y - size_y * 0.5, 0);
		}
		//下
		else if(i==2)
		{
			jumpUIPos[i] = VGet(jumpUICenterPos.x - size_x *0.5, jumpUICenterPos.y + size_y * 0.2, 0);
		}
		//左
		else if(i==3)
		{
			jumpUIPos[i] = VGet(jumpUICenterPos.x - size_x *1.2, jumpUICenterPos.y - size_y*0.49, 0);
		}
		GetGraphSize(ui_button[i], &size_x, &size_y);
		if(i == 0)
		{
			ButtonUIPos[i] = VGet(jumpUICenterPos.x - size_x * 0.5, jumpUICenterPos.y - size_y * 1.5, 0);
		}
		else if(i == 1)
		{
			ButtonUIPos[i] = VGet(jumpUICenterPos.x + size_x * 0.5, jumpUICenterPos.y - size_y * 0.5, 0);
		}
		else if(i == 2)
		{
			ButtonUIPos[i] = VGet(jumpUICenterPos.x - size_x *0.5, jumpUICenterPos.y + size_y * 0.5, 0);
		}
		else if(i == 3)
		{
			ButtonUIPos[i] = VGet(jumpUICenterPos.x - size_x * 1.5, jumpUICenterPos.y - size_y * 0.5, 0);
		}
	}
	
}

void UI::SetEnergyUIChip()
{
	//
	int i = _en->GetHaveNowEnergy();
	for(i; i < _en->GetHaveMaxEnergy(); i++) {
		EnergyIcon[i] = 0;
	}
}

void UI::SetJumpStatus(int pl_status, int pl_jumptype)
{
	for(int i = 0; i < 4; i++)
	{
		status[i] = pl_status;
	}

	jumptype = pl_jumptype;
}

void UI::DrawTimekeeper(int x,int y,int Jumptime,int JustJumpTime)
{
	if(Jumptime != 0)
	{
		//2Dを使って強引に作る
		GetGraphSize(ui_tk_base[0], &size_x, &size_y);
		//タイムキーパー
		float rx = 960 + size_x * 0.5;
		float lx = 960 - size_x * 0.5;
		//ベース部分
		DrawGraph(lx, y - 3, ui_tk_base[1], TRUE);
		DrawGraph(lx, y - 3, ui_tk_base[2], TRUE);
		DrawGraph(lx, y - 3, ui_tk_bar[0], TRUE);
		DrawGraph(lx, y - 3, ui_tk_bar[2], TRUE);
		// 光る部分
		//準備

		float time = Jumptime - JustJumpTime;
		float percent;
		if(time > 0)
		{
			//
			percent = DeciToPer(time / (60.0f - JustJumpTime));
		}
		else
		{
			percent = 0;
		}
		//percent = 100 - percent;

		GetGraphSize(ui_tk_base[0], &size_x, &size_y);

		int h = y + size_y-2;
		int x1 = lx + 19;
		int x2 = rx - 19;
		int barY = y - 4;//位置調整用
		lx += (x2 - lx) / 100 * percent;
		rx -= (rx - x1) / 100 * percent;
		

		
		//緑ゲージ部分(左上、右下)
		/*DrawBox(x1, barY, rx, h - 1, GetColor(255,255,0), TRUE);
		DrawBox(lx, barY, x2, h - 1, GetColor(255, 255, 0), TRUE);*/
		/*DrawExtendGraph(x1, barY, rx, h-1, ui_tk_bar[1], TRUE);
		DrawExtendGraph(lx, barY, x2, h-1, ui_tk_bar[3], TRUE);*/
		//左側
		//DrawRotaGraph3(x1, y, 0, 0,(double)percent, (double)1, 0, ui_tk_bar[1], TRUE);
		DrawModiGraph(x1, barY, rx, barY, rx, h, x1, h, ui_tk_bar[1], TRUE);
		DrawModiGraph(lx, barY, x2, barY, x2, h, lx, h, ui_tk_bar[3], TRUE);
		//DrawLine(x1, barY, rx-10, h, GetColor(255, 0, 0), 1);
		if(JustJumpTime >= Jumptime > 0)
		{
			DrawGraph(x - size_x / 2, y, ui_tk_icon[1], TRUE);
		}
		else
		{
			DrawGraph(x - size_x / 2, y, ui_tk_icon[0], TRUE);
		}
		
	}
}