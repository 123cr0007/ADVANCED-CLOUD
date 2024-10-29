// DX ライブラリの機能(LoadGraph / DrawGraph)を使用しているのでインクルード
#include "DxLib.h"
#include "ObjectBase.h"

// static なメンバ変数は別途ローカルに宣言する必要がある
std::unordered_map<std::string, std::tuple<int>> ObjectBase::handleCache;
ObjectBase* ObjectBase::Instance=nullptr;
ObjectBase::ObjectBase() {
    Instance = this;
    x = 0;
    y = 0;
    _vPos = VGet(0, 0, 0);
    _vDir = VGet(0, 0, 0);
    CapsuleTop = VGet(0, 0, 0);
    _collision_r = 0.f;
    ChHight = 0.f;
    handle = -1;
    valid = false;
}

ObjectBase::~ObjectBase() {
}

bool ObjectBase::DeleteLoadType() {

    if (fileName.empty()) {
        return false;
    }

    auto cache = handleCache[fileName];
    auto handle = std::get<0>(cache);

    if (handle == -1) {
        return false;
    }

    // 実は LoadGraph した画像は DeleteGraph で破棄しないとビデオメモリを圧迫してしまう
    DeleteGraph(handle);

    handleCache[this->fileName] = std::make_tuple(-1);
    fileName.clear();

    return true;
}

// 自機も敵も画像の読み込みはあるはずなので共通の処理として実装
// 自機も敵のクラスを派生クラスにしても、この Load を共通で使えば良い設計
// type=0の時2Dをロード,type=1の時3Dをロード
int ObjectBase::Load(const TCHAR* fileName, int type) {

    if (this->fileName.empty()) {
        this->fileName = std::string(fileName);
    }

    if (handleCache.count(this->fileName) == 0) {
        switch (type) {
        case 0:

            handle = LoadGraph(fileName);
            break;
        case 1:

            handle = MV1LoadModel(fileName);
            break;
        }
        

        if (handle != -1) {

            handleCache[this->fileName] = std::make_tuple(handle);
        }
    }
    else {
        auto cache = handleCache[this->fileName];

        handle = std::get<0>(cache);
    }

    return handle;
}

bool ObjectBase::Initialize() {

    CapsuleTop = VGet(_vPos.x, _vPos.y + ChHight, _vPos.z);
    return true;
}


bool ObjectBase::Process() {
    return true;
}

bool ObjectBase::Render(int type) {

    Rend(type);
    return true;
}


bool ObjectBase::Rend(int type) {
 
    switch (type) {
    case 0:

        DrawGraph(x, y, handle, TRUE);
        break;

    case 1:
        MV1DrawModel(handle);
        break;
    }
    return true;
}