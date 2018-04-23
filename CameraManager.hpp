class CameraManager
{
public:
	enum CameraMoveMode { PLANE, SOLID, MOUSEPLANE, MOUSESOLID };

	CameraManager()
	{
		App::Initialize();
		Release();
	}
	//�ŏ��ɂ���ĂԂׂ�
	void Release(CameraMoveMode mode = CameraMoveMode::PLANE)
	{
		advance = Float3(0.0f, 0.0f, 0.0f);
		side = Float3(0.0f, 0.0f, 0.0f);
		this->mode = mode;
	}
	//�J�����̑���֘A------------------------------------------------------------------
	//�J�����̌����Ă�������Ɉړ����邽�߂̍s��v�Z�������͂�
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

	//�J�����̃A���O���������Ԃ�ړ�
	void SetCameraAngle(Float3 angles)
	{
		camera.angles += angles;
		SetCameraDirection();
	}
	//���݂̌����Ă��������ݒ�@�ŏ��ɂ�����Ă΂Ȃ��ƈړ��s�\
	void SetCameraDirection()
	{
		switch (mode)
		{
			case PLANE:
				advance = Float3(//+90�ɂ��Ă͏�����Y����direction���E�������Ă��邽�ߐ��ʂɖ߂�
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
				advance = Float3(//+90�ɂ��Ă͏�����Y����direction���E�������Ă��邽�ߐ��ʂɖ߂�
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
					advance = Float3(//+90�ɂ��Ă͏�����Y����direction���E�������Ă��邽�ߐ��ʂɖ߂�
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
					advance = Float3(//+90�ɂ��Ă͏�����Y����direction���E�������Ă��邽�ߐ��ʂɖ߂�
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
	//���݂̃J�����̈ړ����@���擾
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
	//���ʓI�����ix,z�j�����̓I������(x,y,z)�� �܂��}�E�X�œ��������ǂ���
	CameraMoveMode mode;
	//�J�����������Ă�������Ɉړ������鎞�Ɏg�p����ϐ�
	Float3 advance;	//�O��
	Float3 side;	//���E
	
};