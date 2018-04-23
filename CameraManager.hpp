class CameraManager
{
public:
	enum CameraMoveMode { PLANE, SOLID, MOUSEPLANE, MOUSESOLID };

	CameraManager()
	{
		App::Initialize();
		Release();
	}
	//最初にこれ呼ぶべし
	void Release(CameraMoveMode mode = CameraMoveMode::PLANE)
	{
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
		this->mode = mode;
	}
	//カメラの操作関連------------------------------------------------------------------
	//カメラの向いている方向に移動するための行列計算だったはず
	void CameraMoveAdvance(float speed)
	{
		SetCameraDirection();
		camera.position = camera.position + advance * speed;
	}
	void CameraMoveSide(float speed)
	{
		SetCameraDirection();
		camera.position = camera.position + side * speed;
	}

	//カメラのアングルを引数ぶん移動
	void SetCameraAngle(Float3 angles)
	{
		camera.angles += angles;
		SetCameraDirection();
	}
	//現在の向いている方向を設定　最初にこれを呼ばないと移動不可能
	void SetCameraDirection()
	{
		switch (mode)
		{
			case PLANE:
				advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
					cos(DirectX::XMConvertToRadians(-camera.angles.y + 90)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-camera.angles.y + 90))
				);

				side = Float3(
					cos(DirectX::XMConvertToRadians(-camera.angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-camera.angles.y))
				);
				break;
				case SOLID:
				advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
					cos(DirectX::XMConvertToRadians(-camera.angles.y + 90)) * cos(DirectX::XMConvertToRadians(-camera.angles.x)),
					sin(DirectX::XMConvertToRadians(-camera.angles.x)),
					sin(DirectX::XMConvertToRadians(-camera.angles.y + 90)) * cos(DirectX::XMConvertToRadians(-camera.angles.x))
				);

				side = Float3(
					cos(DirectX::XMConvertToRadians(-camera.angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-camera.angles.y))
				);
				break;
				case MOUSEPLANE:
					advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
						cos(DirectX::XMConvertToRadians(-camera.angles.y + 90)),
						0.0f,
						sin(DirectX::XMConvertToRadians(-camera.angles.y + 90))
					);

					side = Float3(
						cos(DirectX::XMConvertToRadians(-camera.angles.y)),
						0.0f,
						sin(DirectX::XMConvertToRadians(-camera.angles.y))
					);
					break;
				case MOUSESOLID:
					advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
						cos(DirectX::XMConvertToRadians(-camera.angles.y + 90)) * cos(DirectX::XMConvertToRadians(-camera.angles.x)),
						sin(DirectX::XMConvertToRadians(-camera.angles.x)),
						sin(DirectX::XMConvertToRadians(-camera.angles.y + 90)) * cos(DirectX::XMConvertToRadians(-camera.angles.x))
					);

					side = Float3(
						cos(DirectX::XMConvertToRadians(-camera.angles.y)),
						0.0f,
						sin(DirectX::XMConvertToRadians(-camera.angles.y))
					);
					break;
			default:
				break;
		}
	}
	Camera GetCameraData()
	{
		return camera;
	}
	//現在のカメラの移動方法を取得
	CameraMoveMode GetCameraMode()
	{
		return mode;
	}
	void Update()
	{
		camera.Update();
	}

private:
	Camera camera;
	//平面的挙動（x,z）か立体的挙動か(x,y,z)か またマウスで動かすかどうか
	CameraMoveMode mode;
	//カメラを向いている方向に移動させる時に使用する変数
	Float3 advance;	//前後
	Float3 side;	//左右
	
};