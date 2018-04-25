class Mesh
{
public:
	Float3 position;	//座標
	Float3 angles;		//角度
	Float3 scale;		//大きさ
	Float3 axis;		//物体の軸の座標

	//ボックスの向いている面の初期値
	Float4 BaseDir[3]
	{
		Float4(0,0,1,1),
		Float4(1,0,0,1),
		Float4(0,1,0,1)
	};

	//有向境界ボックスで使用するデータ
	struct OBBData
	{
		//ボックスの中心座標
		Float3 OBBpos;
		//ボックスの方向ベクトル
		Float3 OBBvector[3];
		//ボックスの軸方向の長さ
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
			case 0://UI		座標を追加しろー
				CreatePlane(texSize / 2.0f);
				break;
			case 1://四角形	上に同じ
				CreateCube(tex->GetTexUVData());
				break;
		}
		Apply();
	}

	//ワールドのMatrixをベクトルとして取得
	DirectX::XMMATRIX GetWorld()
	{
		return constant.world;
	}
	//トライアングルリスト作成
	void CreateTriangle()
	{
		vertices.clear();

		//ベクタに要素を追加
		//ここをいじれば描画する物を変えることが可能だろう
		//できれば引数を使って動的に作りたい
		vertices.push_back(Vertex(Float3(0.0f, 1.0f, 0.0f), Float3(1.0f, 0.0f, 0.0f), Float2(0.0f, 0.0f)));
		vertices.push_back(Vertex(Float3(1.0f, -1.0f, 0.0f), Float3(0.0f, 1.0f, 0.0f), Float2(1.0f, 0.0f)));
		vertices.push_back(Vertex(Float3(-1.0f, -1.0f, 0.0f), Float3(0.0f, 0.0f, 1.0f), Float2(0.0f, 1.0f)));
	}

	//四角のボックス作成用関数
	void CreatePlane(
		Float2 size = Float2(0.5f, 0.5f),					//そのままサイズ
		Float2 uv = Float2(1.0f, 1.0f),						//テクスチャの分割数
		Float2 numUV = Float2(0.0f, 0.0f),					//現在描画したい画像
		bool souldclear = true,								//知らんtrueじゃないと確かダメ
		Float3 leftDirection = Float3(1.0f, 0.0f, 0.0f),	//xの座標
		Float3 upDirection = Float3(0.0f, 1.0f, 0.0f),		//yの座標
		Float3 offset = Float3(0.0f, 0.0f, 0.0f),			//zの座標
		Float3 forwardDirection = Float3(0.0f, 0.0f, 1.0f)	//normal関係　よくわかめ
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

		vertices.push_back(Vertex(leftDirection * -size.x + upDirection *  size.y + offset,//左上
			-forwardDirection, Float2(numUV * (1 / uv))));
		vertices.push_back(Vertex(leftDirection *  size.x + upDirection *  size.y + offset,//右上
			-forwardDirection, Float2((numUV.x + 1) * (1 / uv.x), numUV.y *(1 / uv.y))));
		vertices.push_back(Vertex(leftDirection * -size.x + upDirection * -size.y + offset,//左下
			-forwardDirection, Float2(numUV.x * (1 / uv.x), (numUV.y + 1) *(1 / uv.y))));
		vertices.push_back(Vertex(leftDirection *  size.x + upDirection * -size.y + offset,//右下
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

		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[0], false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.0f, 0.0f,-0.5f), Float3( 0.0f, 0.0f, 1.0f));//前
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[1], false,Float3(-1.0f, 0.0f, 0.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.0f, 0.0f, 0.5f), Float3( 0.0f, 0.0f,-1.0f));//後
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[2], false,Float3( 0.0f, 0.0f,-1.0f), Float3( 0.0f, 1.0f, 0.0f), Float3(-0.5f, 0.0f, 0.0f), Float3( 1.0f, 0.0f, 0.0f));//左
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[3], false,Float3( 0.0f, 0.0f, 1.0f), Float3( 0.0f, 1.0f, 0.0f), Float3( 0.5f, 0.0f, 0.0f), Float3(-1.0f, 0.0f, 0.0f));//右
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[4], false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 0.0f, 1.0f), Float3( 0.0f, 0.5f, 0.0f), Float3( 0.0f,-1.0f, 0.0f));//上
		CreatePlane(Float2(0.5f, 0.5f), tex.uv, tex.numUV[5], false,Float3( 1.0f, 0.0f, 0.0f), Float3( 0.0f, 0.0f,-1.0f), Float3( 0.0f,-0.5f, 0.0f), Float3( 0.0f, 1.0f, 0.0f));//下
	}
	//四角形の作られ方↓（トライアングルボックス理解用）
	/*
	void CreateTriangleBox(float x, float y, float z)
	{
		vertices.clear();

		//上面
		setVertex[0] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左上　前
		setVertex[1] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//右上　前
		setVertex[2] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左上　奥

		setVertex[3] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//左上　奥
		setVertex[4] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右上　前
		setVertex[5] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右上　奥

		//前面
		setVertex[6] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右上　前
		setVertex[7] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//左上　前
		setVertex[8] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右下　前

		setVertex[9] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//右下　前
		setVertex[10] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左上　前
		setVertex[11] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左下　前

		//奥側
		setVertex[12] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左上　奥
		setVertex[13] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//右上　奥
		setVertex[14] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左下　奥

		setVertex[15] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//左下　奥
		setVertex[16] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右上　奥
		setVertex[17] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右下　奥

		//正面　右	側面
		setVertex[18] = { Float3(-0.5f + x,-0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右上　奥
		setVertex[19] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//右上　前
		setVertex[20] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右下　奥

		setVertex[21] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//右下　奥
		setVertex[22] = { Float3(-0.5f + x,-0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右上　前
		setVertex[23] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右下　前

		//正面　左　側面
		setVertex[24] = { Float3(0.5f + x,-0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左上　前
		setVertex[25] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//左上　奥
		setVertex[26] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左下　前

		setVertex[27] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//左下　前
		setVertex[28] = { Float3(0.5f + x,-0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左上　奥
		setVertex[29] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左下　奥

		//底
		setVertex[30] = { Float3(-0.5f + x, 0.5f + y, 0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//右下　前
		setVertex[31] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f,-0.0f) };//左下　前
		setVertex[32] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//右下　奥

		setVertex[33] = { Float3(-0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f, 1.0f) };//右下　奥
		setVertex[34] = { Float3(0.5f + x, 0.5f + y, 0.5f + z), Float3(1.0f, 1.0f, 1.0f) };//左下　前
		setVertex[35] = { Float3(0.5f + x, 0.5f + y,-0.5f + z), Float3(0.0f, 1.0f,-0.0f) };//左下　奥

		for (int i = 0; i < 36; i++)
		{
			vertices.push_back(setVertex[i]);
		}
	}
	*/

	void SetCullingMode(D3D11_CULL_MODE culingMode)
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;//レンダリング時に使用する描画モードを決定する
		rasterizerDesc.CullMode = culingMode;//指定の方向を向いている三角形が描画されないことを示す
		App::GetGraphicsDevice().CreateRasterizerState//ラスタライザーステージに動作を支持するラスタライザステートオブジェクトを作成する
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
			//バッファの初期化
			vertexBuffer.Release();
			//バッファ作成
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
			//バッファの初期化
			indexBuffer.Release();
			//バッファ作成
			App::GetGraphicsDevice().CreateBuffer(&indexBufferDesc,&indexSubresourceData,&indexBuffer);
		}
		material.SetBuffer(&constant, sizeof(Constant));
	}
	//描画
	void Draw()
	{
		//vertexBufferが正しく作られているかどうかの確認
		if (vertexBuffer == nullptr)
			return;

		material.Attach();

		//軸の角度変更　また座標移動　行列の掛け算をしている　掛け方をかえると変になる
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
		//mat1 2は回転行列
		DirectX::XMMATRIX mat1, mat2, mat3;
		mat1 = DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(angles.x));
		mat2 = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angles.y));
		mat3 = DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(angles.z));
		Float4 v4[3];//基本の向きのベクトルに行列1,2の回転を適用した状態
					 //を計算して代入
					 //vector4なのは行列の型が4x4なのでそれに合わせた
		Float3 v3[3];

		for (int i = 0; i < 3; i++)
		{
			v4[i] = XMVector4Transform(BaseDir[i], mat1);
			v4[i] = XMVector4Transform(v4[i], mat2);
			v4[i] = XMVector4Transform(v4[i], mat3);
			v4[i] = XMVector4Transform(v4[i], GetWorld());

			//vec4からvec3にデータを移す
			v3[i] = Float3(v4[i].x, v4[i].y, v4[i].z);
		}

		//OBBの中心座標設定
		obbData.OBBpos = position;

		//OBBの辺の長さを設定
		obbData.OBBlength[0] = scale.x;
		obbData.OBBlength[1] = scale.y;
		obbData.OBBlength[2] = scale.z;

		obbData.OBBvector[0] = Float3(DirectX::XMVector3Normalize(v3[0]));
		obbData.OBBvector[1] = Float3(DirectX::XMVector3Normalize(v3[1]));
		obbData.OBBvector[2] = Float3(DirectX::XMVector3Normalize(v3[2]));
	}
	//------------------------------------------久保田_04_10

	//ゴールとなる場所を設定
	void SetGoalPoint()
	{
		goalPoint = true;
	}
	bool GetGoalPoint()
	{
		return goalPoint;
	}

	OBBData GetOBBData()
	{
		return obbData;
	}
private:
	OBBData obbData;
	bool goalPoint = false;

	struct Constant
	{
		DirectX::XMMATRIX world;
	};
	Constant constant;
	ATL::CComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	ATL::CComPtr<ID3D11Buffer> indexBuffer = nullptr;
	ATL::CComPtr<ID3D11RasterizerState> rasterizerState = nullptr;

};
