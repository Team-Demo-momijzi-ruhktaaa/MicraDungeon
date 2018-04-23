#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Material�̎g����
	Material material(L"Shader.hlsl");
	Drow�̑O��material.Attach();
	*/	
	float num = 0.0f;

	bool playFlag = true;

	//texture��uv��ݒ肷�邽�߂̂���
	struct SetUvData
	{
		Float2 uvData[6] = {};

		//�O�ʓ���uv�ɐݒ�
		void SetAll(Float2 numUV)
		{
			for (int i = 0; i < 6; i++)
			{
				uvData[i] = numUV;
			}
		}
		//��Ɖ��ȊO��uv�ݒ�
		void SetAround(Float2 numUV)
		{
			for (int i = 0; i < 4; i++)
			{
				uvData[i] = numUV;
			}
		}
		//�O�ʂ�uv�ݒ�
		void SetFront(Float2 numUV)
		{
			uvData[0] = numUV;
		}
		//��ʂ�uv�ݒ�
		void SetBack(Float2 numUV)
		{
			uvData[1] = numUV;
		}
		//���ʂ�uv�ݒ�
		void SetLeft(Float2 numUV)
		{
			uvData[2] = numUV;
		}
		//�O�ʂ�uv�ݒ�
		void SetRight(Float2 numUV)
		{
			uvData[3] = numUV;
		}
		//��ʂ�uv�ݒ�
		void SetUp(Float2 numUV)
		{
			uvData[4] = numUV;
		}
		//���ʂ�uv�ݒ�
		void SetDown(Float2 numUV)
		{
			uvData[5] = numUV;
		}
	};
	SetUvData setUvData;

	//�}�E�X�̏������W�̐ݒ�
	App::SetMousePosition(App::GetWindowSize().x / 2, App::GetWindowSize().y / 2);
	//�J�����̓��������̎w��@PLANE�ŕ��ʓI�@SOLID�ŗ��̓I
	Camera::CameraMoveMode cameraMode = Camera::CameraMoveMode::SOLID;
	//�J�����̍쐬
	Camera camera(cameraMode);
	camera.position = Float3(5.5f, 0.0f, -9.0f);
	camera.SetCameraDirection();

	//�e�N�X�`���̍쐬-----------------------------------------------------
	Texture textureBox(L"texture/TestTexture.jpg");
	textureBox.texUVData.SetDivide(Float2(4.0f, 2.0f));

	//------------------------------------------------------------------------
	
	
	Mesh box;
	setUvData.SetAll(Float2(0.0f, 0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box.CreateData(&textureBox, 1);

	Mesh box2;
	setUvData.SetAll(Float2(2.0f, 1.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box2.CreateData(&textureBox, 1);

	Mesh box3;
	setUvData.SetAll(Float2(2.0f, 0.0f));
	textureBox.texUVData.SetUVNum(setUvData.uvData);
	box3.CreateData(&textureBox, 1);



	box3.position.x = 2.0f;
	box2.position.y = 5.0f;
	box3.position.y = 5.0f;

	box.Draw();
	box2.Draw();

	box.SetOBBData();
	box2.SetOBBData();
	//�I���G���e�b�h�o�E���f�B���O�{�b�N�X�̔���p�̃f�[�^�쐬
	OBB obb;

	////�_�C���N�g�T�E���h�̃f�o�C�X�쐬
	//DirectSound* pDs = DirectSound::GetInstance();
	//pDs->Create(App::GetWindowHandle());

	//WaveFile waveFile;			//�����t�@�C���f�[�^
	//SoundBuffer soundBuffer;	//�Đ��p�o�b�t�@
	//if (waveFile.Load("music/BGM.wav"))
	//{
	//	soundBuffer.Create(waveFile);
	//	soundBuffer.Play(true);
	//}

	
	while (App::Refresh())
	{
		//�J�����̑���֘A
		{
			//�J�����̎��_��WASD�œ�����
			/*if (App::GetKey('W') || App::GetKey('A') || App::GetKey('S') || App::GetKey('D'))
			{
				if (App::GetKey('D'))
				{
					camera.angles.y += 1.0f;
				}
				else if (App::GetKey('A'))
				{
					camera.angles.y -= 1.0f;
				}
				if (App::GetKey('W'))
				{
					camera.angles.x -= 1.0f;
				}
				else if (App::GetKey('S'))
				{
					camera.angles.x += 1.0f;
				}
				camera.SetCameraDirection();
			}*/
			if (App::GetMousePosition().x < App::GetWindowSize().x / 4)
			{
				camera.angles.y -= 0.5f;
				camera.SetCameraDirection();
			}
			else if (App::GetMousePosition().x > App::GetWindowSize().x / 4 * 3 )
			{
				camera.angles.y += 0.5f;
				camera.SetCameraDirection();
			}
			//�J�����̍��W��WASD�œ������i��ŃJ����hpp�Ɋ֐����������j
			if (App::GetKey('W'))//�O�Ɉړ�
			{
				camera.CameraMoveAdvance(0.05f);
			}
			else if (App::GetKey('S'))//���Ɉړ�
			{
				camera.CameraMoveAdvance(-0.05f);
			}

			if (App::GetKey('D'))//�E�Ɉړ�
			{
				camera.CameraMoveSide(0.05f);
			}
			else if (App::GetKey('A'))//���Ɉړ�
			{
				camera.CameraMoveSide(-0.05f);
			}
			camera.Update();
		}
		/*if (App::GetKey('L'))
		{
			player.leftArm.angles.x += (float)(PI / 180 * 1);
		}
		if(App::GetKey('P'))
		{
			player.leftArm.angles.z -= (float)(PI / 180 * 10);
		}
		if (App::GetKeyDown('Z') && !flagPunch)
		{
			flagPunch = true;
		}
		
		if (flagPunch)
		{
			if (!flagReturn)
			{
				player.leftArm.position.z += 0.5f;
				if (player.leftArm.position.z > 50.0f)
				{
					flagReturn = true;
				}
			}
			else
			{
				player.leftArm.position.z -= 0.5f;
				if (player.leftArm.position.z <= 0.0f)
				{
					flagReturn = false;
					flagPunch = false;
				}
			}
		}

		player.Update();
		*/
		
		//���_�@��񂵂������@�C�ӂɃt���O��؂�ւ���K�v����
		//������obj�ƐڐG����Ƃ�ꍇ�̃t���O�����鎀��
		if (!obb.OBBCheck(box.GetOBBData(), box2.GetOBBData()))
		{
			box2.position.y -= 0.01f;
			box3.position.y -= 0.01f;

			box2.angles.x += 0.01f;
			box2.angles.z += 0.01f;

			box.SetOBBData();
			box2.SetOBBData();
			if (obb.OBBCheck(box.GetOBBData(), box2.GetOBBData()))
			{
				box2.position.y += 0.01f;
				box3.position.y += 0.01f;

				box2.angles.x -= 0.01f;
				box2.angles.z -= 0.01f;

				box.SetOBBData();
				box2.SetOBBData();
			}
		}
		box.Draw();
		box2.Draw();
		box3.Draw();

	}
	return 0;
}

