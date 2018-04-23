class PlayerManager
{
public:

	PlayerManager(CameraManager::CameraMoveMode mode = CameraManager::CameraMoveMode::PLANE)
	{
		//プレイヤーがどういう移動をするか（キー移動なのか　マウス移動なのか）
		cMana.Release(mode);
	}
	~PlayerManager()
	{

	}
	//playerの移動だがカメラの移動が軸になるためカメラを動かしてからplayerのmeshに代入
	//positionの移動（WASD）
	void PlayerMove(float speed)
	{
		if (App::GetKey('W'))//前に移動（視覚的に）
		{
			cMana.CameraMoveAdvance(speed);
		}
		else if (App::GetKey('S'))//後ろに移動
		{
			cMana.CameraMoveAdvance(-speed);
		}
		if (App::GetKey('A'))//左に移動
		{
			cMana.CameraMoveSide(-speed);
		}
		else if (App::GetKey('D'))//右に移動
		{
			cMana.CameraMoveSide(speed);
		}
	}

	//anglesの移動(tenキー or Mouse)
	void PlayerAngles(float speed)
	{
		switch (cMana.GetCameraMode())
		{
			case CameraManager::CameraMoveMode::PLANE:
				if (App::GetKey(VK_LEFT))
				{
					cMana.SetCameraAngle(Float3(0.0f,-speed, 0.0f));
				}
				else if (App::GetKey(VK_RIGHT))
				{
					cMana.SetCameraAngle(Float3(0.0f,speed, 0.0f));
				}
				break;
			case CameraManager::CameraMoveMode::SOLID:
				if (App::GetKey(VK_UP))
				{
					cMana.SetCameraAngle(Float3(-speed, 0.0f, 0.0f));//X軸手前に回転
				}
				else if (App::GetKey(VK_DOWN))
				{
					cMana.SetCameraAngle(Float3(speed, 0.0f, 0.0f));//X軸奥に回転
				}
				if (App::GetKey(VK_LEFT))
				{
					cMana.SetCameraAngle(Float3(0.0f, -speed, 0.0f));//Y軸左回転
				}
				else if (App::GetKey(VK_RIGHT))
				{
					cMana.SetCameraAngle(Float3(0.0f, speed, 0.0f));//Y軸右回転
				}
				break;
			case CameraManager::CameraMoveMode::MOUSEPLANE:
				if (App::GetMousePosition().x < App::GetWindowSize().x / 4)//Y軸左回転
				{
					cMana.SetCameraAngle(Float3(0.0f, -speed, 0.0f));
				}
				else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3)//Y軸右回転
				{
					cMana.SetCameraAngle(Float3(0.0f, speed, 0.0f));
				}
				break;
			case CameraManager::CameraMoveMode::MOUSESOLID:
				if (App::GetMousePosition().y < App::GetWindowSize().y / 4)//X軸手前に回転
				{
					cMana.SetCameraAngle(Float3(-speed, 0.0f, 0.0f));
				}
				else if (App::GetMousePosition().y > App::GetWindowSize().y / 4 * 3)//X軸奥に回転
				{
					cMana.SetCameraAngle(Float3(speed, 0.0f, 0.0f));
				}
				if (App::GetMousePosition().x < App::GetWindowSize().x / 4)//Y軸右回転
				{
					cMana.SetCameraAngle(Float3(0.0f, -speed, 0.0f));
				}
				else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3)//Y軸右回転
				{
					cMana.SetCameraAngle(Float3(0.0f, speed, 0.0f));
				}
				break;
			default:

				break;
		}
	}

	//本来angleも代入したいところだがOBBが現在うまく判定していないためpositionのみ
	void SetPlayerMesh()
	{
		player.position = cMana.GetCameraData().position;
	}

	void Update()
	{
		cMana.Update();
	}
private:
	CameraManager cMana;
	Mesh player;

};