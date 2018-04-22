//参考にしたサイト
//http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
//有向境界ボックス
//OBBで当たり判定をとるのに必要なデータ…中心、各軸の方向ベクトル、軸方向の長さ

//当たり判定のクラス
class OBB
{
public:
	//有向境界ボックスで使用するデータ
	//struct OBBData
	//{
	//	//ボックスの中心座標
	//	Float3 OBBpos;
	//	//ボックスの方向ベクトル
	//	Float3 OBBvector[3];
	//	//ボックスの軸方向の長さ
	//	float OBBlength[3];
	//};

	//コンストラクタの中でオブジェクトのデータの中に作ったOBBようのデータを読み出す
	OBB()
	{
		App::Initialize();
	}

	~OBB()
	{

	}


	//ベクトルの単位ベクトル化
	Float3 Float3Normalize(float x, float y, float z)
	{
		int num = 0;
		num = 1 / sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
		return Float3(x, y, z)*num;
	}

	//二つのベクトルの外積の計算
	Float3 Float3Cross(Float3 f1, Float3 f2)
	{
		Float3 f;
		f.x = f1.z*f2.y - f1.y*f2.z;
		f.y = f1.x*f2.z - f1.z*f2.x;
		f.z = f1.x*f2.y - f1.y*f2.x;

		return f;
	}

	//内積計算
	float Float3Dot(Float3 f1, Float3 f2)
	{
		return f1.x*f2.x + f1.y * f2.y + f1.z*f2.z;
	}

	float Float3Length(Float3 f1)
	{
		return sqrt(pow(f1.x, 2.0f) + pow(f1.y, 2.0f) + pow(f1.z, 2.0f));
	}

	//衝突判定取得
	bool OBBCheck(Mesh::OBBData obb1, Mesh::OBBData obb2)
	{
		Mesh::OBBData obb[2];
		obb[0] = obb1;
		obb[1] = obb2;
		Float3 L = obb[0].OBBpos - obb[1].OBBpos;
		float length = 0;
		float obbLength[2];
		Float3 cross;
		Float3 Ae[3];
		Float3 Be[3];
		for (int i = 0; i < 3; i++)
		{
			Ae[i] = obb[0].OBBvector[i] * obb[0].OBBlength[i] * 0.5f;
			Be[i] = obb[1].OBBvector[i] * obb[1].OBBlength[i] * 0.5f;
		}

		//obb1のx方向
		obbLength[0] = obb[0].OBBlength[0] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[0], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[0], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb1のy方向
		obbLength[0] = obb[0].OBBlength[1] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[1], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[1], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb1のz方向
		obbLength[0] = obb[0].OBBlength[2] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[2], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[2], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb2のx方向
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[0], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[0] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[0], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb2のy方向
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[1], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[1] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[1], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		//obb2のz方向
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[2], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[2] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[2], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//衝突していない
			return false;
		}

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				cross = Float3Cross(obb[0].OBBvector[i], obb[1].OBBvector[j]);

				obbLength[0] = LenSegOnSeparateAxis(&cross, &Ae[(i + 1) % 3], &Ae[(i + 2) % 3]);
				obbLength[1] = LenSegOnSeparateAxis(&cross, &Be[(j + 1) % 3], &Be[(j + 2) % 3]);
				length = fabs(Float3Dot(cross, L));
				if (length > obbLength[0] + obbLength[1])
				{
					//衝突していない
					return false;
				}
			}
		}
		return true;
	}

	
	// 分離軸に投影された軸成分から投影線分長を算出
	FLOAT LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3 = 0)
	{
		// 3つの内積の絶対値の和で投影線分長を計算
		// 分離軸Sepは標準化されていること
		FLOAT r1 = fabs(Float3Dot(*Sep, *e1));
		FLOAT r2 = fabs(Float3Dot(*Sep, *e2));
		FLOAT r3 = e3 ? (fabs(Float3Dot(*Sep, *e3))) : 0;
		return r1 + r2 + r3;
	}
};