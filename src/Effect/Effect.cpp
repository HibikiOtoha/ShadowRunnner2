#include "DxLib.h"
#include <EffekseerForDXLib.h>
#include "Vector3.h"
#include "Effect.h"


// コンストラクタ
Effect::Effect()
    : effectResourceHandle(-1)
    , playingEffectHandle(-1)
    , playCount(0)
{
    // 初期化
    Initialize();

    // 読み込み
    Load();
}

// デストラクタ
Effect::~Effect()
{
    // エフェクトリソースの開放
    // (Effekseer終了時に破棄されるので削除しなくてもいい)
    if (effectResourceHandle != NULL) {
        DeleteEffekseerEffect(effectResourceHandle);
    }
}

// 初期化
void Effect::Initialize()
{
    // DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
    // Effekseerを使用するには必ず設定する。
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    // 引数には画面に表示する最大パーティクル数を設定する。
    if (Effkseer_Init(EffectParticleLimit) == -1) { DxLib_End(); }

    Playback = false;
}

// 読み込み
void Effect::Load()
{
    // エフェクトのリソースを読み込む
    effectResourceHandle = LoadEffekseerEffect(EffectFilePath, EffectSize);    
}

//更新
void Effect::Update(Vector3 pos,bool flag)
{
    Playback = flag;
    //再生してほしいタイミングの時
    if (Playback) {
        // 定期的にエフェクトを再生する
        if (!(playCount % EffectPlayInterval))
        {
            // SetRotationPlayingEffekseer3DEffect(playingEffectHandle, 0, 0,0);

             // エフェクトを再生する。
            playingEffectHandle = PlayEffekseer3DEffect(effectResourceHandle);
        }


        // 再生カウントを進める
        playCount++;

        // 再生中のエフェクトを移動する。
        SetPosPlayingEffekseer3DEffect(playingEffectHandle, pos.x, pos.y, pos.z);

        // Effekseerにより再生中のエフェクトを更新する。
        UpdateEffekseer3D();
    }
}

// 描画
void Effect::Draw()
{
    //再生してほしいタイミングの時
    if (Playback) {
        // Effekseerにより再生中のエフェクトを描画する。
        DrawEffekseer3D();
    }
}