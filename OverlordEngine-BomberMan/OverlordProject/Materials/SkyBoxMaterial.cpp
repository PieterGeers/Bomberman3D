#include "stdafx.h"
#include "SkyBoxMaterial.h"
#include "ContentManager.h"
#include "TextureData.h"

ID3DX11EffectShaderResourceVariable* SkyBoxMaterial::m_pDiffuseSRVvariable;

SkyBoxMaterial::SkyBoxMaterial()
	: Material(L"./Resources/Effects/SkyBox.fx")

{
}

void SkyBoxMaterial::SetSkyBoxMaterial(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void SkyBoxMaterial::LoadEffectVariables()
{
	auto effect = ContentManager::Load<ID3DX11Effect>(L"./Resources/Effects/SkyBox.fx");
	m_pDiffuseSRVvariable = effect->GetVariableByName("m_CubeMap")->AsShaderResource();

}

void SkyBoxMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());

}



