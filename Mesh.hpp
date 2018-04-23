class Mesh
{
public:
	Float3 position;	//���W
	Float3 angles;		//�p�x
	Float3 scale;		//�傫��
	Float3 axis;		//���̂̎��̍��W

	//�{�b�N�X�̌����Ă���ʂ̏����l
	Float4 BaseDir[3]
	{
		Float4(0,0,1,1),
		Float4(1,0,0,1),
		Float4(0,1,0,1)
	};

	//�L�����E�{�b�N�X�Ŏg�p����f�[�^
	struct OBBData
	{
		//�{�b�N�X�̒��S���W
		Float3 OBBpos;
		//�{�b�N�X�̕����x�N�g��
		Float3 OBBvector[3];
		//�{�b�N�X�̎������̒���
		float OBBlength[3];
	};

	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	Material material;

	Mesh()
	{
		App::Initialize();

		position = Float3(0.0f, 0.0f, 0.0f);
		angles = Float3(0.0f, 0.0f, 0.0f);
		scale = Float3(1.0f, 1.0f, 1.0f);
		axis = Float3(0.0f, 0.0f, 0.0f);

		material = Material(L"Shader.hlsl");

		SetCullingMode(D3D11_CULL_BACK);
	}

	~Mesh()
	{

	}

	void CreateData(Texture* tex, int mode)
	{
		material.SetTexture(0, tex);

		Float2 texSize(tex->GetSize().x, tex->GetSize().y);

		switch (mode)
		{
			case 0://UI		���W��ǉ�����[
				CreatePlane(texSize / 2.0f);
				break;
			case 1://�l�p�`	��ɓ���
				CreateCube(tex->GetTexUVData());
				break;
		}
		Apply();
	}

	//���[���h��Matrix���x�N�g���Ƃ��Ď擾
	DirectX::XMMATRIX GetWorld()
	{
		return constant.world;
	}
	//�g���C�A���O�����X�g�쐬
	void CreateTriangle()
	{
		vertices.clear();

		//�x�N�^�ɗv�f��ǉ�
		//������������Ε`�悷�镨��ς��邱�Ƃ��\���낤
		//�ł���Έ������g���ē��I�ɍ�肽��
		vertices.push_back(Vertex(Float3(0.0f, 1.0f, 0.0f), Float3(1.0f, 0.0f, 0.0f), Float2(0.0f, 0.0f)));
		vertices.push_back(Vertex(Float3(1.0f, -1.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f), Float2(1.0f, 0.0f)));
		vertices.push_back(Vertex(Float3(-1.0f, -1.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f), Float2(0.0f, 1.0f)));
	}

	//�l�p�̃{�b�N�X�쐬�p�֐�
	void CreatePlane(
		Float2 size = Float2(0.5f, 0.5f),					//���̂܂܃T�C�Y
		Float2 uv = Float2(1.0f, 1.0f),						//�e�N�X�`���̕�����
		Float2 numUV = Float2(0.0f, 0.0f),					//���ݕ`�悵�����摜
		bool souldclear = true,								//�m���true����Ȃ��Ɗm���_��
		Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),	//x�̍��W
		Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),		//y�̍��W
		Float3 offset = Float3(0.0f, 0.0f, 0.0f),			//z�̍��W
		Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f)	//normal�֌W�@�悭�킩��
	)
	{
		if (souldclear)
		{
			vertices.clear();
			indices.clear();
		}
		leftDirection = DirectX::XMVector3Normalize(leftDirection);
		upDirection = DirectX::XMVector3Normalize(upDirection);
		forwardDirection = DirectX::XMVector3Normalize(forwardDirection);

		vertices.push_back(Vertex(leftDirection * -size.x + upDirection *  size.y + offset,//����
			-forwardDirection, Float2(numUV * (1 / uv))));
		vertices.push_back(Vertex(leftDirection *  size.x + upDirection *  size.y + offset,//�E��
			-forwardDirection, Float2((numUV.x + 1) * (1 / uv.x), numUV.y *(1 / uv.y))));
		vertices.push_back(Vertex(leftDirection * -size.x + upDirection * -size.y + offset,//����
			-forwardDirection, Float2(numUV.x * (1 / uv.x), (numUV.y + 1) *(1 / uv.y))));
		vertices.push_back(Vertex(leftDirection *  size.x + upDirection * -size.y + offset,//�E��
			-forwardDirection, Float2((numUV + 1) * (1 / uv))));

		size_t indexOffset = vertices.size() - 4;
		indices.push_back(indexOffset + 0);
		indices.push_back(indexOffset + 1);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 3);
		indices.push_back(indexOffset + 2);
		indices.push_back(indexOffset + 1);
	}

	void CreateCube(
		Texture::TexUVData tex,
		bool souldClear = true
	)
	{
		if (souldClear)
		{
			vertices.clear();
			indices.clear();
		}

		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[0], false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.0f, 0.0f,-0.5f), Float3( 0.0f, 0.0f, 1.0f));//�O
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[1], false,Float3(-1.0f, 0.0f, 0.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.0f, 0.0f, 0.5f), Float3( 0.0f, 0.0f,-1.0f));//��
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[2], false,Float3( 0.0f, 0.0f,-1.0f), Float3( 0.0f, 1.0f, 0.0f), Float3(-0.5f, 0.0f, 0.0f), Float3( 1.0f, 0.0f, 0.0f));//��
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[3], false,Float3( 0.0f, 0.0f, 1.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.5f, 0.0f, 0.0f), Float3(-1.0f, 0.0f, 0.0f));//�E
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[4], false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 0.0f, 1.0f), Float3( 0.0f, 0.5f, 0.0f), Float3( 0.0f,-1.0f, 0.0f));//��
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[5], false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 0.0f,-1.0f), Float3( 0.0f,-0.5f, 0.0f), Float3( 0.0f, 1.0f, 0.0f));//��
	}
	//�l�p�`�̍�������i�g���C�A���O���{�b�N�X����p�j
	/*
	void CreateTriangleBox(float x, float y, float z)
	{
		vertices.clear();

		//���
		setVertex[0] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//����@�O
		setVertex[1] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//�E��@�O
		setVertex[2] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//����@��

		setVertex[3] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//����@��
		setVertex[4] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�E��@�O
		setVertex[5] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�E��@��

		//�O��
		setVertex[6] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�E��@�O
		setVertex[7] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//����@�O
		setVertex[8] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�E���@�O

		setVertex[9] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//�E���@�O
		setVertex[10] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//����@�O
		setVertex[11] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�����@�O

		//����
		setVertex[12] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//����@��
		setVertex[13] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//�E��@��
		setVertex[14] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�����@��

		setVertex[15] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//�����@��
		setVertex[16] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�E��@��
		setVertex[17] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�E���@��

		//���ʁ@�E	����
		setVertex[18] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�E��@��
		setVertex[19] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//�E��@�O
		setVertex[20] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�E���@��

		setVertex[21] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//�E���@��
		setVertex[22] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�E��@�O
		setVertex[23] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�E���@�O

		//���ʁ@���@����
		setVertex[24] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//����@�O
		setVertex[25] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//����@��
		setVertex[26] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�����@�O

		setVertex[27] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//�����@�O
		setVertex[28] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//����@��
		setVertex[29] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�����@��

		//��
		setVertex[30] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�E���@�O
		setVertex[31] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//�����@�O
		setVertex[32] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�E���@��

		setVertex[33] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//�E���@��
		setVertex[34] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//�����@�O
		setVertex[35] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//�����@��

		for (int i = 0; i < 36; i++)
		{
			vertices.push_back(setVertex[i]);
		}
	}
	*/

	void SetCullingMode(D3D11_CULL_MODE culingMode)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;//�����_�����O���Ɏg�p����`�惂�[�h�����肷��
		rasterizerDesc.CullMode = culingMode;//�w��̕����������Ă���O�p�`���`�悳��Ȃ����Ƃ�����
		App::GetGraphicsDevice().CreateRasterizerState//���X�^���C�U�[�X�e�[�W�ɓ�����x�����郉�X�^���C�U�X�e�[�g�I�u�W�F�N�g���쐬����
		(
			&rasterizerDesc, 
			&rasterizerState
		);
	}

	void Apply()
	{
		if (vertices.size() > 0)
		{
			D3D11_BUFFER_DESC vertexBufferDesc = {};
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			D3D11_SUBRESOURCE_DATA vertexSubresourceData = {};
			vertexSubresourceData.pSysMem = vertices.data();
			//�o�b�t�@�̏�����
			vertexBuffer.Release();
			//�o�b�t�@�쐬
			App::GetGraphicsDevice().CreateBuffer(&vertexBufferDesc, &vertexSubresourceData,&vertexBuffer);
		}
		if (indices.size() > 0)
		{
			D3D11_BUFFER_DESC indexBufferDesc = {};
			indexBufferDesc.ByteWidth = sizeof(int) * indices.size();
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			D3D11_SUBRESOURCE_DATA indexSubresourceData = {};
			indexSubresourceData.pSysMem = indices.data();
			//�o�b�t�@�̏�����
			indexBuffer.Release();
			//�o�b�t�@�쐬
			App::GetGraphicsDevice().CreateBuffer(&indexBufferDesc,&indexSubresourceData,&indexBuffer);
		}
		material.SetBuffer(&constant, sizeof(Constant));
	}
	//�`��
	void Draw()
	{
		//vertexBuffer������������Ă��邩�ǂ����̊m�F
		if (vertexBuffer == nullptr)
			return;

		material.Attach();

		//���̊p�x�ύX�@�܂����W�ړ��@�s��̊|���Z�����Ă���@�|������������ƕςɂȂ�
		constant.world = DirectX::XMMatrixTranspose
		(
			DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * 
			DirectX::XMMatrixTranslation(axis.x, axis.y, axis.z) *
			DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y)) *
			DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x)) *
			DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z)) *
			DirectX::XMMatrixTranslation(position.x, position.y, position.z )
			
		);
		App::GetGraphicsContext().RSSetState(rasterizerState);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		App::GetGraphicsContext().IASetVertexBuffers(0, 1, &vertexBuffer.p,&stride, &offset);

		if (indexBuffer == nullptr)
		{
			App::GetGraphicsContext().Draw(vertices.size(), 0);
		}
		else
		{
			App::GetGraphicsContext().IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			App::GetGraphicsContext().DrawIndexed(indices.size(), 0, 0);
		}
	}
	
	void SetOBBData()
	{
		//mat1 2�͉�]�s��
		DirectX::XMMATRIX mat1, mat2, mat3;
		mat1 = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x));
		mat2 = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y));
		mat3 = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z));
		Float4 v4[3];//��{�̌����̃x�N�g���ɍs��1,2�̉�]��K�p�������
					 //���v�Z���đ��
					 //vector4�Ȃ͍̂s��̌^��4x4�Ȃ̂ł���ɍ��킹��
		Float3 v3[3];

		for (int i = 0; i < 3; i++)
		{
			v4[i] = XMVector4Transform(BaseDir[i], mat1);
			v4[i] = XMVector4Transform(v4[i], mat2);
			v4[i] = XMVector4Transform(v4[i], mat3);
			v4[i] = XMVector4Transform(v4[i], GetWorld());

			//vec4����vec3�Ƀf�[�^���ڂ�
			v3[i] = Float3(v4[i].x, v4[i].y, v4[i].z);
		}

		//OBB�̒��S���W�ݒ�
		obbData.OBBpos = position;

		//OBB�̕ӂ̒�����ݒ�
		obbData.OBBlength[0] = scale.x;
		obbData.OBBlength[1] = scale.y;
		obbData.OBBlength[2] = scale.z;

		obbData.OBBvector[0] = Float3(DirectX::XMVector3Normalize(v3[0]));
		obbData.OBBvector[1] = Float3(DirectX::XMVector3Normalize(v3[1]));
		obbData.OBBvector[2] = Float3(DirectX::XMVector3Normalize(v3[2]));
	}
	//------------------------------------------�v�ۓc_04_10

	void UpdateOBBData()
	{
		SetOBBData();
	}

	OBBData GetOBBData()
	{
		return obbData;
	}

private:
	OBBData obbData;

	struct Constant
	{
		DirectX::XMMATRIX world;
	};
	Constant constant;
	ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;
	ATL::CComPtr<ID3D11RasterizerState> rasterizerState = nullptr;

};
