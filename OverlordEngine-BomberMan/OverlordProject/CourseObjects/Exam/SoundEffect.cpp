#include "stdafx.h"
#include "SoundEffect.h"
#include "SoundManager.h"

SoundEffect::SoundEffect(const std::string& path, const std::string& name)
	:m_Name(name)
{
	SoundManager::GetInstance()->GetSystem()->createSound(path.c_str(), FMOD_DEFAULT, 0, &m_pSoundStream);
	m_pSoundStream->setMode(FMOD_LOOP_OFF);
	SoundManager::GetInstance()->GetSystem()->playSound(m_pSoundStream, 0, false, &m_pChannel);
	m_pChannel->setVolume(m_Volume);
	m_pChannel->setPaused(true);
}

SoundEffect::~SoundEffect()
{
}

void SoundEffect::Play()
{
	SoundManager::GetInstance()->GetSystem()->playSound(m_pSoundStream, 0, false, &m_pChannel);
	m_pChannel->setVolume(m_Volume);
}

void SoundEffect::SetVolume(float amount)
{
	m_Volume = amount;
	m_pChannel->setVolume(amount);
}
