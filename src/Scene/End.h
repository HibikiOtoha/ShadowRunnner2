#pragma once

class Ending {
private:

	//エンドシーンの画像
	int End_image;
	//プレイヤーの画像
	int player_image;

	int bEnd_image;

	//クリアか、失敗か
	bool clear;
	

public:
	Ending();

	//切り替え可能かどうか
	bool Scene_Change;


	void Init(int player_model);
	void Update(bool gameclear);
	void Render();
	void Exit();
};