#pragma once

class Game {
public:
	enum SAVE
	{
		WALL,
		WIRE,
		WIRE2,
		EX,

		SAVE_MAX
	};


private:
	//ステージモデル
	int stage_model;
	//スカイボックスモデル
	int sky_model;
	//プレイヤーモデル
	int player_model;

	//ブラー用の画像を整理する関数
	void Blur();

#define IMAGE_MAX 10
	//
	int count;
	//モーションブラー風の半透明のスクリーンを書き込む用の変数
	int screen_image[IMAGE_MAX];
	//int screen_image;
	//一フレームの画像を保存し半透明で描画する用の変数
	int blur_image[IMAGE_MAX];

	//開始時目が空いているか
	bool eyeOpen = false;


	//チェックポイントの座標
	float SavePoint[SAVE_MAX];
	//チェックポイントをクリアした(通った)か
	bool ClearFlag[SAVE_MAX];
	//動画の再生が終わっているかどうか(二度目の再生がないように)
	int PlayMovie;

	//プレイヤーがセーブポイントにいるかどうか
	void CheckSave( float posx);

public:
	Game();

	//切り替え可能かどうか
	bool Scene_Change;
	bool Game_Clear;


	void Init(int M_Stage,int M_Sky,int M_Player);
	void Update(bool tutrial_mode, int Sound,int Sens);
	void Render();
	void Exit();

	void Reset();
};