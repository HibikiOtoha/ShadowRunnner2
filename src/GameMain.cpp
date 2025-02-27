
#include "WinMain.h"
#include <iostream>
using namespace std;
#include "DxLib.h"
#include "Vector2.h"
#include "Vector3.h"
#include "GameMain.h"
//シーン
#include "Scene/Title.h"
#include "Scene/Game.h"
#include "Scene/End.h"

#include "Option/Option.h"



Title title;
Game game;
Ending ending;

Option option;

//大きなシーンの種類
enum
{
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_END
};
//今のシーン番号入れ
int now_scene;

int stage_model;
int sky_model;
int player_model;

int Sound;

void GameInit()
{

	stage_model = MV1LoadModel("data/Stage/blender/stage6.mv1");
	sky_model = MV1LoadModel("data/SkyBox/sky.mv1");
	player_model = MV1LoadModel("data/Player/Bot.mv1");
	//player_model = MV1LoadModel("data/movie/model/MovieBot.mv1");
	title.Init();

	game.Init(stage_model, sky_model , player_model);
	ending.Init(player_model);

	SetMouseDispFlag(FALSE);	//マウスカーソルの表示(全画面)

	//タイトルからスタート
	now_scene = SCENE_TITLE;
}
void GameUpdate()
{
	switch (now_scene)
	{
	case SCENE_TITLE:
		if (!option.OptionMode_ON) {
			title.Update(Sound);
		}
		if (title.Scene_Change)
		{
			game.Reset();
			now_scene = SCENE_PLAY;
		}

		option.Update(SCENE_TITLE, Sound);

		break;

	case SCENE_PLAY:

		if (!option.OptionMode_ON) {
			game.Update(option.tutorial_on,Sound,option.Sens_vol);
		}
		if (game.Scene_Change)
		{
			ending.Init(player_model);
			now_scene = SCENE_END;
		}
		
		option.Update(SCENE_TITLE, Sound);
		
		break;

	case SCENE_END:
		ending.Update(game.Game_Clear);
		if (ending.Scene_Change)
		{
			title.Init();
			now_scene = SCENE_TITLE;
		}
		break;

	default:
		break;
	}
}
void GameRender()
{
	switch (now_scene)
	{
	case SCENE_TITLE:
		title.Render();
		break;

	case SCENE_PLAY:
		game.Render();
		break;

	case SCENE_END:
		ending.Render();
		break;


	default:
		break;
	}

	option.Draw();
}

void GameExit()
{
	title.Exit();
	game.Exit();
	ending.Exit();

	option.Exit();

	MV1DeleteModel(player_model);
	MV1DeleteModel(stage_model);
	MV1DeleteModel(sky_model);
}