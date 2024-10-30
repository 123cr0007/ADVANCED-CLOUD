# AppFrameReadme
AppFrame を使用した作品の説明サンプル  
(この段落は使用しない)

# 作品タイトル名(自身の作品名)
DX ライブラリと C++ 言語で作成した 3D ハイスピードシューティングゲーム

# 開発環境
Windows 11  
Visual Studio Community 2022

# 使用ライブラリ
[DX ライブラリ](https://dxlib.xsrv.jp/)  
[nlohmann-json](https://github.com/nlohmann/json)

# プロジェクト構成と起動方法
下記 DxLib ディレクトリ内に [DX ライブラリ Windows版 VisualStudio(C++)用](https://dxlib.xsrv.jp/DxLib/DxLib_VC3_24d.zip) を DownLoad して解凍後の ***プロジェクトに追加すべきファイル_VC用*** ディレクトリの内容を全てコピーします。
<pre>
.
├── DxLib(ライブラリ用ディレクトリ)
│
├── AppFrame(自作ゲームフレームワーク用のライブラリ Project)
│   └─source
│     ├─Collision
│     │   ├── Collision.cpp(当たり判定用の関数が入っているクラス)
│     │   ├── Collision.h
│     │    
│     │─GamePad    
│     │   ├── GamePad.cpp(コントローラーの入力クラス)
│     │   ├── GamePad.h
│     │    
│     └──Mymath   
│        ├── mymath.cpp(よく使う計算関数が入っているクラス)
│        ├── mymath.h
│  
└── Game(ゲーム本体の Project)
    │
    ├── Game
    │   │
    │   ├── Res(ゲーム用リソースディレクトリ)
    │   │
    │   └── Source(ソースファイル)
    │       ├── Camera.cpp(極座標で動かしてるカメラクラス)
    │       ├── Camera.h
    │       ├── EnergyGauge.cpp(メインシステムであるジャンプに必要なエネルギーを管理するクラス)
    │       ├── EnergyGauge.h
    │       ├── Fellow.cpp(お供のクラス)
    │       ├── Fellow.h
    │       ├── Item.cpp(エネルギーの最大値を上げるアイテムのクラス)
    │       ├── Item.h
    │       ├── ModeGame.cpp(ゲームのメインのクラス)
    │       ├── ModeGame.h
    │       ├── ModeGameOver.cpp(ゲームオーバーのクラス)
    │       ├── ModeGameOver.h
    │       ├── ModeStory.cpp(ストーリーを流すとき時のクラス)
    │       ├── ModeStory.h
    │       ├── ModeTutorial.cpp(チュートリアルを流すとき時のクラス)
    │       ├── ModeTutorial.h
    │       ├── ObjectBase.cpp(プレイヤーや敵などのオブジェクトのもとになるクラス)
    │       ├── ObjectBase.h
    │       ├── Player.cpp(プレイヤーのクラス。当たり判定など)
    │       ├── Player.h
    │       ├── Stage.cpp(ステージのクラス。UEで作ったステージをJsonにして読み込む)
    │       ├── Stage.h
    │
    └── Game.sln(こちらを起動して下さい)
</pre>