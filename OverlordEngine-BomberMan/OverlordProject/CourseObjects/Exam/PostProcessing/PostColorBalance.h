#pragma once
#include "PostProcessingMaterial.h"

class ID3D11EffectShaderResourceVariable;

class PostColorBalance final : public PostProcessingMaterial
{
public:
	PostColorBalance(float redPercent, float greenPercent, float bluePercent);
	virtual ~PostColorBalance() = default;

	PostColorBalance(const PostColorBalance& other) = delete;
	PostColorBalance(PostColorBalance&& other) noexcept = delete;
	PostColorBalance& operator=(const PostColorBalance& other) = delete;
	PostColorBalance& operator=(PostColorBalance&& other) noexcept = delete;

protected:
	void LoadEffectVariables() override;
	void UpdateEffectVariables(RenderTarget* pRendertarget) override;

private:
	ID3DX11EffectShaderResourceVariable* m_pTextureMapVariabele = nullptr;
	ID3DX11EffectScalarVariable* m_pRed = nullptr;
	ID3DX11EffectScalarVariable* m_pGreen = nullptr;
	ID3DX11EffectScalarVariable* m_pBlue = nullptr;
	float m_Red, m_Green, m_Blue;
};

