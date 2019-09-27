#include "stdafx.h"

#include "UberMaterial.h"
#include "ContentManager.h"
#include "TextureData.h"

ID3DX11EffectVectorVariable* UberMaterial::m_pLightDirectionVariable;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseDiffuseTextureVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pDiffuseSRVvariable;
ID3DX11EffectVectorVariable* UberMaterial::m_pDiffuseColorVariable;

ID3DX11EffectVectorVariable* UberMaterial::m_pSpecularColorVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseSpecularLevelTextureVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pSpecularLevelSRVvariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pShininessVariable;

ID3DX11EffectVectorVariable* UberMaterial::m_pAmbientColorVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pAmbientIntensityVariable;

ID3DX11EffectScalarVariable* UberMaterial::m_pFlipGreenChannelVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseNormalMappingVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pNormalMappingSRVvariable;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseEnvironmentMappingVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pEnvironmentSRVvariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pReflectionStrengthVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionStrengthVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pRefractionIndexVariable;

ID3DX11EffectScalarVariable* UberMaterial::m_pOpacityVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUseOpacityMapVariable;
ID3DX11EffectShaderResourceVariable* UberMaterial::m_pOpacitySRVvariable;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseBlinnVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pUsePhongVariable;

ID3DX11EffectScalarVariable* UberMaterial::m_pUseFresnelFalloffVariable;
ID3DX11EffectVectorVariable* UberMaterial::m_pFresnelColorVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelPowerVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelMultiplierVariable;
ID3DX11EffectScalarVariable* UberMaterial::m_pFresnelHardnessVariable;

UberMaterial::UberMaterial()
	:Material(L"./Resources/Effects/UberShader.fx")
{}

UberMaterial::~UberMaterial()
{}


//LIGHT
//*****
void UberMaterial::SetLightDirection(DirectX::XMFLOAT3 direction)
{
	m_LightDirection = direction;
}

//DIFFUSE
//*******
void UberMaterial::EnableDiffuseTexture(bool enable)
{
	m_bDiffuseTexture = enable;
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetDiffuseColor(DirectX::XMFLOAT4 color)
{
	m_ColorDiffuse = color;
}

//SPECULAR
//********
void UberMaterial::SetSpecularColor(DirectX::XMFLOAT4 color)
{
	m_ColorSpecular = color;
}

void UberMaterial::EnableSpecularLevelTexture(bool enable)
{
	m_bSpecularLevelTexture = enable;
}

void UberMaterial::SetSpecularLevelTexture(const std::wstring& assetFile)
{
	m_pSpecularLevelTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetShininess(int shininess)
{
	m_Shininess = shininess;
}

//AMBIENT
//*******
void UberMaterial::SetAmbientColor(DirectX::XMFLOAT4 color)
{
	m_ColorAmbient = color;
}

void UberMaterial::SetAmbientIntensity(float intensity)
{
	m_AmbientIntensity = intensity;
}

//NORMAL MAPPING
//**************
void UberMaterial::FlipNormalGreenCHannel(bool flip)
{
	m_bFlipGreenChannel = flip;
}

void UberMaterial::EnableNormalMapping(bool enable)
{
	m_bNormalMapping = enable;
}

void UberMaterial::SetNormalMapTexture(const std::wstring& assetFile)
{
	m_pNormalMappingTexture = ContentManager::Load<TextureData>(assetFile);
}

//ENVIRONMENT MAPPING
//*******************
void UberMaterial::EnableEnvironmentMapping(bool enable)
{
	m_bEnvironmentMapping = enable;
}

void UberMaterial::SetEnvironmentCube(const std::wstring& assetFile)
{
	m_pEnvironmentCube = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::SetReflectionStrength(float strength)
{
	m_ReflectionStrength = strength;
}

void UberMaterial::SetRefractionStrength(float strength)
{
	m_RefractionStrength = strength;
}

void UberMaterial::SetRefractionIndex(float index)
{
	m_RefractionIndex = index;
}

//OPACITY
//*******
void UberMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

void UberMaterial::EnableOpacityMap(bool enable)
{
	m_bOpacityMap = enable;
}

void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityMap = ContentManager::Load<TextureData>(assetFile);
}

//SPECULAR MODELS
//***************
void UberMaterial::EnableSpecularBlinn(bool enable)
{
	m_bSpecularBlinn = enable;
}

void UberMaterial::EnableSpecularPhong(bool enable)
{
	m_bSpecularPhong = enable;
}

//FRESNEL FALLOFF
//***************
void UberMaterial::EnableFresnelFaloff(bool enable)
{
	m_bFresnelFaloff = enable;
}

void UberMaterial::SetFresnelColor(DirectX::XMFLOAT4 color)
{
	m_ColorFresnel = color;
}

void UberMaterial::SetFresnelPower(float power)
{
	m_FresnelPower = power;
}

void UberMaterial::SetFresnelMultiplier(float multiplier)
{
	m_FresnelMultiplier = multiplier;
}

void UberMaterial::SetFresnelHardness(float hardness)
{
	m_FresnelHardness = hardness;
}

void UberMaterial::LoadEffectVariables()
{
	//LIGHT
	//******
	if (!m_pLightDirectionVariable)
	{
		m_pLightDirectionVariable = GetEffect()->GetVariableByName("gLightDirection")->AsVector();
		if (!m_pLightDirectionVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'m_pLightDirectionVariable\' variable not found!");
			m_pLightDirectionVariable = nullptr;
		}
	}

	//DIFFUSE
	//********
	if (!m_pUseDiffuseTextureVariable)
	{
		m_pUseDiffuseTextureVariable = GetEffect()->GetVariableByName("gUseTextureDiffuse")->AsScalar();
		if (!m_pUseDiffuseTextureVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \' m_pUseDiffuseTextureVariable\' variable not found!");
			m_pUseDiffuseTextureVariable = nullptr;
		}
	}

	if (!m_pDiffuseSRVvariable)
	{
		m_pDiffuseSRVvariable = GetEffect()->GetVariableByName("gTextureDiffuse")->AsShaderResource();
		if (!m_pDiffuseSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \' m_pDiffuseSRVvariable\' variable not found!");
			m_pDiffuseSRVvariable = nullptr;
		}
	}

	if (!m_pDiffuseColorVariable)
	{
		m_pDiffuseColorVariable = GetEffect()->GetVariableByName("gColorDiffuse")->AsVector();
		if (!m_pDiffuseColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \' m_pDiffuseColorVariable\' variable not found!");
			m_pDiffuseColorVariable = nullptr;
		}
	}

	//SPECULAR
	//*********
	if (!m_pSpecularColorVariable)
	{
		m_pSpecularColorVariable = GetEffect()->GetVariableByName("gColorSpecular")->AsVector();
		if (!m_pSpecularColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \' m_pSpecularColorVariable\' variable not found!");
			m_pSpecularColorVariable = nullptr;
		}
	}

	if (!m_pUseSpecularLevelTextureVariable)
	{
		m_pUseSpecularLevelTextureVariable = GetEffect()->GetVariableByName("gUseTextureSpecularIntensity")->AsScalar();
		if (!m_pUseSpecularLevelTextureVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \' m_pUseSpecularLevelTextureVariable\' variable not found!");
			m_pUseSpecularLevelTextureVariable = nullptr;
		}
	}

	if (!m_pSpecularLevelSRVvariable)
	{
		m_pSpecularLevelSRVvariable = GetEffect()->GetVariableByName("gTextureSpecularIntensity")->AsShaderResource();
		if (!m_pSpecularLevelSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'  m_pSpecularLevelSRVvariable\' variable not found!");
			m_pSpecularLevelSRVvariable = nullptr;
		}
	}

	if (!m_pShininessVariable)
	{
		m_pShininessVariable = GetEffect()->GetVariableByName("gShininess")->AsScalar();
		if (!m_pShininessVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'  m_pShininessVariable\' variable not found!");
			m_pShininessVariable = nullptr;
		}
	}

	//AMBIENT
	//********
	if (!m_pAmbientColorVariable)
	{
		m_pAmbientColorVariable = GetEffect()->GetVariableByName("gColorAmbient")->AsVector();
		if (!m_pAmbientColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'  m_pAmbientColorVariable\' variable not found!");
			m_pAmbientColorVariable = nullptr;
		}
	}

	if (!m_pAmbientIntensityVariable)
	{
		m_pAmbientIntensityVariable = GetEffect()->GetVariableByName("gAmbientIntensity")->AsScalar();
		if (!m_pAmbientIntensityVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'  m_pAmbientIntensityVariable\' variable not found!");
			m_pAmbientIntensityVariable = nullptr;
		}
	}

	//NORMAL MAPPING
	//***************
	if (!m_pFlipGreenChannelVariable)
	{
		m_pFlipGreenChannelVariable = GetEffect()->GetVariableByName("gFlipGreenChannel")->AsScalar();
		if (!m_pFlipGreenChannelVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'  m_pFlipGreenChannelVariable\' variable not found!");
			m_pFlipGreenChannelVariable = nullptr;
		}
	}

	if (!m_pUseNormalMappingVariable)
	{
		m_pUseNormalMappingVariable = GetEffect()->GetVariableByName("gUseTextureNormal")->AsScalar();
		if (!m_pUseNormalMappingVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'   m_pUseNormalMappingVariable\' variable not found!");
			m_pUseNormalMappingVariable = nullptr;
		}
	}

	if (!m_pNormalMappingSRVvariable)
	{
		m_pNormalMappingSRVvariable = GetEffect()->GetVariableByName("gTextureNormal")->AsShaderResource();
		if (!m_pNormalMappingSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'   m_pNormalMappingSRVvariable\' variable not found!");
			m_pNormalMappingSRVvariable = nullptr;
		}
	}

	//ENVIRONMENT MAPPING
	//********************
	if (!m_pUseEnvironmentMappingVariable)
	{
		m_pUseEnvironmentMappingVariable = GetEffect()->GetVariableByName("gUseTextureEnvironment")->AsScalar();
		if (!m_pUseEnvironmentMappingVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'   m_pUseEnvironmentMappingVariable\' variable not found!");
			m_pUseEnvironmentMappingVariable = nullptr;
		}
	}

	if (!m_pEnvironmentSRVvariable)
	{
		m_pEnvironmentSRVvariable = GetEffect()->GetVariableByName("gCubeEnvironment")->AsShaderResource();
		if (!m_pEnvironmentSRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'   m_pEnvironmentSRVvariable\' variable not found!");
			m_pEnvironmentSRVvariable = nullptr;
		}
	}

	if (!m_pReflectionStrengthVariable)
	{
		m_pReflectionStrengthVariable = GetEffect()->GetVariableByName("gReflectionStrength")->AsScalar();
		if (!m_pReflectionStrengthVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'    m_pReflectionStrengthVariable\' variable not found!");
			m_pReflectionStrengthVariable = nullptr;
		}
	}

	if (!m_pRefractionStrengthVariable)
	{
		m_pRefractionStrengthVariable = GetEffect()->GetVariableByName("gRefractionStrength")->AsScalar();
		if (!m_pRefractionStrengthVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'    m_pRefractionStrengthVariable\' variable not found!");
			m_pRefractionStrengthVariable = nullptr;
		}
	}

	if (!m_pRefractionIndexVariable)
	{
		m_pRefractionIndexVariable = GetEffect()->GetVariableByName("gRefractionIndex")->AsScalar();
		if (!m_pRefractionIndexVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'    m_pRefractionIndexVariable\' variable not found!");
			m_pRefractionIndexVariable = nullptr;
		}
	}

	//OPACITY
	//********
	if (!m_pOpacityVariable)
	{
		m_pOpacityVariable = GetEffect()->GetVariableByName("gOpacityIntensity")->AsScalar();
		if (!m_pOpacityVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'    m_pOpacityVariable\' variable not found!");
			m_pOpacityVariable = nullptr;
		}
	}

	if (!m_pUseOpacityMapVariable)
	{
		m_pUseOpacityMapVariable = GetEffect()->GetVariableByName("gUseTextureOpacity")->AsScalar();
		if (!m_pUseOpacityMapVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'    m_pUseOpacityMapVariable\' variable not found!");
			m_pUseOpacityMapVariable = nullptr;
		}
	}

	if (!m_pOpacitySRVvariable)
	{
		m_pOpacitySRVvariable = GetEffect()->GetVariableByName("gTextureOpacity")->AsShaderResource();
		if (!m_pOpacitySRVvariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'     m_pOpacitySRVvariable\' variable not found!");
			m_pOpacitySRVvariable = nullptr;
		}
	}

	//SPECULAR MODELS
	//****************
	if (!m_pUseBlinnVariable)
	{
		m_pUseBlinnVariable = GetEffect()->GetVariableByName("gUseSpecularBlinn")->AsScalar();
		if (!m_pUseBlinnVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'      m_pUseBlinnVariable\' variable not found!");
			m_pUseBlinnVariable = nullptr;
		}
	}

	if (!m_pUsePhongVariable)
	{
		m_pUsePhongVariable = GetEffect()->GetVariableByName("gUseSpecularPhong")->AsScalar();
		if (!m_pUsePhongVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'      m_pUsePhongVariable\' variable not found!");
			m_pUsePhongVariable = nullptr;
		}
	}

	//FRESNEL FALLOFF
	//***************
	if (!m_pUseFresnelFalloffVariable)
	{
		m_pUseFresnelFalloffVariable = GetEffect()->GetVariableByName("gUseFresnelFalloff")->AsScalar();
		if (!m_pUseFresnelFalloffVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'       m_pUseFresnelFalloffVariable\' variable not found!");
			m_pUseFresnelFalloffVariable = nullptr;
		}
	}

	if (!m_pFresnelColorVariable)
	{
		m_pFresnelColorVariable = GetEffect()->GetVariableByName("gColorFresnel")->AsVector();
		if (!m_pFresnelColorVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'       m_pFresnelColorVariable\' variable not found!");
			m_pFresnelColorVariable = nullptr;
		}
	}

	if (!m_pFresnelPowerVariable)
	{
		m_pFresnelPowerVariable = GetEffect()->GetVariableByName("gFresnelPower")->AsScalar();
		if (!m_pFresnelPowerVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'       m_pFresnelPowerVariable\' variable not found!");
			m_pFresnelPowerVariable = nullptr;
		}
	}

	if (!m_pFresnelMultiplierVariable)
	{
		m_pFresnelMultiplierVariable = GetEffect()->GetVariableByName("gFresnelMultiplier")->AsScalar();
		if (!m_pFresnelMultiplierVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'       m_pFresnelMultiplierVariable\' variable not found!");
			m_pFresnelMultiplierVariable = nullptr;
		}
	}

	if (!m_pFresnelHardnessVariable)
	{
		m_pFresnelHardnessVariable = GetEffect()->GetVariableByName("gFresnelHardness")->AsScalar();
		if (!m_pFresnelHardnessVariable->IsValid())
		{
			Logger::LogWarning(L"UberMaterial::LoadEffectVariables() > \'       m_pFresnelHardnessVariable\' variable not found!");
			m_pFresnelHardnessVariable = nullptr;
		}
	}
}

void UberMaterial::UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent)
{
	using namespace DirectX;

	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(pModelComponent);

	//LIGHT
	//*****
	if (m_pLightDirectionVariable)
		m_pLightDirectionVariable->GetFloatVector(&m_LightDirection.x);

	//DIFFUSE
	//*******
	if (m_bDiffuseTexture)
	{
		if (m_pUseDiffuseTextureVariable)
			m_pUseDiffuseTextureVariable->SetBool(m_bDiffuseTexture);

		if (m_pDiffuseSRVvariable)
			m_pDiffuseSRVvariable->SetResource(m_pDiffuseTexture->GetShaderResourceView());

		if (m_pDiffuseColorVariable)
			m_pDiffuseColorVariable->SetFloatVector(&m_ColorDiffuse.x);
	}

	//SPECULAR
	//********
	if (m_bSpecularLevelTexture)
	{
		if (m_pSpecularColorVariable)
			m_pSpecularColorVariable->SetFloatVector(&m_ColorSpecular.x);

		if (m_pUseSpecularLevelTextureVariable)
			m_pUseSpecularLevelTextureVariable->SetBool(m_bSpecularLevelTexture);

		if (m_pSpecularLevelSRVvariable)
			m_pSpecularLevelSRVvariable->SetResource(m_pSpecularLevelTexture->GetShaderResourceView());

		if (m_pShininessVariable)
			m_pShininessVariable->SetInt(m_Shininess);
	}
	//AMBIENT
	//*******

	if (m_pAmbientColorVariable)
		m_pAmbientColorVariable->SetFloatVector(&m_ColorAmbient.x);

	if (m_pAmbientIntensityVariable)
		m_pAmbientIntensityVariable->SetFloat(m_AmbientIntensity);

	//NORMAL MAPPING
	//**************
	if (m_bNormalMapping)
	{
		if (m_pFlipGreenChannelVariable)
			m_pFlipGreenChannelVariable->SetBool(m_bFlipGreenChannel);

		if (m_pUseNormalMappingVariable)
			m_pUseNormalMappingVariable->SetBool(m_bNormalMapping);

		if (m_pNormalMappingSRVvariable)
			m_pNormalMappingSRVvariable->SetResource(m_pNormalMappingTexture->GetShaderResourceView());
	}
	//ENVIRONMENT MAPPING
	//*******************
	if (m_bEnvironmentMapping)
	{
		if (m_pUseEnvironmentMappingVariable)
			m_pUseEnvironmentMappingVariable->SetBool(m_bEnvironmentMapping);

		if (m_pEnvironmentSRVvariable)
			m_pEnvironmentSRVvariable->SetResource(m_pEnvironmentCube->GetShaderResourceView());

		if (m_pReflectionStrengthVariable)
			m_pReflectionStrengthVariable->SetFloat(m_ReflectionStrength);

		if (m_pRefractionStrengthVariable)
			m_pRefractionStrengthVariable->SetFloat(m_RefractionStrength);

		if (m_pRefractionIndexVariable)
			m_pRefractionIndexVariable->SetFloat(m_RefractionIndex);
	}
	//OPACITY
	//*******
	if (m_bOpacityMap)
	{
		if (m_pOpacityVariable)
			m_pOpacityVariable->SetFloat(m_Opacity);

		if (m_pUseOpacityMapVariable)
			m_pUseOpacityMapVariable->SetBool(m_bOpacityMap);

		if (m_pOpacitySRVvariable)
			m_pOpacitySRVvariable->SetResource(m_pOpacityMap->GetShaderResourceView());
	}
	//SPECULAR MODELS
	//***************
	if (m_bSpecularBlinn)
	{
		if (m_pUseBlinnVariable)
			m_pUseBlinnVariable->SetBool(m_bSpecularBlinn);
	}
	if (m_bSpecularPhong)
	{
		if (m_pUsePhongVariable)
			m_pUsePhongVariable->SetBool(m_bSpecularPhong);
	}
	//FRESNEL FALLOFF
	//***************
	if (m_bFresnelFaloff)
	{
		if (m_pUseFresnelFalloffVariable)
			m_pUseFresnelFalloffVariable->SetBool(m_bFresnelFaloff);

		if (m_pFresnelColorVariable)
			m_pFresnelColorVariable->SetFloatVector(&m_ColorFresnel.x);

		if (m_pFresnelPowerVariable)
			m_pFresnelPowerVariable->SetFloat(m_FresnelPower);

		if (m_pFresnelMultiplierVariable)
			m_pFresnelMultiplierVariable->SetFloat(m_FresnelMultiplier);

		if (m_pFresnelHardnessVariable)
			m_pFresnelHardnessVariable->SetFloat(m_FresnelHardness);
	}
}




















































