#include"App.hpp"

using namespace std;
using namespace DirectX;

int MAIN()
{
	/*Materialの使い方
	Material material(L"Shader.hlsl");
	Drowの前にmaterial.Attach();
	*/	
	float num = 0.0f;

	bool playFlag = true;

	//textureのuvを設定するためのもの
	struct SetUvData
	{
		Float2 uvData[6] = {};

		//前面同じuvに設定
		void SetAll(Float2 numUV)
		{
			for (int i = 0; i < 6; i++)
			{
				uvData[i] = numUV;
			}
		}
		//上と下以外のuv設定
		void SetAround(Float2 numUV)
		{
			for (int i = 0; i < 4; i++)
			{
				uvData[i] = numUV;
			}
		}
		//前面のuv設定
		void SetFront(Float2 numUV)
		{
			uvData[0] = numUV;
		}
		//後面のuv設定
		void SetBack(Float2 numUV)
		{
			uvData[1] = numUV;
		}
		//左面のuv設定
		void SetLeft(Float2 numUV)
		{
			uvData[2] = numUV;
		}
		//前面のuv設定
		void SetRight(Float2 numUV)
		{
			uvData[3] = numUV;
		}
		//上面のuv設定
		void SetUp(Float2 numUV)
		{
			uvData[4] = numUV;
		}
		//下面のuv設定
		void SetDown(Float2 numUV)
		{
			uvData[5] = numUV;
		}
	};
	SetUvData setUvData;

	//マウスの初期座標の設定
	App::SetMousePosition(App::GetWindowSize().x / 2, App::GetWindowSize().y / 2);
	//カメラの動かし方の指定　PLANEで平面的　SOLIDで立体的
	Camera::CameraMoveMode cameraMode = Camera::CameraMoveMode::SOLID;
	//カメラの作成
	Camera camera(cameraMode);
	camera.position = Float3(5.5f, 0.0f, -9.0f);
	camera.SetCameraDirection();

	//テクスチャの作成-----------------------------------------------------
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
	//オリエンテッドバウンディングボックスの判定用のデータ作成
	OBB obb;

	////ダイレクトサウンドのデバイス作成
	//DirectSound* pDs = DirectSound::GetInstance();
	//pDs->Create(App::GetWindowHandle());

	//WaveFile waveFile;			//音声ファイルデータ
	//SoundBuffer soundBuffer;	//再生用バッファ
	//if (waveFile.Load("music/BGM.wav"))
	//{
	//	soundBuffer.Create(waveFile);
	//	soundBuffer.Play(true);
	//}

	
	while (App::Refresh())
	{
		//カメラの操作関連
		{
			//カメラの視点をWASDで動かす
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
			//カメラの座標をWASDで動かす（後でカメラhppに関数化したい）
			if (App::GetKey('W'))//前に移動
			{
				camera.CameraMoveAdvance(0.05f);
			}
			else if (App::GetKey('S'))//後ろに移動
			{
				camera.CameraMoveAdvance(-0.05f);
			}

			if (App::GetKey('D'))//右に移動
			{
				camera.CameraMoveSide(0.05f);
			}
			else if (App::GetKey('A'))//左に移動
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
		
		//欠点　一回しか無理　任意にフラグを切り替える必要あり
		//複数のobjと接触判定とる場合のフラグがだる死ぬ
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

