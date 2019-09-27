#pragma once
#include "Singleton.h"
#include "../OverlordProject/CourseObjects/Exam/SoundStream.h"
#include "../OverlordProject/CourseObjects/Exam/SoundEffect.h"

class SoundManager: public Singleton<SoundManager>
{
	void Initialize();
	FMOD::System* m_pFmodSystem = nullptr;

public:
	SoundManager(const SoundManager& other) = delete;
	SoundManager(SoundManager&& other) noexcept = delete;
	SoundManager& operator=(const SoundManager& other) = delete;
	SoundManager& operator=(SoundManager&& other) noexcept = delete;
	SoundManager();
	virtual ~SoundManager();

	void AddSoundStream(SoundStream* strm) { m_pSoundStreamVec.push_back(strm); }
	SoundStream* GetStream(const std::string& name);

	void AddSoundEffect(SoundEffect* ffct) { m_pSoundEffectVec.push_back(ffct); }
	SoundEffect* GetEffect(const std::string& name);

	static bool ErrorCheck(FMOD_RESULT res);
	FMOD::System* GetSystem() const { return m_pFmodSystem; }

	void ChangeVolumeSoundEffect(float amount);
	void ChangeVolumeSoundStream(float amount);

private:
	std::vector<SoundStream*> m_pSoundStreamVec;
	std::vector<SoundEffect*> m_pSoundEffectVec;
	float m_EffectVolume = 0.5f;
	float m_StreamVolume = 0.2f;
};

