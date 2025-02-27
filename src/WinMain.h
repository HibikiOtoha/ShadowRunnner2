#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <time.h>
#include <math.h>

#pragma warning (disable : 4819)

#include <d3d9.h>
//#include <d3dx9.h>

#include <DxLib.h>

bool PushHitKey(int key);
bool CheckMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
bool PushMouseInput(int button = MOUSE_INPUT_LEFT | MOUSE_INPUT_RIGHT);
int GetMouseX();
int GetMouseY();
int GetMouseMoveX();
int GetMouseMoveY();
int GetCenterMouseMoveX();
int GetCenterMouseMoveY();
float TO_RADIAN(float degree);
float TO_DEGREE(float radian);
int MyLoadGraph(const char* FileName);
void Natura_numbers(float *num);

//--------------------------------------------------------------
// ���w�萔
//--------------------------------------------------------------
static constexpr float PI = 3.141592653589793f;    //!< �~���� ��
static constexpr float TAU = 2.0f * PI;             //!< �~������2�{ ��
static constexpr float RadToDeg = 57.29577951f;          //!< Radian��Degree �ϊ��W��
static constexpr float DegToRad = 0.017453293f;          //!< Degree��Radian �ϊ��W��



//void DrawCircle3D_XZ(Vector3 center, float radius, int color, bool fill = false);