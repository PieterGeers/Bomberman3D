#include "stdafx.h"
#include "ShadowMapRenderer.h"
#include "ContentManager.h"
#include "ShadowMapMaterial.h"
#include "RenderTarget.h"
#include "MeshFilter.h"
#include "SceneManager.h"
#include "OverlordGame.h"

ShadowMapRenderer::~ShadowMapRenderer()
{
	SafeDelete(m_pShadowRT);
	SafeDelete(m_pShadowMat);
}

void ShadowMapRenderer::Initialize(const GameContext& gameContext)
{
	if (m_IsInitialized)
		return;

	m_pShadowMat = new ShadowMapMaterial{};
	m_pShadowMat->Initialize(gameContext);

	m_pShadowRT = new RenderTarget{ gameContext.pDevice };
	RENDERTARGET_DESC desc{};
	desc.Width = OverlordGame::GetGameSettings().Window.Width;
	desc.Height = OverlordGame::GetGameSettings().Window.Height;
	desc.EnableDepthBuffer = true;
	desc.EnableDepthSRV = true;
	desc.EnableColorBuffer = false;
	desc.EnableColorSRV = false;
	m_pShadowRT->Create(desc);

	m_IsInitialized = true;
}

void ShadowMapRenderer::SetLight(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 direction)
{
	m_LightPosition = position;
	m_LightDirection = direction;

	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	DirectX::XMVECTOR LightPosition = DirectX::XMLoadFloat3(&position);
	DirectX::XMVECTOR LightDirection = DirectX::XMLoadFloat3(&direction);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f,1.0f,0.0f, 0.0f);
	
	const float viewWidth = (m_Size > 0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
	const float viewHeight = (m_Size > 0) ? m_Size : windowSettings.Height;
	
	DirectX::XMMATRIX view = DirectX::XMMatrixLookToLH(LightPosition, LightDirection, up);
	DirectX::XMMATRIX projection = DirectX::XMMatrixOrthographicLH(viewWidth * 5, viewHeight * 5, 0.1f, 5000.0f);
	
	DirectX::XMStoreFloat4x4(&m_LightVP, view * projection);
}

void ShadowMapRenderer::Begin(const GameContext& gameContext) const
{
	//Reset Texture Register 5 (Unbind)
	ID3D11ShaderResourceView *const pSRV[] = { nullptr };
	gameContext.pDeviceContext->PSSetShaderResources(1, 1, pSRV);

	SceneManager::GetInstance()->GetGame()->SetRenderTarget(m_pShadowRT);
	m_pShadowRT->Clear(gameContext, DirectX::Colors::CornflowerBlue);
}

void ShadowMapRenderer::End(const GameContext& gameContext) const
{
	UNREFERENCED_PARAMETER(gameContext);
	SceneManager::GetInstance()->GetGame()->SetRenderTarget(nullptr);
}

void ShadowMapRenderer::Draw(const GameContext& gameContext, MeshFilter* pMeshFilter, DirectX::XMFLOAT4X4 world, const std::vector<DirectX::XMFLOAT4X4>& bones) const
{
	ID3DX11EffectTechnique* shadowTechnique{nullptr};
	VertexBufferData vertexBufferData{};
	UINT offset = 0;

	const ShadowMapMaterial::ShadowGenType type = (!pMeshFilter->m_HasAnimations) ? ShadowMapMaterial::ShadowGenType::Static : ShadowMapMaterial::ShadowGenType::Skinned;
	
	m_pShadowMat->SetBones(reinterpret_cast<const float*>(bones.data()), bones.size());
	m_pShadowMat->SetLightVP(m_LightVP);
	m_pShadowMat->SetWorld(world);

	shadowTechnique = m_pShadowMat->m_pShadowTechs[type];
	vertexBufferData = pMeshFilter->GetVertexBufferData(gameContext, m_pShadowMat->m_InputLayoutIds[type]);
	
	gameContext.pDeviceContext->IASetInputLayout(m_pShadowMat->m_pInputLayouts[type]);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferData.pVertexBuffer, &vertexBufferData.VertexStride, &offset);
	gameContext.pDeviceContext->IASetIndexBuffer(pMeshFilter->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3DX11_TECHNIQUE_DESC techDesc{};
	shadowTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		shadowTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->DrawIndexed(pMeshFilter->m_IndexCount, 0, 0);
	}
}

void ShadowMapRenderer::UpdateMeshFilter(const GameContext& gameContext, MeshFilter* pMeshFilter)
{
	ShadowMapMaterial::ShadowGenType type = (!pMeshFilter->m_HasAnimations) ? ShadowMapMaterial::ShadowGenType::Static : ShadowMapMaterial::ShadowGenType::Skinned;
	pMeshFilter->BuildVertexBuffer(gameContext, m_pShadowMat->m_InputLayoutIds[type], m_pShadowMat->m_InputLayoutSizes[type], m_pShadowMat->m_InputLayoutDescriptions[type]);
}

ID3D11ShaderResourceView* ShadowMapRenderer::GetShadowMap() const
{
	return m_pShadowRT->GetDepthShaderResourceView();
}
