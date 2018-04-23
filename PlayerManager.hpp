class PlayerManager
{
public:

	PlayerManager(CameraManager::CameraMoveMode mode = CameraManager::CameraMoveMode::PLANE)
	{
		//�v���C���[���ǂ������ړ������邩�i�L�[�ړ��Ȃ̂��@�}�E�X�ړ��Ȃ̂��j
		cMana.Release(mode);
	}
	~PlayerManager()
	{

	}
	//player�̈ړ������J�����̈ړ������ɂȂ邽�߃J�����𓮂����Ă���player��mesh�ɑ��
	//position�̈ړ��iWASD�j
	void PlayerMove(float speed)
	{
		if (App::GetKey('W'))//�O�Ɉړ��i���o�I�Ɂj
		{
			cMana.CameraMoveAdvance(speed);
		}
		else if (App::GetKey('S'))//���Ɉړ�
		{
			cMana.CameraMoveAdvance(-speed);
		}
		if (App::GetKey('A'))//���Ɉړ�
		{
			cMana.CameraMoveSide(-speed);
		}
		else if (App::GetKey('D'))//�E�Ɉړ�
		{
			cMana.CameraMoveSide(speed);
		}
	}

	//angles�̈ړ�(ten�L�[ or Mouse)
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
					cMana.SetCameraAngle(Float3(-speed, 0.0f, 0.0f));//X����O�ɉ�]
				}
				else if (App::GetKey(VK_DOWN))
				{
					cMana.SetCameraAngle(Float3(speed, 0.0f, 0.0f));//X�����ɉ�]
				}
				if (App::GetKey(VK_LEFT))
				{
					cMana.SetCameraAngle(Float3(0.0f, -speed, 0.0f));//Y������]
				}
				else if (App::GetKey(VK_RIGHT))
				{
					cMana.SetCameraAngle(Float3(0.0f, speed, 0.0f));//Y���E��]
				}
				break;
			case CameraManager::CameraMoveMode::MOUSEPLANE:
				if (App::GetMousePosition().x < App::GetWindowSize().x / 4)//Y������]
				{
					cMana.SetCameraAngle(Float3(0.0f, -speed, 0.0f));
				}
				else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3)//Y���E��]
				{
					cMana.SetCameraAngle(Float3(0.0f, speed, 0.0f));
				}
				break;
			case CameraManager::CameraMoveMode::MOUSESOLID:
				if (App::GetMousePosition().y < App::GetWindowSize().y / 4)//X����O�ɉ�]
				{
					cMana.SetCameraAngle(Float3(-speed, 0.0f, 0.0f));
				}
				else if (App::GetMousePosition().y > App::GetWindowSize().y / 4 * 3)//X�����ɉ�]
				{
					cMana.SetCameraAngle(Float3(speed, 0.0f, 0.0f));
				}
				if (App::GetMousePosition().x < App::GetWindowSize().x / 4)//Y���E��]
				{
					cMana.SetCameraAngle(Float3(0.0f, -speed, 0.0f));
				}
				else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3)//Y���E��]
				{
					cMana.SetCameraAngle(Float3(0.0f, speed, 0.0f));
				}
				break;
			default:

				break;
		}
	}

	//�{��angle������������Ƃ��낾��OBB�����݂��܂����肵�Ă��Ȃ�����position�̂�
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