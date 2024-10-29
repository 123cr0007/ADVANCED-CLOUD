#pragma once
#pragma once
// ObjectBase クラスの定義(Load 関数)で TCHAR 型を宣言(使用)しているのでインクルード
#include <tchar.h>
#include <unordered_map>
#include <tuple>
#include <string>

#include "DxLib.h"
#include "Effect.h"
#include "EffectManager.h"

// Game クラスと合わせるやつ
class ObjectBase {
public:
    ObjectBase();
    virtual ~ObjectBase();

    static ObjectBase* Instance;
    static ObjectBase* GetInstance() { return (ObjectBase*)Instance; }

    // キャッシュしているハンドルを解放する
    virtual bool DeleteLoadType();

    // 自機も敵も画像の読み込みはあるはずなので共通の処理として実装。
    // typeに読み込みファイルに応じた番号を入れる。0:画像ファイル 1:MV1ファイル
    int Load(const TCHAR* fileName,int type);
    virtual bool Rend(int type);

    // これを自機と敵のクラス側で override して、それぞれクラス専用の関数書き換えたりする
    virtual bool Initialize();
    virtual bool Process();
    // type=0の時2Dを描画,type=1の時3Dを描画
    virtual bool Render(int type);

    virtual void End() { valid = false; }

    // ゲッター
    int GetX() const { return x; }
    int GetY() const { return y; }
    VECTOR GetPos() { return _vPos; };
    VECTOR GetDir() { return _vDir; };
    float GetRadius() const { return _collision_r; };
    bool IsValid() const { return valid; }
    int GetEffect() { return ef_type;}
    int GetRelJump() { return release_jump; }


    //セッター
    void SetPos(VECTOR pos) { _vPos = pos; };
    void SetRelJump(int RelJump) { release_jump = RelJump; };

protected:
    // 取り合えず描画に必要な最低限の変数
    // (DrawGraph が必要としている)
    int handle;
    int ef_type;
    //いろんなところで使える汎用変数
    int x;
    int y;
    VECTOR _vPos;       //
    VECTOR _vDir;       //
    VECTOR CapsuleTop;
    VECTOR CapsuleRow;

    float _collision_r;     //キャラの半径
    float ChHight;          //キャラの高さ（カプセル作るときの）

    int     release_jump; //ジャンプの解放状況 0:初期、1:3個目解放(仮)

    bool valid;

    std::string fileName;
    
    static std::unordered_map<std::string, std::tuple<int>> handleCache;
};