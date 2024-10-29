#pragma once
#pragma once
#include <tchar.h>
#include <unordered_map>
#include <tuple>
#include <string>

#define OBJ_MAX 26

#define	DEG2RAD(x)			( ((x) / 180.0f ) * PI )
#define	RAD2DEG(x)			( ((x) * 180.0f ) / PI )

constexpr auto RATATE_RATE = 3;	//n秒間で何回転するか

class Stage {
public:

	static Stage* STInstance;
	static Stage* GetSTInstance() { return (Stage*)STInstance; }

	Stage();
	virtual ~Stage();

	bool MainInitialize();
	bool Terminate();
	bool Process();
	bool Render();

	bool TitleInitialize();
	bool TitleTerminate();
	bool TitleProcess();
	bool RenderForTitle();

	//ゲッター

	int GetSHandle() { return _handleMap; };
	int GetSCol() { return _frameMapCollision; };

	int GetJHandle(int i) { return HandleJson[i]; };
	bool GetFinLoad() { return finLoad; };

protected:

	//マップ入れとく系
	//スカイドーム
	int _handleSkySphere;
	//マップと地面用コリジョン
	int _handleMap;
	int _frameMapCollision;

	// 3Dブロック描画用
	int HandleJson[OBJ_MAX];	// ABC
	int CollisionJson[OBJ_MAX];	// ABC

	
	bool ChangeAdvanceStage;
	float ratate;

	// 3Dブロック情報
	struct BLOCKPOS {
		std::string		name;
		float		x, y, z;	// 座標
		float		rx, ry, rz;	// 回転
		float		sx, sy, sz;	// 拡大
		int			modelNo;	// モデル番号(nameから引く)
		int			handle;		// ハンドル
		int			Collision;	// コリジョン
		bool		AdvanceStage;	//ステージを進行するかのトリガー
		bool        stepClassroom;	//スタートとゴールが教室なので判定するよう
		int			SaveNum;	//セーブポイントの番号
	};
	std::vector<BLOCKPOS>	_vBlockPos;
	std::vector<BLOCKPOS>	_vTitlePos;

	int         stepClassroomNum;
	bool finLoad;		//読み込みが終わったかどうか
	int EfUseCount = 0;	//エフェクトの使用回数
	int	SaveNumCount;	//セーブポイントの番号確認用
public:

	// ゲッター
	const std::vector<BLOCKPOS>& GetBlockPositions() {

		return _vBlockPos;
	}
	
	int GetStepClassroomNum() {
		return stepClassroomNum;
	}
	

	//セッター
	void SetAdvanceStage(int index, bool newAdvanceStage) {
		if (index >= 0 && index < _vBlockPos.size()) {
			_vBlockPos[index].AdvanceStage = newAdvanceStage;
		}
	}
	void SetStepClassroom(int index, bool newStepClassroom) {
		if (index >= 0 && index < _vBlockPos.size()) {
			_vBlockPos[index].stepClassroom = newStepClassroom;
		}
	}
	void SetAddStepClassroomNum(int addNum) {
		stepClassroomNum += addNum;
	}
	void SetStepClassroomNum(int addNum) {
		stepClassroomNum = addNum;
	}
};