#pragma once
#include "Material.h"

class TextureData;

class SkyBoxMaterial final: public Material
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial() = default;
	void SetSkyBoxMaterial(const std::wstring& assetFile);

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(const GameContext& gameContext, ModelComponent* pModelComponent) override;

private:
	TextureData* m_pDiffuseTexture;
	static ID3DX11EffectShaderResourceVariable* m_pDiffuseSRVvariable;

	//-----------------------------
	//Disabling default copy constructor and default assignment operator.
	//-----------------------------
	SkyBoxMaterial(const SkyBoxMaterial &obj) = delete;
	SkyBoxMaterial& operator=(const SkyBoxMaterial& obj) = delete;

};

