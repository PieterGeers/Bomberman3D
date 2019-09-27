#pragma once
class SoundEffect
{
public:
	SoundEffect(const std::string& path, const std::string& name);
	~SoundEffect();

	void Play();
	void SetVolume(float amount);

	FMOD::Channel* m_pChannel;
	FMOD::Sound* m_pSoundStream;
	std::string m_Name;
	float m_Volume = .5f;
};

