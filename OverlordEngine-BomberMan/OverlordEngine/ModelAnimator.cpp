#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
m_pMeshFilter(pMeshFilter),
m_Transforms(std::vector<DirectX::XMFLOAT4X4>()),
m_IsPlaying(false), 
m_Reversed(false),
m_ClipSet(false),
m_TickCount(0),
m_AnimationSpeed(1.0f)
{
	SetAnimation(0);
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	m_ClipSet = false;
	if (clipNumber < m_pMeshFilter->m_AnimationClips.size())
	{
		m_CurrentClip = m_pMeshFilter->m_AnimationClips[clipNumber];
		SetAnimation(m_CurrentClip);
	}
	else
	{
		Reset();
		Logger::Log(LogLevel::Warning, L"Clipnumber is bigger then the actual size of m_AnimationClips");
	}
}

void ModelAnimator::SetAnimation(std::wstring clipName)
{
	m_ClipSet = false;
	for (unsigned int i = 0; i < m_pMeshFilter->m_AnimationClips.size(); ++i)
	{
		if (m_pMeshFilter->m_AnimationClips[i].Name == clipName)
		{
			m_CurrentClip = m_pMeshFilter->m_AnimationClips[i];
			SetAnimation(m_CurrentClip);
			return;
		}
	}
	Reset();
	Logger::Log(LogLevel::Warning, L"Clipnumber is bigger then the actual size of m_AnimationClips");
}

void ModelAnimator::SetAnimation(AnimationClip clip)
{
	m_ClipSet = true;
	m_CurrentClip = clip;
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	UNREFERENCED_PARAMETER(pause);
	//TODO: complete
	//If pause is true, set m_IsPlaying to false
	if (pause)
		m_IsPlaying = false;
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;
	//If m_ClipSet is true
	if (m_ClipSet)
	{
		auto boneTransform = m_CurrentClip.Keys[0].BoneTransforms;
		m_Transforms.assign(boneTransform.begin(), boneTransform.end());
	}
	else
	{
		DirectX::XMFLOAT4X4 identity{};
		DirectX::XMStoreFloat4x4(&identity, DirectX::XMMatrixIdentity());
		m_Transforms.assign(m_Transforms.size(), identity);
	}
}

void ModelAnimator::Update(const GameContext& gameContext)
{
	//TODO: complete
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		auto passedTicks = gameContext.pGameTime->GetElapsed() * m_CurrentClip.TicksPerSecond * m_AnimationSpeed;
		passedTicks = fmod(passedTicks, m_CurrentClip.Duration);

		//2. 
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
				m_TickCount = m_CurrentClip.Duration;
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.Duration)
				m_TickCount -= m_CurrentClip.Duration;
		}
		
		//3.
		AnimationKey keyA, keyB, keyToFind;
		keyToFind.Tick = m_TickCount;
		auto pred = [](AnimationKey& key1, AnimationKey key2) {return key1.Tick < key2.Tick; };
		auto it = std::lower_bound(m_CurrentClip.Keys.begin(), m_CurrentClip.Keys.end(), keyToFind, pred);
		keyB = *it;
		keyA = *prev(it);

		//4.
		//Interpolate between keys

		//Figure out the BlendFactor (See lab document)
		float blendFactor = (m_TickCount - keyA.Tick) / (keyB.Tick - keyA.Tick);
		//Clear the m_Transforms vector
		m_Transforms.clear();
		//FOR every boneTransform in a key (So for every bone)
		for (unsigned int i = 0; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			//	Retrieve the transform from keyA (transformA)
			auto transformA = keyA.BoneTransforms[i];
			// 	Retrieve the transform from keyB (transformB)
			auto transformB = keyB.BoneTransforms[i];
			//	Decompose both transforms
			DirectX::XMVECTOR transA, transB, scaleA, scaleB, rotA, rotB;

			DirectX::XMMatrixDecompose(&scaleA, &rotA, &transA, DirectX::XMLoadFloat4x4(&transformA));
			DirectX::XMMatrixDecompose(&scaleB, &rotB, &transB, DirectX::XMLoadFloat4x4(&transformB));

			transA = DirectX::XMVectorLerp(transA, transB, blendFactor);
			scaleA = DirectX::XMVectorLerp(scaleA, scaleB, blendFactor);
			rotA = DirectX::XMQuaternionSlerp(rotA, rotB, blendFactor);
			//	Lerp between all the transformations (Position, Scale, Rotation)
			//	Compose a transformation matrix with the lerp-results
			DirectX::XMMATRIX trans = DirectX::XMMatrixTranslationFromVector(transA);
			DirectX::XMMATRIX scale = DirectX::XMMatrixScalingFromVector(scaleA);
			DirectX::XMMATRIX rot = DirectX::XMMatrixRotationQuaternion(rotA);
			DirectX::XMMATRIX compose = scale * rot * trans;
			DirectX::XMFLOAT4X4 m{};
			DirectX::XMStoreFloat4x4(&m, compose);
			//	Add the resulting matrix to the m_Transforms vector
			m_Transforms.push_back(m);
		}

	}
}
