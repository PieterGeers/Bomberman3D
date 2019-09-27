#include "stdafx.h"
#include "PostProcessingMaterial.h"
#include "RenderTarget.h"
#include "OverlordGame.h"
#include "ContentManager.h"

PostProcessingMaterial::PostProcessingMaterial(std::wstring effectFile, unsigned int renderIndex,
                                               std::wstring technique)
	: m_IsInitialized(false), 
	  m_pInputLayout(nullptr),
	  m_pInputLayoutSize(0),
	  m_effectFile(std::move(effectFile)),
	  m_InputLayoutID(0),
	  m_RenderIndex(renderIndex),
	  m_pRenderTarget(nullptr),
	  m_pVertexBuffer(nullptr),
	  m_pIndexBuffer(nullptr),
	  m_NumVertices(0),
	  m_NumIndices(0),
	  m_VertexBufferStride(sizeof(VertexPosTex)),
	  m_pEffect(nullptr),
	  m_pTechnique(nullptr),
	  m_TechniqueName(std::move(technique))
{
}

PostProcessingMaterial::~PostProcessingMaterial()
{
	//TODO: delete and/or release necessary objects and/or resources
	SafeDelete(m_pRenderTarget);
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void PostProcessingMaterial::Initialize(const GameContext& gameContext)
{
	if (!m_IsInitialized)
	{
		LoadEffect(gameContext, m_effectFile);

		EffectHelper::BuildInputLayout(gameContext.pDevice, m_pTechnique, &m_pInputLayout, 
			m_pInputLayoutDescriptions, m_pInputLayoutSize, m_InputLayoutID);
		
		CreateVertexBuffer(gameContext);
		CreateIndexBuffer(gameContext);

		m_pRenderTarget = new RenderTarget(gameContext.pDevice);
		RENDERTARGET_DESC desc{};
		desc.Width = OverlordGame::GetGameSettings().Window.Width;
		desc.Height = OverlordGame::GetGameSettings().Window.Height;
		desc.EnableDepthBuffer = true;
		desc.EnableDepthSRV = true;
		desc.EnableColorBuffer = true;
		desc.EnableColorSRV = true;
		desc.GenerateMipMaps_Color = true;
		desc.ColorBufferSupplied = false;
		desc.DepthBufferSupplied = false;
		m_pRenderTarget->Create(desc);

		m_IsInitialized = true;
	}
}

bool PostProcessingMaterial::LoadEffect(const GameContext& gameContext, const std::wstring& effectFile)
{
	UNREFERENCED_PARAMETER(gameContext);

	m_pEffect = ContentManager::Load<ID3DX11Effect>(effectFile);
	if (!m_TechniqueName.empty())
	{
		m_pTechnique = m_pEffect->GetTechniqueByName(reinterpret_cast<LPCSTR>(m_TechniqueName.c_str()));
		if (!m_pTechnique->IsValid())
			Logger::LogError(L"PostProcessingMaterial.cpp -> m_pTechnique not set");
	}
	else
		m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (!m_pTechnique->IsValid())
		return false;

	LoadEffectVariables();
	return true;
}

void PostProcessingMaterial::Draw(const GameContext& gameContext, RenderTarget* previousRendertarget)
{
	UINT offset = 0;

	m_pRenderTarget->Clear(gameContext, DirectX::Colors::Black);
	UpdateEffectVariables(previousRendertarget);
	
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &m_VertexBufferStride, &offset);
	gameContext.pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(m_NumIndices, 0, 0);
	}

	// Generate Mips
	gameContext.pDeviceContext->GenerateMips(m_pRenderTarget->GetShaderResourceView());
}

void PostProcessingMaterial::CreateVertexBuffer(const GameContext& gameContext)
{
	m_NumVertices = 4;
	const VertexPosTex leftBot{ DirectX::XMFLOAT3{-1.f, 1.0f,0.0f}, DirectX::XMFLOAT2{0.0f,0.0f} };
	const VertexPosTex leftTop{ DirectX::XMFLOAT3{-1.0f, -1.0f,0.0f},DirectX::XMFLOAT2{0.0f,1.0f} };
	const VertexPosTex rightBot{ DirectX::XMFLOAT3{1.0f, 1.0f,0.0f},DirectX::XMFLOAT2{1.0f,0.0f} };
	const VertexPosTex rightTop{ DirectX::XMFLOAT3{1.0f, -1.0f,0.0f},DirectX::XMFLOAT2{1.0f,1.0f} };
	std::vector<VertexPosTex> vertices{leftBot,leftTop,rightBot,rightTop};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = m_NumVertices * sizeof(VertexPosTex);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = vertices.data();
	gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
}

void PostProcessingMaterial::CreateIndexBuffer(const GameContext& gameContext)
{
	m_NumIndices = 6;
	std::vector<DWORD> indices = { 0,2,1,2,3,1 };

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(DWORD) * m_NumIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = indices.data();
	gameContext.pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);

}
