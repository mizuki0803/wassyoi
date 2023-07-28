#include "ImageUIRenderer.h"
#include "DescHeapSRV.h"
#include "Easing.h"


ID3D12Device *ImageUIRenderer::dev = nullptr;
ID3D12GraphicsCommandList *ImageUIRenderer::cmdList = nullptr;

ImageUIRenderer *ImageUIRenderer::Create()
{
	//インスタンスを生成
	ImageUIRenderer *instance = new ImageUIRenderer();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
		return nullptr;
	}

	return instance;
}

bool ImageUIRenderer::Initialize()
{
	HRESULT result;

	player_camera_.reset(new Camera);
	player_camera_->Initialize();
	Vector3 player_camera_pos = { -5,25,-20 };
	player_camera_->SetEye(player_camera_pos);
	player_camera_->SetTarget({0,0,0});
	camera_camera_.reset(new Camera);
	camera_camera_->Initialize();
	camera_camera_->SetEye({ 5,25,-20 });
	camera_camera_->SetTarget({ 0,0,0 });

	//頂点データ
	VertexPosUv vertices[] = {
		{{ -1.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},	//左下
		{{ -1.0f, +1.0f, 0.0f}, {0.0f, 0.0f}},	//左上
		{{ +1.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},	//右下
		{{ +1.0f, +1.0f, 0.0f}, {1.0f, 0.0f}},	//右上
	};

	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//頂点バッファへのデータ転送
	VertexPosUv *vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

	////定数バッファの生成
	//result = dev->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) * 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuff));
	//assert(SUCCEEDED(result));

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WindowApp::window_width,
		(UINT)WindowApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	for (int i{ 0 }; i < static_cast<int>(TexName::Max); ++i)
	{
		//テクスチャバッファの生成	
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		result = dev->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
				D3D12_MEMORY_POOL_L0),
			D3D12_HEAP_FLAG_NONE,
			&texresDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
			IID_PPV_ARGS(&texture_[i].texBuff)
		);
		assert(SUCCEEDED(result));


		//テクスチャを赤クリア
		{
			//画素数(1280x720 = 921600ピクセル)
			const UINT pixelCount = WindowApp::window_width * WindowApp::window_height;
			//画像1行分のデータサイズ
			const UINT rowPitch = sizeof(UINT) * WindowApp::window_width;
			//画像全体のデータサイズ
			const UINT depthPitch = rowPitch * WindowApp::window_height;
			//画像イメージ
			UINT *img = new UINT[pixelCount];
			for (int i = 0; i < pixelCount; i++) {
				img[i] = 0xff0000ff;
			}

			//テクスチャバッファにデータ転送
			result = texture_[i].texBuff->WriteToSubresource(0, nullptr,
				img, rowPitch, depthPitch);
			assert(SUCCEEDED(result));
			delete[] img;
		}

		//SRV設定
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = 1;
		//デスクリプタヒープにSRV作成
		DescHeapSRV::CreateShaderResourceView(srvDesc, texture_[i]);

		//RTV用デスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescHeapDesc.NumDescriptors = 2;
		//RTV用デスクリプタヒープを生成
		result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV_));
		assert(SUCCEEDED(result));

	}


	// RTV
	for (int i{ 0 }; i < static_cast<int>(TexName::Max); ++i)
	{
		//デスクリプタヒープにRTV生成
		dev->CreateRenderTargetView(
			texture_[i].texBuff.Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))

		);
	}
	texture_[static_cast<int>(TexName::Camera)].texBuff->SetName(L"Camera");
	texture_[static_cast<int>(TexName::Move)].texBuff->SetName(L"Move");


	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_D32_FLOAT,
			WindowApp::window_width,
			WindowApp::window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);
	//深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuff)
	);
	assert(SUCCEEDED(result));

	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV_));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(depthBuff.Get(),
		&dsvDesc,
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart());

	

	// モデルの読み込み
	key_[static_cast<int>(KeyObjectName::W)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradW", true))
	);
	key_[static_cast<int>(KeyObjectName::W)]->SetPosition({ 0,0,-2});

	key_[static_cast<int>(KeyObjectName::A)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradA", true))
	);
	key_[static_cast<int>(KeyObjectName::A)]->SetPosition({ -2,0,-4 });

	key_[static_cast<int>(KeyObjectName::S)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradS", true))
	);
	key_[static_cast<int>(KeyObjectName::S)]->SetPosition({ 0,0,-4 });

	key_[static_cast<int>(KeyObjectName::D)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradD", true))
	);
	key_[static_cast<int>(KeyObjectName::D)]->SetPosition({ 2,0,-4 });

	player_.reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("Player", true))
	);
	player_->SetPosition({ 0,0,4 });
	player_->SetScale({ 2,2,2 });

	arrow_[static_cast<int>(ArrowObjectName::Left)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("Arrow", true))
	);
	arrow_[static_cast<int>(ArrowObjectName::Left)]->SetColor(
		{ 0.5f,0.5f,0.5f,1.0f }
	);
	arrow_[static_cast<int>(ArrowObjectName::Left)]->SetPosition(
		{ -3.0f,0.0f,0.5f }
	);

	arrow_[static_cast<int>(ArrowObjectName::Right)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("Arrow", true))
	);
	arrow_[static_cast<int>(ArrowObjectName::Right)]->SetColor(
		{ 0.5f,0.5f,0.5f,1.0f }
	);
	arrow_[static_cast<int>(ArrowObjectName::Right)]->SetPosition(
		{ 3.0f,0.0f,0.5f }
	);
	arrow_[static_cast<int>(ArrowObjectName::Right)]->SetRotation(
		{ 0.0f,180.0f,0.0f }
	);


	// カメラ用
	key_[static_cast<int>(KeyObjectName::Up)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradUp", true))
	);
	key_[static_cast<int>(KeyObjectName::Up)]->SetPosition({ 0,0,-2 });
	key_[static_cast<int>(KeyObjectName::Left)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradLeft", true))
	);
	key_[static_cast<int>(KeyObjectName::Left)]->SetPosition({ -2,0,-4 });
	key_[static_cast<int>(KeyObjectName::Down)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradDown", true))
	);
	key_[static_cast<int>(KeyObjectName::Down)]->SetPosition({ 0,0,-4 });
	key_[static_cast<int>(KeyObjectName::Right)].reset(
		ObjObject3d::Create(ObjModel::LoadFromOBJ("KeyBoradRight", true))
	);
	key_[static_cast<int>(KeyObjectName::Right)]->SetPosition({ 2,0,-4 });
	return true;
}

void ImageUIRenderer::Update(bool isMoveMenu, bool isCameraMenu)
{
	static auto aKey = key_[static_cast<int>(KeyObjectName::A)].get();
	static auto dKey = key_[static_cast<int>(KeyObjectName::D)].get();
	static auto leftKey = key_[static_cast<int>(KeyObjectName::Left)].get();
	static auto rightKey = key_[static_cast<int>(KeyObjectName::Right)].get();
		// 移動用画像更新
	if (isMoveMenu)
	{
		// D更新
		{
			auto pos = dKey->GetPosition();
			auto rot = arrow_[static_cast<int>(ArrowObjectName::Right)]->GetRotation();
			// 押し込み
			if (moveFrameCounter_ >= 0 &&
				moveFrameCounter_ < 15)
			{
				pos.y -= 0.05f;
				dKey->SetPosition(pos);
				
			}
			// 押し上げ
			else if (
				moveFrameCounter_ >= 135 &&
				moveFrameCounter_ < 150)
			{
				pos.y += 0.05f;
				dKey->SetPosition(pos);
			}

			if (moveFrameCounter_ >= 0 &&
				moveFrameCounter_ < 150)
			{
				rot.x += (180.0f / 150.0f);
				arrow_[static_cast<int>(ArrowObjectName::Right)]->SetRotation(
					{ rot }
				);
				arrow_[static_cast<int>(ArrowObjectName::Right)]->SetColor(
					{ 0.8f,0.0f,0.0f,0.8f }
				);
			}
			else
			{
				arrow_[static_cast<int>(ArrowObjectName::Right)]->SetColor(
					{ 0.3f,0.3f,0.3f,1.0f }
				);
			}
		}
		
		// A更新
		{
			auto pos = aKey->GetPosition();
			auto rot = arrow_[static_cast<int>(ArrowObjectName::Left)]->GetRotation();
			if (
				moveFrameCounter_ >= 180 &&
				moveFrameCounter_ < 195)
			{
				pos.y -= 0.05f;
				aKey->SetPosition(pos);
			}
			else if (
				moveFrameCounter_ >= 300 &&
				moveFrameCounter_ < 315)
			{
				pos.y += 0.05f;
				aKey->SetPosition(pos);
			}

			if (moveFrameCounter_ >= 180 &&
				moveFrameCounter_ < 315)
			{
				rot.x += (180.0f / 150.0f);
				arrow_[static_cast<int>(ArrowObjectName::Left)]->SetRotation(
					{ rot }
				);
				arrow_[static_cast<int>(ArrowObjectName::Left)]->SetColor(
					{ 0.8f,0.0f,0.0f,0.8f }
				);
			}
			else
			{
				arrow_[static_cast<int>(ArrowObjectName::Left)]->SetColor(
					{ 0.3f,0.3f,0.3f,1.0f }
				);
			}
		}

		// Player
		{
			auto pos = player_->GetPosition();
			// +移動
			if (moveFrameCounter_ >= 0 &&
				moveFrameCounter_ < 150)
			{
				int index{ static_cast<int>(moveFrameCounter_ / 30.0f) };
				pos.x = Easing::OutQuint(
					player_pos_[index],
					player_pos_[index + 1],
					static_cast<float>((moveFrameCounter_ - index * 30) / 30.0f)
					);
				
				player_->SetPosition(pos);
			}
			// -移動
			else if (
				moveFrameCounter_ >= 180 &&
				moveFrameCounter_ < 315
				)
			{
				int counter = moveFrameCounter_ - 180;
				int index{ 5 - static_cast<int>(counter / 30.0f) };
				int next_index{ index - 1 };
				pos.x = Easing::OutQuint(
					player_pos_[index],
					player_pos_[next_index],
					static_cast<float>((counter - ( 5 - index) * 30) / 30.0f)
				);

				player_->SetPosition(pos);
				player_->SetPosition(pos);
			}
		}

		// カウントリセット
		if (moveFrameCounter_ > 345)
		{
			moveFrameCounter_ = 0;
		}
		else
		{
			++moveFrameCounter_;
		}
	}

	// カメラ用画像更新
	if (isCameraMenu)
	{
		// →
		{
			auto pos = rightKey->GetPosition();
			auto rot = arrow_[static_cast<int>(ArrowObjectName::Right)]->GetRotation();
			// 押し込み
			if (cameraFrameCounter_ >= 0 &&
				cameraFrameCounter_ < 15)
			{
				pos.y -= 0.05f;
				rightKey->SetPosition(pos);

			}
			// 押し上げ
			else if (
				cameraFrameCounter_ >= 135 &&
				cameraFrameCounter_ < 150)
			{
				pos.y += 0.05f;
				rightKey->SetPosition(pos);
			}

			if (cameraFrameCounter_ >= 0 &&
				cameraFrameCounter_ < 150)
			{
				/*rot.x += (180.0f / 150.0f);
				arrow_[static_cast<int>(ArrowObjectName::Right)]->SetRotation(
					{ rot }
				);
				arrow_[static_cast<int>(ArrowObjectName::Right)]->SetColor(
					{ 0.8f,0.0f,0.0f,0.8f }
				);*/
			}
			else
			{
				/*arrow_[static_cast<int>(ArrowObjectName::Right)]->SetColor(
					{ 0.3f,0.3f,0.3f,1.0f }
				);*/
			}
		}

		// ←
		{
			auto pos = leftKey->GetPosition();
			auto rot = arrow_[static_cast<int>(ArrowObjectName::Left)]->GetRotation();
			if (
				cameraFrameCounter_ >= 180 &&
				cameraFrameCounter_ < 195)
			{
				pos.y -= 0.05f;
				leftKey->SetPosition(pos);
			}
			else if (
				cameraFrameCounter_ >= 300 &&
				cameraFrameCounter_ < 315)
			{
				pos.y += 0.05f;
				leftKey->SetPosition(pos);
			}

			if (cameraFrameCounter_ >= 180 &&
				cameraFrameCounter_ < 315)
			{
				/*rot.x += (180.0f / 150.0f);
				arrow_[static_cast<int>(ArrowObjectName::Left)]->SetRotation(
					{ rot }
				);
				arrow_[static_cast<int>(ArrowObjectName::Left)]->SetColor(
					{ 0.8f,0.0f,0.0f,0.8f }
				);*/
			}
			else
			{
				/*arrow_[static_cast<int>(ArrowObjectName::Left)]->SetColor(
					{ 0.3f,0.3f,0.3f,1.0f }
				);*/
			}
		}
		// カウントリセット
		if (cameraFrameCounter_ > 345)
		{
			cameraFrameCounter_ = 0;
		}
		else
		{
			++cameraFrameCounter_;
		}

	}
}

void ImageUIRenderer::DrawCameraDescription()
{
	ObjObject3d::SetCamera(camera_camera_.get());
	camera_camera_->Update();
	for (int i = static_cast<int>(KeyObjectName::Up); i <= static_cast<int>(KeyObjectName::Right); ++i)
	{
		key_[i]->Update();
	}


	//リソースバリアを変更(シェーダリソース→描画可能)
	cmdList->ResourceBarrier(1,	&CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Camera)].texBuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));


	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), static_cast<int>(TexName::Camera),
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		WindowApp::window_width, WindowApp::window_height));
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WindowApp::window_width,
		WindowApp::window_height));

	//全画面クリア
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);

	// ここから描画
	ObjObject3d::DrawPrev();


	for (int i = static_cast<int>(KeyObjectName::Up); i <= static_cast<int>(KeyObjectName::Right); ++i)
	{
		key_[i]->Draw();
	}


	//リソースバリアを変更(描画可能→シェーダリソース)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Camera)].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void ImageUIRenderer::DrawMoveDescription()
{
	ObjObject3d::SetCamera(player_camera_.get());
	player_camera_->Update();
	// モデル更新
	player_->Update();
	for (int i = 0; i <= static_cast<int>(KeyObjectName::D); ++i)
	{
		key_[i]->Update();
	}
	arrow_[static_cast<int>(ArrowObjectName::Left)]->Update();
	arrow_[static_cast<int>(ArrowObjectName::Right)]->Update();
	//リソースバリアを変更(シェーダリソース→描画可能)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Move)].texBuff.Get(),
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		D3D12_RESOURCE_STATE_RENDER_TARGET));


	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV_->GetCPUDescriptorHandleForHeapStart(), static_cast<int>(TexName::Move),
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV_->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f,
		WindowApp::window_width, WindowApp::window_height));
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WindowApp::window_width,
		WindowApp::window_height));

	//全画面クリア
	float clearColor[4] = {	0.0f, 0.0f, 0.0f, 0.0f };
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
		nullptr);

	// ここから描画
	ObjObject3d::DrawPrev();

	// モデル更新
	player_->Draw();

	for (int i = 0; i <= static_cast<int>(KeyObjectName::D); ++i)
	{
		key_[i]->Draw();
	}

	arrow_[static_cast<int>(ArrowObjectName::Left)]->Draw();
	arrow_[static_cast<int>(ArrowObjectName::Right)]->Draw();

	//リソースバリアを変更(描画可能→シェーダリソース)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture_[static_cast<int>(TexName::Move)].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

}
