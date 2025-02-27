#pragma once

//---------------------------------------------------------------------------------
//	Vector2 クラス
//---------------------------------------------------------------------------------
class Vector2
{
public:
	float x;
	float y;

	Vector2();									//	コンストラクタ
	Vector2( float x, float y );
	void clear();								//	ゼロ初期化
	void set( float x, float y );				//	値のセット
	void set( Vector2 &v );

	float GetLength();							//	長さ取得
	void normalize();							//	正規化
	void SetLength( float length );				//	長さ設定

	Vector2& operator = ( const Vector2 &v );	//	= 演算子のオーバーロード

	Vector2& operator += ( const Vector2 &v );	//	+= 演算子のオーバーロード
	Vector2& operator -= ( const Vector2 &v );	//	-= 演算子のオーバーロード
	Vector2& operator *= ( const float f );		//	*= 演算子のオーバーロード
	Vector2& operator /= ( const float f );		//	/= 演算子のオーバーロード
};

//	+ 演算子のオーバーロード
Vector2 operator + ( const Vector2 &v0, const Vector2 &v1 );
//	- 演算子のオーバーロード
Vector2 operator - ( const Vector2 &v0, const Vector2 &v1 );
//	* 演算子のオーバーロード
Vector2 operator * ( const Vector2 &v, const float f );
//	/ 演算子のオーバーロード
Vector2 operator / ( const Vector2 &v, const float f );

//	２つの Vector2 の距離を求める
float GetVector2Distance( Vector2 &p0, Vector2 &p1 );
//	２つの Vector2 の内積を求める
float GetVector2Dot( Vector2 &v0, Vector2 &v1 );
//	２つの Vector2 の外積を求める
float GetVector2Cross( Vector2 &v0, Vector2 &v1 );
