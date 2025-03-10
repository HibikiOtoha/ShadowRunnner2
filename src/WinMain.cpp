#include "DxLib.h"
#include "GameMain.h"
#include "WinMain.h"


char KeyBuffer[256];
int KeyFrame[256];
int MouseLeftFrame;
int MouseRightFrame;

int NowMouseX;
int NowMouseY;
int BeforeMouseX;
int BeforeMouseY;


// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int Time;

	SetOutApplicationLogValidFlag(FALSE);	//Logを表示しない

	//ChangeWindowMode(TRUE);		//ウィンドウモード
	ChangeWindowMode(FALSE);		//全画面モード

	SetMainWindowText("SHADOW RUNNER");
	SetBackgroundColor(0, 0, 0);	//後ろの色を黒

	SetDoubleStartValidFlag(TRUE);	//二重起動が可能かどうか
	SetAlwaysRunFlag(TRUE);	//非アクティブ時でも動くか

	SetGraphMode(SCREEN_W, SCREEN_H, 32);	//ウィンドウサイズ設定(x,y,32bitカラー)



	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);	// 描画先画面を設定
	SetTransColor(255, 0, 255);	// 作成するグラフィックハンドルに適用する透過色を設定
	srand(GetNowCount() % RAND_MAX);

	//オプション処理に移動
	//SetMouseDispFlag(TRUE);	//マウスカーソルの表示(全画面)
	//SetMouseDispFlag(FALSE);	//マウスカーソルの表示(全画面)

	for (int i = 0; i < 256; i++) {
		KeyFrame[i] = 0;
	}
	MouseLeftFrame = 0;
	MouseRightFrame = 0;

	SetCameraNearFar(0.1f, 3000.0f);	// カメラの Nearクリップ面と Farクリップ面の距離を設定
	SetupCamera_Perspective(TO_RADIAN(45.0f));	// 遠近法カメラをセットアップする

	SetUseZBuffer3D(TRUE);	// Ｚバッファを使用するかどうかを設定
	SetWriteZBuffer3D(TRUE);	// Ｚバッファに書き込みを行うかどうかを設定
	//ChangeLightTypeDir(VGet(0.8f, -1.2f, 1.0f));	// デフォルトライトのタイプをディレクショナルライトにする
	ChangeLightTypeDir(VGet(1.0f, 1.0f, 0.0f));	// デフォルトライトのタイプをディレクショナルライトにする

	GameInit();


	//エスケープか、ウィンドウの閉じるボタンなどが押されていないときmainループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_F4) == 0)
	{
		Time = GetNowCount();
		ClearDrawScreen();	//描画をクリア

		GetHitKeyStateAll(KeyBuffer);	// すべてのキーの押下状態を取得する

		for (int i = 0; i < 256; i++) {
			if (KeyBuffer[i])	KeyFrame[i]++;
			else				KeyFrame[i] = 0;
		}

		if (CheckMouseInput(MOUSE_INPUT_LEFT))	MouseLeftFrame++;
		else										MouseLeftFrame = 0;

		if (CheckMouseInput(MOUSE_INPUT_RIGHT))	MouseRightFrame++;
		else										MouseRightFrame = 0;

		NowMouseX = GetMouseX();
		NowMouseY = GetMouseY();

		GameUpdate();
		GameRender();

		BeforeMouseX = NowMouseX;
		BeforeMouseY = NowMouseY;

		//オプション処理に移動
		//SetMousePoint(SCREEN_W / 2, SCREEN_H / 2);

		ScreenFlip();
		while (GetNowCount() - Time < 17) {}
		if (ProcessMessage())	break;
		if (CheckHitKey(KEY_INPUT_F4))	break;

	}
	GameExit();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}

//---------------------------------------------------------------------------------
//	キーが押された瞬間を取得する
//---------------------------------------------------------------------------------
bool PushHitKey(int key)
{
	if (KeyFrame[key] == 1) {
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	マウスが押されているかを取得する
//---------------------------------------------------------------------------------
bool CheckMouseInput(int button)
{
	if (GetMouseInput() & button) {
		return true;
	}
	return false;
}
//---------------------------------------------------------------------------------
//	マウスが押された瞬間を取得する
//---------------------------------------------------------------------------------
bool PushMouseInput(int button)
{
	if (button & MOUSE_INPUT_LEFT) {
		if (MouseLeftFrame == 1) {
			return true;
		}
	}
	if (button & MOUSE_INPUT_RIGHT) {
		if (MouseRightFrame == 1) {
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------------
//	マウスの座標を取得する
//---------------------------------------------------------------------------------
int GetMouseX()
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_x;
}
int GetMouseY()
{
	int mouse_x;
	int mouse_y;
	GetMousePoint(&mouse_x, &mouse_y);
	return mouse_y;
}

int GetCenterMouseMoveX()
{
	int mov_x = GetMouseX() - (SCREEN_W / 2);
	return mov_x;
}
int GetCenterMouseMoveY()
{
	int mov_y = GetMouseY() - (SCREEN_H / 2);
	return mov_y;
}

int GetCenterMouseMoveY();
//---------------------------------------------------------------------------------
//	マウスの移動量を取得する
//---------------------------------------------------------------------------------
int GetMouseMoveX()
{
	return NowMouseX - BeforeMouseX;
}
int GetMouseMoveY()
{
	return NowMouseY - BeforeMouseY;
}
//---------------------------------------------------------------------------------
//	度をラジアンに変換する関数
//---------------------------------------------------------------------------------
float TO_RADIAN(float degree)
{
	return degree * 3.14159265f / 180.0f;
}
//---------------------------------------------------------------------------------
//	ラジアンを度に変換する関数
//---------------------------------------------------------------------------------
float TO_DEGREE(float radian)
{
	return radian * 180.0f / 3.14159265f;
}

//----------------------------------------------------------------------------------
// ロード画像の大きさを変える(２倍)
//----------------------------------------------------------------------------------
int MyLoadGraph(const char* FileName) {
	int handle = LoadGraph(FileName);//画像をロード
	if (handle != -1) {//画像のロードに成功した場合
		int SizeX, SizeY;//画像サイズを格納するための変数を用意
		GetGraphSize(handle, &SizeX, &SizeY);//ロードした画像のサイズ取得
		//if ((SizeX >= 10000) && (SizeY >= 10000)) {//画像サイズで条件分岐
		int Nowscreen = GetDrawScreen();//現在の描画対象画面を一時保存
		SizeX *= 2;//二倍サイズにしたいので
		SizeY *= 2;
		int minihandle = MakeScreen(SizeX, SizeY, TRUE);//２倍サイズのグラフィックハンドルを作成
		if (minihandle == -1) {//ハンドル作成に失敗した場合
			DeleteGraph(handle);//メモリリークしないように、本来のサイズのグラフィックハンドルを削除※return handle;にすれば本来のサイズのまま返すこともできます
			return -1;//エラー発生を示す-1を返す
		}
		SetDrawScreen(minihandle);//描画対象画面を先ほど作った二倍サイズのグラフィックハンドルに変更
		DrawExtendGraph(0, 0, SizeX, SizeY, handle, TRUE);//ロードした画像を二倍サイズで描画
		DeleteGraph(handle);//不要になった本来のサイズのグラフィックハンドルを削除
		SetDrawScreen(Nowscreen);//現在の描画対象画面を元の画面に戻す
		SetCameraNearFar(0.1f, 3000.0f);	// カメラの Nearクリップ面と Farクリップ面の距離を設定
		SetupCamera_Perspective(TO_RADIAN(45.0f));	// 遠近法カメラをセットアップする
		SetCameraPositionAndTarget_UpVecY(VGet(0.0f, 10.0f, -10.0f), VGet(0.0f, 0.0f, 0.0f));
		return minihandle;//縮めた画像のグラフィックハンドルを返す
		//}
		//else {
		//	return handle;//指定サイズより大きくないのでそのままのグラフィックハンドルを返す
		//}
	}
	else {//画像のロードに失敗した場合
		return -1;//エラー発生を示す-1を返す
	}

}

void Natura_numbers(float *num)
{
	if (*num < 0) {
		*num *= -1;
	}
}