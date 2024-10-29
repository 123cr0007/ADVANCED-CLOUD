#pragma once

#include "ObjectBase.h"
#include <string>
#include <vector>
#include "Player.h"

constexpr auto ITEM_MAX = 2;
constexpr auto EFFECT_WAIT = 1;

class Item {

public:
	static Item* ItemInstance;
	static Item* ItemGetInstance() { return (Item*)ItemInstance; }

	Item();
	virtual ~Item();

	bool Initialize();
	bool Terminate();
	bool Process();
	bool Render();

	bool Wait(int second,int count);
protected:

	// 3Dブロック描画用
	int HandleJson[ITEM_MAX];	// ABC

	// 3Dブロック情報
	struct ItemPos {

		std::string		name;
		float		x, y, z;	// 座標
		int			modelNo;	// モデル番号(nameから引く)
		int			type;		// アイテムのタイプ
		int			use;		// 使う（描画する）かどうか	
		int			cnt;		// エフェクト用カウント
	};

	float check[100];
	std::vector<ItemPos>	_vItem;
};