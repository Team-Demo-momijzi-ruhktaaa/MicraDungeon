class PlayerManager
{
public:
	//player�̈ړ����@��ݒ�p
	enum PlayerAngleMode { PLANE, MOUSEPLANE };
	Mesh player;
	bool moveFlag = true;//��O�ɗ����ă��Z�b�g�����������Ɉړ��ł��Ȃ��悤�ɂ���
	PlayerManager()
	{

	}

	PlayerManager(PlayerAngleMode mode,Texture* tex)
	{
		//�v���C���[���ǂ������ړ������邩�i�L�[�ړ��Ȃ̂��@�}�E�X�ړ��Ȃ̂��j
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

	//player�̈ړ������J�����̈ړ������ɂȂ邽�߃J�����𓮂����Ă���player��mesh�ɑ��
	//position�̈ړ��iWASD�j
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
	//player���{�b�N�X�ɓ������Ă������̉����o��
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
	//angles�̈ړ�(ten�L�[ or Mouse)
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
				//�{�b�N�X�ɂQ�̉�]������Ƃ��������̂ŃJ���������ύX���ďc�̊p�x�ύX
				//�����K�{
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
	//�J�����̌����Ă�������Ɉړ����邽�߂̌v�Z
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
		//4.0f�́A��������Ȃ��A������
		camera.position = player.position + advance * 4.0f;
	}
	void CameraMoveSide()
	{
		SetCameraDirection();
		camera.position = player.position + side * 4.0f;
	}
	//���݂̌����Ă��������ݒ�@player
	void SetPlayerDirection()
	{
		switch (mode)
		{
			case PLANE:
				advance = Float3(//�����̐��l�͓�@�B��̐���������
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
				advance = Float3(//+90�ɂ��Ă͏�����Y����direction���E�������Ă��邽�ߐ��ʂɖ߂�
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

	//���݂̌����Ă��������ݒ�@camera ���݁@x���̊p�x�ύX���J�����݂̂ɒǉ�
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

				side = Float3(//90�Ӗ��킩���
					cos(DirectX::XMConvertToRadians(-player.angles.y - 90)),
					sin(DirectX::XMConvertToRadians(-camera.angles.x)),
					sin(DirectX::XMConvertToRadians(-player.angles.y - 90))
				);
				break;
			case MOUSEPLANE:
				advance = Float3(//+90�ɂ��Ă͏�����Y����direction���E�������Ă��邽�ߐ��ʂɖ߂�
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

	//�W�����v������
	void JumpPlay()
	{
		//�X�y�[�X�L�[���������Ƃ��W�����v
		if (App::GetKeyDown(VK_SPACE) && playerGCount < 0)
		{
			playerGCount = 0.0f;
			playerSpeed = Float3(0.0f, jumpSpeed, 0.0f);
			player.position += playerSpeed;
			player.SetOBBData();
		}
	}
	//�d�͏���
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
	//�{�b�N�X�ɂԂ���������
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

	Float3 playerTestPos = Float3(0.0f, 0.0f, 0.0f);//��t���[���ړ��ɂ������Ɉړ��ł��邩������
	Float3 playerTestAngles = Float3(0.0f, 0.0f, 0.0f);//��ƍ��킹�Ďg�p�ړ��Ɗp�x�����������ď��߂Ĉړ�


	float moveSpeed = 0.1f;
	float angleSpeed = 1.0f;
	float playerGCount = 0.0f;//������  0.0f�`
							  //�Î~�� -1.0f
							  //�������ƐÎ~�����t���O�Ŕ��肵�Ă���������

	const float gSpeed = 0.0098f;//�d�͉����x  �K���Ȃ̂Ō�ŏC��
	const float jumpSpeed = 0.1f;//�W�����v���̏���  ����
	Float3 playerSpeed = {};//�P�t���[�����̃v���C���[�̈ړ�

	Float3 advance;	//�O��
	Float3 side;	//���E
};