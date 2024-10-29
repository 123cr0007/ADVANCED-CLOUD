#include "calculation.h"

// radian→degree
float calculation::DegToRad(float deg) {
	return deg * PI / 180.0f;		// 角度をdegreeからradianに変換
}

// degree→radian
float calculation::RadToDeg(float rad) {
	return rad * 180.0f / PI;		// 角度をradianからdegreeに変換
}

VECTOR calculation::RadPositionVector(VECTOR position,float deg, float speed)
{
	float rad = RadToDeg(deg);

	//現在の角度でベクトルを作る
	VECTOR vDir = VGet(0, 0, 0);
	vDir.x = cos(rad);
	vDir.y = sin(rad);
	// 作ったベクトルを「単位ベクトル」（スカラが1のベクトル）にする
	VECTOR vUnit = VNorm(vDir);
	// 単位ベクトルに移動速度をかけた、移動ベクトルを作る
	VECTOR vMove = VScale(vUnit, speed);
	// 移動ベクトルを位置に加算する
	position = VAdd(position, vMove);
	return position;
}

VECTOR calculation::RadUnitVector(float deg, float scale)
{

	float rad = RadToDeg(deg);

	//現在の角度でベクトルを作る
	VECTOR vDir = VGet(0, 0, 0);
	vDir.x = cos(rad);
	vDir.y = sin(rad);
	// 作ったベクトルを「単位ベクトル」（スカラが1のベクトル）にする
	VECTOR vUnit = VNorm(vDir);
	if (scale > 0) {
		// 単位ベクトルのスケールを変える
		VECTOR vMove = VScale(vUnit, scale);
	}
	else{
		return VGet(0, 0, 0);
	}

}

VECTOR calculation::Rad3DPositionVector(VECTOR position, float deg_xy, float deg_zx, float speed)
{

	float rad_xy = DegToRad(deg_xy);
	float rad_zx = DegToRad(deg_zx);

	//現在の角度でベクトルを作る
	VECTOR vDir = VGet(0, 0, 0);
	

	VECTOR willpos = VScale(position, speed);
	
	float r = VSize(VSub(willpos,position));

	vDir.x = r * sin(rad_xy) * cos(rad_zx);
	vDir.y = r * cos(rad_xy);
	vDir.z = r * sin(rad_xy) * sin(rad_zx);
	
	// 作ったベクトルを「単位ベクトル」（スカラが1のベクトル）にする
	VECTOR vUnit = VNorm(vDir);

	// 単位ベクトルに移動速度をかけた、移動ベクトルを作る
	VECTOR vMove = VScale(vUnit, speed);
	 //移動ベクトルを位置に加算する
	position = VAdd(position, vMove);

	return	position;
}