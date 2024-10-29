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

	// 3D�u���b�N�`��p
	int HandleJson[ITEM_MAX];	// ABC

	// 3D�u���b�N���
	struct ItemPos {

		std::string		name;
		float		x, y, z;	// ���W
		int			modelNo;	// ���f���ԍ�(name�������)
		int			type;		// �A�C�e���̃^�C�v
		int			use;		// �g���i�`�悷��j���ǂ���	
		int			cnt;		// �G�t�F�N�g�p�J�E���g
	};

	float check[100];
	std::vector<ItemPos>	_vItem;
};