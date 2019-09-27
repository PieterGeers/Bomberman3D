#include "stdafx.h"
#include "PostColorBalance.h"
#include "RenderTarget.h"


PostColorBalance::PostColorBalance(float redPercent, float greenPercent, float bluePercent)
	: PostProcessingMaterial(L"./Resources/Effects/Post/ColorBalance.fx", 0)
	, m_Red(redPercent)
	, m_Green(greenPercent)
	, m_Blue(bluePercent)
{
}

void PostColorBalance::LoadEffectVariables()
{
	m_pTextureMapVariabele = GetEffect()->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pTextureMapVariabele->IsValid())
		Logger::Log(LogLevel::Error, L"PostBlurEffect variable not initialized");

	m_pBlue = GetEffect()->GetVariableByName("gBp")->AsScalar();
	if (!m_pBlue->IsValid())
		Logger::Log(LogLevel::Error, L"PostBlurEffect variable not initialized");

	m_pGreen = GetEffect()->GetVariableByName("gGp")->AsScalar();
	if (!m_pGreen->IsValid())
		Logger::Log(LogLevel::Error, L"PostBlurEffect variable not initialized");

	m_pRed = GetEffect()->GetVariableByName("gRp")->AsScalar();
	if (!m_pRed->IsValid())
		Logger::Log(LogLevel::Error, L"PostBlurEffect variable not initialized");
}

void PostColorBalance::UpdateEffectVariables(RenderTarget* pRendertarget)
{
	if (m_pTextureMapVariabele->IsValid())
		m_pTextureMapVariabele->SetResource(pRendertarget->GetShaderResourceView());

	if (m_pRed->IsValid())
		m_pRed->SetFloat(m_Red);

	if (m_pGreen->IsValid())
		m_pGreen->SetFloat(m_Green);

	if (m_pBlue->IsValid())
		m_pBlue->SetFloat(m_Blue);
}
