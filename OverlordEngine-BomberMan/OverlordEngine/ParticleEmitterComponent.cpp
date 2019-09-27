#include "stdafx.h"
#include "ParticleEmitterComponent.h"
 #include <utility>
#include "EffectHelper.h"
#include "ContentManager.h"
#include "TextureDataLoader.h"
#include "Particle.h"
#include "TransformComponent.h"

ParticleEmitterComponent::ParticleEmitterComponent(std::wstring  assetFile, int particleCount):
	m_pVertexBuffer(nullptr),
	m_pEffect(nullptr),
	m_pParticleTexture(nullptr),
	m_pInputLayout(nullptr),
	m_pInputLayoutSize(0),
	m_Settings(ParticleEmitterSettings()),
	m_ParticleCount(particleCount),
	m_ActiveParticles(0),
	m_LastParticleInit(0.0f),
	m_AssetFile(std::move(assetFile))
{
	for (int i = 0; i < particleCount; ++i)
		m_Particles.push_back(new Particle(m_Settings));
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	for (int i = 0; i < m_ParticleCount; ++i)
		SafeDelete(m_Particles[i]);
	m_Particles.clear();
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const GameContext& gameContext)
{
	LoadEffect(gameContext);
	CreateVertexBuffer(gameContext);
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::LoadEffect(const GameContext& gameContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/ParticleRenderer.fx");
	m_pDefaultTechnique = m_pEffect->GetTechniqueByName("Default");
	m_pWvpVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pViewInverseVariable = m_pEffect->GetVariableByName("gViewInverse")->AsMatrix();
	m_pTextureVariable = m_pEffect->GetVariableByName("gParticleTexture")->AsShaderResource();
	if (!EffectHelper::BuildInputLayout(gameContext.pDevice, m_pDefaultTechnique, &m_pInputLayout, m_pInputLayoutSize))
		Logger::Log(LogLevel::Error, L"ParticleEmitterComponent, inputLayout failed");

}

void ParticleEmitterComponent::CreateVertexBuffer(const GameContext& gameContext)
{
	if (m_pVertexBuffer)
		m_pVertexBuffer->Release();
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = m_ParticleCount * sizeof(ParticleVertex);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	gameContext.pDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer);
	
}

void ParticleEmitterComponent::Update(const GameContext& gameContext)
{
	const float particleInterval = (m_Settings.MinEnergy + m_Settings.MaxEnergy) / 2.0f / m_ParticleCount;
	m_LastParticleInit += gameContext.pGameTime->GetElapsed();

	m_ActiveParticles = 0;
	
	//BUFFER MAPPING CODE [PARTIAL :)]
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	gameContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	ParticleVertex* pBuffer = (ParticleVertex*) mappedResource.pData;
	for (auto ptr = m_Particles.begin(); ptr < m_Particles.end(); ++ptr)
	{
		(*ptr)->Update(gameContext);
		if ((*ptr)->IsActive())
		{
			pBuffer[m_ActiveParticles] = (*ptr)->GetVertexInfo();
			++m_ActiveParticles;
		}
		else if (m_LastParticleInit >= particleInterval)
		{
			(*ptr)->Init(GetTransform()->GetWorldPosition(), m_MaxTravelRange);
			pBuffer[m_ActiveParticles] = (*ptr)->GetVertexInfo();
			++m_ActiveParticles;
			m_LastParticleInit = 0;
		}
	}
	gameContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::Draw(const GameContext& )
{}

void ParticleEmitterComponent::PostDraw(const GameContext& gameContext)
{
	m_pWvpVariable->SetMatrix(&gameContext.pCamera->GetViewProjection()._11);
	m_pViewInverseVariable->SetMatrix(&gameContext.pCamera->GetViewInverse()._11);
	m_pTextureVariable->SetResource(m_pParticleTexture->GetShaderResourceView());

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;
	gameContext.pDeviceContext->IASetInputLayout(m_pInputLayout);
	gameContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	gameContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pDefaultTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pDefaultTechnique->GetPassByIndex(p)->Apply(0, gameContext.pDeviceContext);
		gameContext.pDeviceContext->Draw(m_ParticleCount, 0);
	}

}
