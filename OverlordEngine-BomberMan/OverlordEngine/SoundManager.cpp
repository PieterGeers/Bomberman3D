#include "stdafx.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
	Initialize();
}

SoundManager::~SoundManager()
{
	if(m_pFmodSystem)
	{
		m_pFmodSystem->release();
	}
	for (unsigned i = 0; i != m_pSoundStreamVec.size(); ++i)
	{
		delete m_pSoundStreamVec[i];
	}
	for (unsigned i = 0; i != m_pSoundEffectVec.size(); ++i)
	{
		delete m_pSoundEffectVec[i];
	}
}

SoundStream* SoundManager::GetStream(const std::string& name)
{
	auto it = std::find_if(m_pSoundStreamVec.begin(), m_pSoundStreamVec.end(), [name](SoundStream* s) {return s->m_Name == name; });
	if (it != m_pSoundStreamVec.end())
		return (*it);
	return nullptr;
}

SoundEffect* SoundManager::GetEffect(const std::string& name)
{
	auto it = std::find_if(m_pSoundEffectVec.begin(), m_pSoundEffectVec.end(), [name](SoundEffect* e) {return e->m_Name == name; });
	if (it != m_pSoundEffectVec.end())
		return (*it);
	return nullptr;
}

bool SoundManager::ErrorCheck(FMOD_RESULT res)
{
	if(res != FMOD_OK)
	{
	 std::wstringstream strstr;
		strstr << L"FMOD error! \n[" << res << L"] "<< FMOD_ErrorString(res) << std::endl;
		Logger::LogError(strstr.str());
		return false;
	}

	return true;
}

void SoundManager::ChangeVolumeSoundEffect(float amount)
{
	m_EffectVolume += amount;
	if (m_EffectVolume < 0.0f)
		m_EffectVolume = 0.0f;
	if (m_EffectVolume > 1.0f)
		m_EffectVolume = 1.0f;

	for (auto effect : m_pSoundEffectVec)
		effect->SetVolume(m_EffectVolume);
}

void SoundManager::ChangeVolumeSoundStream(float amount)
{
	m_StreamVolume += amount;
	if (m_StreamVolume < 0.0f)
		m_StreamVolume = 0.0f;
	if (m_StreamVolume > 1.0f)
		m_StreamVolume = 1.0f;

	for (auto stream : m_pSoundStreamVec)
		stream->SetVolume(m_StreamVolume);
}

void SoundManager::Initialize()
{
	unsigned int version;
	int numdrivers;
	
	/*
	Create a System object and initialize.
	*/
	FMOD_RESULT result = FMOD::System_Create(&m_pFmodSystem);
	ErrorCheck(result);
	result = m_pFmodSystem->getVersion(&version);
	ErrorCheck(result);
	
	if (version < FMOD_VERSION)
	{
		Logger::LogFormat(LogLevel::Error, L"SoundManager Initialization Failed!\n\nYou are using an old version of FMOD %08x. This program requires %08x\n",
		version, FMOD_VERSION);
		return;
	}

	result = m_pFmodSystem->getNumDrivers(&numdrivers);
	ErrorCheck(result);
	if (numdrivers == 0)
	{
		result = m_pFmodSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ErrorCheck(result);
	}
	else
	{
		result = m_pFmodSystem->init(32, FMOD_INIT_NORMAL, nullptr);
		ErrorCheck(result);
	}

}
