#include "stdafx.h"
#include "SoundStream.h"
#include "SoundManager.h"

SoundStream::SoundStream(const std::string& path, const std::string& name)
	:m_Name(name)
{
	SoundManager::GetInstance()->GetSystem()->createSound(path.c_str(), FMOD_DEFAULT, 0, &m_pSoundStream);
	m_pSoundStream->setMode(FMOD_LOOP_NORMAL);
	SoundManager::GetInstance()->GetSystem()->playSound(m_pSoundStream, 0, false, &m_pChannel);
	m_pChannel->setVolume(m_Volume);
	Pause();
}

SoundStream::~SoundStream()
{
}

void SoundStream::SetVolume(float amount)
{
	m_Volume = amount;
	m_pChannel->setVolume(m_Volume);
}
