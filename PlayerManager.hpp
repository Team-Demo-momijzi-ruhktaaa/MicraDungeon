class PlayerManager
{
public:
	//playerの移動方法を設定用
	enum PlayerAngleMode { PLANE, MOUSEPLANE };
	Mesh player;
	bool moveFlag = true;//場外に落ちてリセットかかった時に移動できないようにする
	PlayerManager()
	{

	}

	PlayerManager(PlayerAngleMode mode,Texture* tex)
	{
		//プレイヤーがどういう移動をするか（キー移動なのか　マウス移動なのか）
		ResetPos();
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
		this->mode = mode;
		player.CreateData(tex, 1);
		SetPlayerDirection();
	}
	~PlayerManager()
	{

	}
	
	Float3 GetPlayerSpeed()
	{
		return playerSpeed;
	}
	void ResetPos()
	{
		moveFlag = false;
		playerGCount = 0.0f;
		playerSpeed = Float3(0.0f, 0.0f, 0.0f);
		player.angles = Float3(0.0f, 0.0f, 0.0f);
		camera.angles.x = -45.0f;
		player.position = Float3(0.0f, 5.0f, 0.0f);
	}

	//playerの移動だがカメラの移動が軸になるためカメラを動かしてからplayerのmeshに代入
	//positionの移動（WASD）
	void PlayerMove()
	{
		if (App::GetKey('W'))
		{
			PlayerMoveAdvance(moveSpeed);
		}
		else if (App::GetKey('S'))
		{
			PlayerMoveAdvance(-moveSpeed);
		}
		if (App::GetKey('A'))
		{
			PlayerMoveSide(-moveSpeed);
		}
		else if (App::GetKey('D'))
		{
			PlayerMoveSide(moveSpeed);
		}
	}
	//playerがボックスに当たっていた時の押し出し
	void PlayerRemove()
	{
		if (App::GetKey('W'))
		{
			PlayerMoveAdvance(-moveSpeed);
		}
		else if (App::GetKey('S'))
		{
			PlayerMoveAdvance(moveSpeed);
		}
		if (App::GetKey('A'))
		{
			PlayerMoveSide(moveSpeed);
		}
		else if (App::GetKey('D'))
		{
			PlayerMoveSide(-moveSpeed);
		}
		Gravity();
	}
	//anglesの移動(tenキー or Mouse)
	void PlayerAngles()
	{
		switch (mode)
		{
			case PlayerManager::PLANE:
				if (App::GetKey(VK_LEFT))
				{
					player.angles.y -= angleSpeed;
					camera.angles.y -= angleSpeed;
				}
				else if (App::GetKey(VK_RIGHT))
				{
					player.angles.y += angleSpeed;
					camera.angles.y += angleSpeed;
				}
				//ボックスに２つの回転を入れるとおかしいのでカメラだけ変更して縦の角度変更
				//調整必須
				if (App::GetKey(VK_UP))
				{
					if (camera.angles.x - angleSpeed >= -90.0f)
						camera.angles.x -= angleSpeed;
					else
						camera.angles.x = -90.0f;

				}
				else if (App::GetKey(VK_DOWN))
				{
					if (camera.angles.x + angleSpeed <= 0.0f)
						camera.angles.x += angleSpeed;
					else
						camera.angles.x = 0.0f;
				}
				break;
			case PlayerManager::MOUSEPLANE:
				if (App::GetMousePosition().x < App::GetWindowSize().x / 4)
				{
					player.angles.y -= angleSpeed;
					camera.angles.y -= angleSpeed;
				}
				else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3)
				{
					player.angles.y += angleSpeed;
					camera.angles.y += angleSpeed;
				}
				break;
			default:
				break;
		}
	}
	//カメラの向いている方向に移動するための計算
	void PlayerMoveAdvance(float moveSpeed)
	{
		SetPlayerDirection();
		player.position = player.position + advance * moveSpeed;
	}
	void PlayerMoveSide(float moveSpeed)
	{
		SetPlayerDirection();
		player.position = player.position + side * moveSpeed;
	}

	void CameraMoveAdvance()
	{
		SetCameraDirection();
		//4.0fは、早さじゃない、距離だ
		camera.position = player.position + advance * 4.0f;
	}
	void CameraMoveSide()
	{
		SetCameraDirection();
		camera.position = player.position + side * 4.0f;
	}
	//現在の向いている方向を設定　player
	void SetPlayerDirection()
	{
		switch (mode)
		{
			case PLANE:
				advance = Float3(//ここの数値は謎　唯一の成功がこれ
					cos(DirectX::XMConvertToRadians(-player.angles.y + 90)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-player.angles.y + 90))
				);

				side = Float3(
					cos(DirectX::XMConvertToRadians(-player.angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-player.angles.y))
				);
				break;
			case MOUSEPLANE:
				advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
					cos(DirectX::XMConvertToRadians(-player.angles.y + 90)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-player.angles.y + 90))
				);

				side = Float3(
					cos(DirectX::XMConvertToRadians(-player.angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-player.angles.y))
				);
				break;
			default:
				break;
		}
	}

	//現在の向いている方向を設定　camera 現在　x軸の角度変更をカメラのみに追加
	void SetCameraDirection()
	{
		switch (mode)
		{
			case PLANE:
				advance = Float3(
					cos(DirectX::XMConvertToRadians(-player.angles.y)) * sin(DirectX::XMConvertToRadians(-camera.angles.x)),
					sin(DirectX::XMConvertToRadians(-camera.angles.x)),
					sin(DirectX::XMConvertToRadians(-player.angles.y)) * cos(DirectX::XMConvertToRadians(-camera.angles.x))
				);

				side = Float3(//90意味わからん
					cos(DirectX::XMConvertToRadians(-player.angles.y - 90)),
					sin(DirectX::XMConvertToRadians(-camera.angles.x)),
					sin(DirectX::XMConvertToRadians(-player.angles.y - 90))
				);
				break;
			case MOUSEPLANE:
				advance = Float3(//+90については初期のY軸のdirectionが右を向いているため正面に戻す
					cos(DirectX::XMConvertToRadians(-player.angles.y)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-player.angles.y))
				);

				side = Float3(
					cos(DirectX::XMConvertToRadians(-player.angles.y - 90)),
					0.0f,
					sin(DirectX::XMConvertToRadians(-player.angles.y - 90))
				);
				break;
			default:
				break;
		}
	}

	//ジャンプをする
	void JumpPlay()
	{
		//スペースキーを押したときジャンプ
		if (App::GetKeyDown(VK_SPACE) && playerGCount < 0)
		{
			playerGCount = 0.0f;
			playerSpeed = Float3(0.0f, jumpSpeed, 0.0f);
			player.position += playerSpeed;
			player.SetOBBData();
		}
	}
	//重力処理
	void Gravity()
	{
		if (playerGCount >= 0)
		{
			if (gSpeed*playerGCount < 0.05f)
			{
				playerSpeed.y -= gSpeed*playerGCount;
			}
			else
			{
				playerSpeed.y -= 0.05f;
			}
		}
		else
		{
			playerGCount = 0.0f;
		}

		playerGCount += 0.01f;

		player.position += playerSpeed;

		if (player.position.y < -10.0f)
		{
			ResetPos();
		}
	}
	//ボックスにぶつかった時に
	void GravityReset(Mesh* box)
	{
		if(!moveFlag)
		moveFlag = true;
		player.position.y = player.scale.y / 2 + box->scale.y / 2 + box->position.y;
		playerGCount = -1.0f;
		playerSpeed = Float3(0.0f, 0.0f, 0.0f);
	}


	void Update()
	{
		CameraMoveAdvance();
		CameraMoveSide();
		camera.Update(player.position);
		player.Draw();
	}
private:
	Camera camera;
	PlayerAngleMode mode;

	Float3 playerTestPos = Float3(0.0f, 0.0f, 0.0f);//一フレーム移動につきそこに移動できるかを示す
	Float3 playerTestAngles = Float3(0.0f, 0.0f, 0.0f);//上と合わせて使用移動と角度両方が生きて初めて移動


	float moveSpeed = 0.1f;
	float angleSpeed = 1.0f;
	float playerGCount = 0.0f;//落下時  0.0f～
							  //静止時 -1.0f
							  //落下時と静止時をフラグで判定してもいいかも

	const float gSpeed = 0.0098f;//重力加速度  適当なので後で修正
	const float jumpSpeed = 0.1f;//ジャンプ時の初速  同上
	Float3 playerSpeed = {};//１フレーム毎のプレイヤーの移動

	Float3 advance;	//前後
	Float3 side;	//左右
};