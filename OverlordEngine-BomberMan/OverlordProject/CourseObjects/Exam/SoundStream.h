#pragma once

class SoundStream
{
public:
	SoundStream(const std::string& path, const std::string& name);
	~SoundStream();
	void Pause()
	{
		m_pChannel->setPaused(true);
	}
	void Resume()
	{
		m_pChannel->setPaused(false);
	}
	void SetVolume(float amount);
	FMOD::Channel* m_pChannel;
	FMOD::Sound* m_pSoundStream;
	std::string m_Name;
	float m_Volume = 0.2f;
};
