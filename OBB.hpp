//�Q�l�ɂ����T�C�g
//http://marupeke296.com/COL_3D_No13_OBBvsOBB.html
//�L�����E�{�b�N�X
//OBB�œ����蔻����Ƃ�̂ɕK�v�ȃf�[�^�c���S�A�e���̕����x�N�g���A�������̒���

//�����蔻��̃N���X
class OBB
{
public:
	//�L�����E�{�b�N�X�Ŏg�p����f�[�^
	//struct OBBData
	//{
	//	//�{�b�N�X�̒��S���W
	//	Float3 OBBpos;
	//	//�{�b�N�X�̕����x�N�g��
	//	Float3 OBBvector[3];
	//	//�{�b�N�X�̎������̒���
	//	float OBBlength[3];
	//};

	//�R���X�g���N�^�̒��ŃI�u�W�F�N�g�̃f�[�^�̒��ɍ����OBB�悤�̃f�[�^��ǂݏo��
	OBB()
	{
		App::Initialize();
	}

	~OBB()
	{

	}


	//�x�N�g���̒P�ʃx�N�g����
	Float3 Float3Normalize(float x, float y, float z)
	{
		int num = 0;
		num = 1 / sqrt(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0));
		return Float3(x, y, z)*num;
	}

	//��̃x�N�g���̊O�ς̌v�Z
	Float3 Float3Cross(Float3 f1, Float3 f2)
	{
		Float3 f;
		f.x = f1.z*f2.y - f1.y*f2.z;
		f.y = f1.x*f2.z - f1.z*f2.x;
		f.z = f1.x*f2.y - f1.y*f2.x;

		return f;
	}

	//���όv�Z
	float Float3Dot(Float3 f1, Float3 f2)
	{
		return f1.x*f2.x + f1.y * f2.y + f1.z*f2.z;
	}

	float Float3Length(Float3 f1)
	{
		return sqrt(pow(f1.x, 2.0f) + pow(f1.y, 2.0f) + pow(f1.z, 2.0f));
	}

	//�Փ˔���擾
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

		//obb1��x����
		obbLength[0] = obb[0].OBBlength[0] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[0], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[0], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb1��y����
		obbLength[0] = obb[0].OBBlength[1] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[1], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[1], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb1��z����
		obbLength[0] = obb[0].OBBlength[2] * 0.5f;
		obbLength[1] = LenSegOnSeparateAxis(&obb[0].OBBvector[2], &Be[0], &Be[1], &Be[2]);
		length = Float3Dot(obb[0].OBBvector[2], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb2��x����
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[0], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[0] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[0], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb2��y����
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[1], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[1] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[1], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
			return false;
		}

		//obb2��z����
		obbLength[0] = LenSegOnSeparateAxis(&obb[1].OBBvector[2], &Ae[0], &Ae[1], &Ae[2]);
		obbLength[1] = obb[1].OBBlength[2] * 0.5f;
		length = Float3Dot(obb[1].OBBvector[2], L);
		if (length > obbLength[0] + obbLength[1])
		{
			//�Փ˂��Ă��Ȃ�
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
					//�Փ˂��Ă��Ȃ�
					return false;
				}
			}
		}
		return true;
	}

	
	// �������ɓ��e���ꂽ���������瓊�e���������Z�o
	FLOAT LenSegOnSeparateAxis(Float3 *Sep, Float3 *e1, Float3 *e2, Float3 *e3 = 0)
	{
		// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
		// ������Sep�͕W��������Ă��邱��
		FLOAT r1 = fabs(Float3Dot(*Sep, *e1));
		FLOAT r2 = fabs(Float3Dot(*Sep, *e2));
		FLOAT r3 = e3 ? (fabs(Float3Dot(*Sep, *e3))) : 0;
		return r1 + r2 + r3;
	}
};