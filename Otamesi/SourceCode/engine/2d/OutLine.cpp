#include "OutLine.h"

#include "WindowApp.h"
#include "DescHeapSRV.h"
#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

ID3D12Device *OutLine::dev = nullptr;
ID3D12GraphicsCommandList *OutLine::cmdList = nullptr;
PipelineSet OutLine::pipelineSet;
Camera *OutLine::camera_ = nullptr;

OutLine *OutLine::Create()
{
	//インスタンスを生成
	OutLine *instance = new OutLine();
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

bool OutLine::Initialize()
{
	//パイプライン生成
	CreateBeforeGraphicsPipelineState();

	HRESULT result;

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

	for (int i{ 0 }; i < kTexMax; ++i)
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
			IID_PPV_ARGS(&texture[i].texBuff)
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
			result = texture[i].texBuff->WriteToSubresource(0, nullptr,
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
		DescHeapSRV::CreateShaderResourceView(srvDesc, texture[i]);

		//RTV用デスクリプタヒープ設定
		D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
		rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvDescHeapDesc.NumDescriptors = 2;
		//RTV用デスクリプタヒープを生成
		result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
		assert(SUCCEEDED(result));

	}
	

	// RTV
	for (int i{ 0 }; i < kTexMax; ++i)
	{
		//デスクリプタヒープにRTV生成
		dev->CreateRenderTargetView(
			texture[i].texBuff.Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(
				descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
				dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV))

		);
	}
	texture[0].texBuff->SetName(L"OutLineBase");
	texture[1].texBuff->SetName(L"DepthTex");


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
	////DSV用デスクリプタヒープを生成
	//result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	//assert(SUCCEEDED(result));

	////デスクリプタヒープにDSV作成
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//dev->CreateDepthStencilView(depthBuff.Get(),
	//	&dsvDesc,
	//	descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(CameraBuffer) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&camera_buffer_));


	return true;
}

void OutLine::Draw()
{

	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//定数バッファへのデータ転送(光源カメラ視点)
	CameraBuffer *camera_map = nullptr;
	if (SUCCEEDED(camera_buffer_->Map(0, nullptr, (void **)&camera_map))) {
		camera_map->view_projection = camera_->GetMatView() * camera_->GetMatProjection();
		camera_map->inv_view_projection = XMMatrixInverse(nullptr, camera_map->view_projection);
		camera_buffer_->Unmap(0, nullptr);
	}
	cmdList->SetGraphicsRootConstantBufferView(0, camera_buffer_->GetGPUVirtualAddress());

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//シェーダリソースビューをセット
	DescHeapSRV::SetGraphicsRootDescriptorTable(1, texture[kColor].texNumber);
	DescHeapSRV::SetGraphicsRootDescriptorTable(1, texture[kDepth].texNumber);



	//ポリゴンの描画(4頂点で四角形)
	cmdList->DrawInstanced(4, 1, 0, 0);


}

void OutLine::DrawScenePrev(ID3D12DescriptorHeap *descHeapDSV)
{
	for (int i{ 0 }; i < kTexMax; ++i)
	{
		//リソースバリアを変更(シェーダリソース→描画可能)
		cmdList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(texture[i].texBuff.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET));
	}
	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[kTexMax];
	for (int i{ 0 }; i < kTexMax; ++i)
	{
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
		);
	}

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		descHeapDSV->GetCPUDescriptorHandleForHeapStart();



	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(kTexMax, rtvH, false, &dsvH);

	CD3DX12_VIEWPORT viewports[kTexMax];
	CD3DX12_RECT scissor_rects[kTexMax];
	for (int i = 0; i < kTexMax; ++i)
	{
		viewports[i] = CD3DX12_VIEWPORT(0.0f, 0.0f,
			WindowApp::window_width, WindowApp::window_height); 
		scissor_rects[i] = CD3DX12_RECT(0, 0, 
			WindowApp::window_width, WindowApp::window_height);
	}

	//ビューポートの設定
	cmdList->RSSetViewports(2, viewports);
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(2, scissor_rects);

	//全画面クリア
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < kTexMax; ++i)
	{
		cmdList->ClearRenderTargetView(rtvH[i], clearColor, 0, nullptr);
	}


	////深度バッファのクリア
	//cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0,
	//	nullptr);
}

void OutLine::DrawSceneRear()
{
	//リソースバリアを変更(描画可能→シェーダリソース)
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[kColor].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture[kDepth].texBuff.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, 
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void OutLine::CreateBeforeGraphicsPipelineState()
{
	HRESULT result;

	ComPtr<ID3DBlob> vsBlob;		//頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;		//ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob;		//エラーオブジェクト

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OutLineVS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//インクルード可能にする
		"main", "vs_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバック用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OutLinePS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//インクルード可能にする
		"main", "ps_5_0",	//エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&psBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char *)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";

		//エラー内容を出力ウインドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	//頂点レイアウト配列の宣言と設定
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{	//xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{	//uv座標
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	//パイプラインステートの設定変数の宣言と、各種項目の設定
	//グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスクとラスタライザステートの設定
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;	//標準設定
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);	//一旦標準値をセット
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;			//背面カリングをしない

	//デプスステンシルステートの設定
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//一旦標準値をセット
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//常に上書きルール

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//環境設定

	//共通設定
	blenddesc.BlendEnable = true;	//ブレンドを有効にする
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;	//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;	//デストの値を0%使う
	//色合成

	//加算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	//減算合成
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	//デストからソースを減算
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;	//ソースの値を100%使う
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	//デストの値を100%使う

	//色反転
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	//1.0f-デストカラーの値
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;	//使わない

	////半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	//加算
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;	//ソースのアルファ値
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	//1.0f-ソ

	//深度値フォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状を三角形に設定
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//その他の設定
	// 書き出し設定
	gpipeline.NumRenderTargets = 2;	//描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラー画像
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM;	// カラー画像
	gpipeline.SampleDesc.Count = 1;	//1ピクセルにつき1回サンプリング

	//デスクリプタテーブルの設定
	CD3DX12_DESCRIPTOR_RANGE depthSrv{};
	//CD3DX12_DESCRIPTOR_RANGE outlineObjectSrv{};
	// 読み込むするテクスチャ
	depthSrv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);	//t0 画像
	//outlineObjectSrv.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);	//t1 アウトラインオブジェクト用テクスチャ

	//ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0,0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &depthSrv);
	//rootparams[2].InitAsDescriptorTable(1, &outlineObjectSrv);

	//テクスチャサンプラーの設定
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	//ルートシグネチャの生成
	ComPtr<ID3DBlob> rootSigBlob;

	//バージョン自動判定でのシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result));

	//ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&pipelineSet.rootsignature));
	assert(SUCCEEDED(result));

	//グラフィックスパイプラインステートの生成
	//パイプラインにルートシグネチャをセット
	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();

	//パイプラインステートの生成
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));
	assert(SUCCEEDED(result));
}
