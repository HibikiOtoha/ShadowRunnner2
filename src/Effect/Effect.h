#pragma once

class Effect
{
private:

	// 定数
	const int	EffectParticleLimit = 20000000;				// 画面に表示できる最大パーティクル数
	const char* EffectFilePath = "data/Effect/ProjectDanmakuGirls/wind.efkefc";		// ファイルパス
	const float EffectSize = 5.0f;					// エフェクトサイズ
	const int	EffectPlayInterval = 60;					// エフェクトを再生する周期
	const float	EffectMoveSpeed = 0.2f;					// エフェクトが移動する速度

	// 変数
	int effectResourceHandle;	// エフェクトのリソース用
	int playingEffectHandle;	// 再生中のエフェクトハンドル

	// 今回の動作でにみ必要な変数
	int		playCount;			// 周期的に再生するためのカウント

	//再生したいときかどうか
	bool Playback;

public:
	Effect();	//コンストラクタ
	~Effect();	//デストラクタ
	void Initialize();	//初期化
	void Load(); //読み込み
	void Update(Vector3 pos, bool flag);	//読み込み
	void Draw();		//描画

	
};