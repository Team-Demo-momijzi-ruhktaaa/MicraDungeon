class PlayerManager
{
public:
	//player�̈ړ����@��ݒ�p
	enum PlayerAngleMode { PLANE, MOUSEPLANE };

	PlayerManager(PlayerAngleMode mode,Texture* tex)
	{
		//�v���C���[���ǂ������ړ������邩�i�L�[�ړ��Ȃ̂��@�}�E�X�ړ��Ȃ̂��j
		player.position.y = 0.5f;
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
		this->mode = mode;
		player.CreateData(tex, 1);
		SetPlayerDirection();
	}
	~PlayerManager()
	{

	}
	
	void PlayerBehavior(float angleSpeed,float moveSpeed)
	{
		PlayerAngles(angleSpeed);
		PlayerMove(moveSpeed);
		player.SetOBBData();
		Update();
	}
	//player�̈ړ������J�����̈ړ������ɂȂ邽�߃J�����𓮂����Ă���player��mesh�ɑ��
	//position�̈ړ��iWASD�j
	void PlayerMove(float speed)
	{
		if (App::GetKey('W'))
		{
			PlayerMoveAdvance(speed);
		}
		else if (App::GetKey('S'))
		{
			PlayerMoveAdvance(-speed);
		}
		if (App::GetKey('A'))
		{
			PlayerMoveSide(-speed);
		}
		else if (App::GetKey('D'))
		{
			PlayerMoveSide(speed);
		}
	}

	//angles�̈ړ�(ten�L�[ or Mouse)
	void PlayerAngles(float speed)
	{
		switch (mode)
		{
			case PlayerManager::PLANE:
				if (App::GetKey(VK_LEFT))
				{
					player.angles.y -= speed;
					camera.angles.y -= speed;
				}
				else if (App::GetKey(VK_RIGHT))
				{
					player.angles.y += speed;
					camera.angles.y += speed;
				}
				//�{�b�N�X�ɂQ�̉�]������Ƃ��������̂ŃJ���������ύX���ďc�̊p�x�ύX
				//�����K�{
				if (App::GetKey(VK_UP))
				{
					if (camera.angles.x - speed >= -90.0f)
						camera.angles.x -= speed;
					else
						camera.angles.x = -90.0f;

				}
				else if (App::GetKey(VK_DOWN))
				{
					if (camera.angles.x + speed <= 0.0f)
						camera.angles.x += speed;
					else
						camera.angles.x = 0.0f;
				}
				break;
			case PlayerManager::MOUSEPLANE:
				if (App::GetMousePosition().x < App::GetWindowSize().x / 4)
				{
					player.angles.y -= speed;
					camera.angles.y -= speed;
				}
				else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3)
				{
					player.angles.y += speed;
					camera.angles.y += speed;
				}
				break;
			default:
				break;
		}
	}
	//�J�����̌����Ă�������Ɉړ����邽�߂̌v�Z
	void PlayerMoveAdvance(float speed)
	{
		SetPlayerDirection();
		player.position = player.position + advance * speed;
	}
	void PlayerMoveSide(float speed)
	{
		SetPlayerDirection();
		player.position = player.position + side * speed;
	}

	void CameraMoveAdvance()
	{
		SetCameraDirection();
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


	void Update()
	{
		CameraMoveAdvance();
		CameraMoveSide();
		camera.Update(player.position);
		player.Draw();
	}
private:
	Mesh player;
	Camera camera;
	PlayerAngleMode mode;

	Float3 playerTestPos = Float3(0.0f, 0.0f, 0.0f);//��t���[���ړ��ɂ������Ɉړ��ł��邩������
	Float3 playerTestAngles = Float3(0.0f, 0.0f, 0.0f);//��ƍ��킹�Ďg�p�ړ��Ɗp�x�����������ď��߂Ĉړ�

	bool jumpFlag = false;
	float playerGCount = 0.0f;//������  0.0f�`
							  //�Î~�� -1.0f
							  //�������ƐÎ~�����t���O�Ŕ��肵�Ă���������

	const float gSpeed = 0.0098f;//�d�͉����x  �K���Ȃ̂Ō�ŏC��
	const float jumpSpeed = 0.1f;//�W�����v���̏���  ����
	Float3 playerSpeed = {};//���݂̃v���C���[�̃X�s�[�h

	Float3 advance;	//�O��
	Float3 side;	//���E
};