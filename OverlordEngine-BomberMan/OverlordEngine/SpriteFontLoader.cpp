#include "stdafx.h"
#include "SpriteFontLoader.h"
#include "BinaryReader.h"
#include "ContentManager.h"
#include "TextureData.h"
#include <bitset>

SpriteFont* SpriteFontLoader::LoadContent(const std::wstring& assetFile)
{
	auto pBinReader = new BinaryReader();
	pBinReader->Open(assetFile);

	if (!pBinReader->Exists())
	{
		Logger::LogFormat(LogLevel::Error, L"SpriteFontLoader::LoadContent > Failed to read the assetFile!\nPath: \'%s\'", assetFile.c_str());
		return nullptr;
	}

	//See BMFont Documentation for Binary Layout

	//Parse the Identification bytes (B,M,F)
	//If Identification bytes doesn't match B|M|F,
	unsigned char B = pBinReader->Read<BYTE>();
	unsigned char M = pBinReader->Read<BYTE>();
	unsigned char F = pBinReader->Read<BYTE>();
	if (B != 'B' || M != 'M' || F != 'F')
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > not a valid .fnt font");
		return nullptr;
	}
	//Log Error (SpriteFontLoader::LoadContent > Not a valid .fnt font) &
	//return nullptr
	//...

	//Parse the version (version 3 required)
	int version = static_cast<int>(pBinReader->Read<BYTE>());
	if (version != 3)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > Only .fnt version 3 is supported");
		return nullptr;
	}
	//If version is < 3,
	//Log Error (SpriteFontLoader::LoadContent > Only .fnt version 3 is supported)
	//return nullptr
	//...

	//Valid .fnt file
	auto pSpriteFont = new SpriteFont();
	
	//SpriteFontLoader is a friend class of SpriteFont
	//That means you have access to its privates (pSpriteFont->m_FontName = ... is valid)

	//**********
	// BLOCK 1 *
	//**********
	//Retrieve the blockId and blockSize
	unsigned char blockId = (pBinReader->Read<BYTE>());
	int blockSize = pBinReader->Read<INT32>();
	short FontSize = pBinReader->Read<short>();
	pSpriteFont->m_FontSize = FontSize;
	pBinReader->MoveBufferPosition(12);
	std::wstring name = pBinReader->ReadNullString();
	pSpriteFont->m_FontName = name;
	//Retrieve the FontSize (will be -25, BMF bug) [SpriteFont::m_FontSize]
	//Move the binreader to the start of the FontName [BinaryReader::MoveBufferPosition(...) or you can set its position using BinaryReader::SetBufferPosition(...))
	//Retrieve the FontName [SpriteFont::m_FontName]
	//...


	//**********
	// BLOCK 2 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<BYTE>();
	blockSize = pBinReader->Read<INT32>();
	pBinReader->MoveBufferPosition(4);
	pSpriteFont->m_TextureWidth = static_cast<int>(pBinReader->Read<UINT16>());
	pSpriteFont->m_TextureHeight = static_cast<int>(pBinReader->Read<UINT16>());
	//Retrieve Texture Width & Height [SpriteFont::m_TextureWidth/m_TextureHeight]
	//Retrieve PageCount
	if (static_cast<int>(pBinReader->Read<UINT16>()) != 1)
	{
		Logger::LogError(L"SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed");
	}
	pBinReader->MoveBufferPosition(5);
	//> if pagecount > 1
	//> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Only one texture per font allowed)
	//Advance to Block2 (Move Reader)
	//...

	//**********
	// BLOCK 3 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<BYTE>();
	blockSize = pBinReader->Read<INT32>();
	std::wstring pageName = pBinReader->ReadNullString();
	//Retrieve the PageName (store Local)
	if (pageName.empty())
	{
		Logger::LogError(L"SprinteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty]");
	}
	//	> If PageName is empty
	//	> Log Error (SpriteFontLoader::LoadContent > SpriteFont (.fnt): Invalid Font Sprite [Empty])
	//>Retrieve texture filepath from the assetFile path
	//> (ex. c:/Example/somefont.fnt => c:/Example/) [Have a look at: wstring::rfind()]
	std::wstring newPath = assetFile;
	std::wstring::size_type found = newPath.rfind(pSpriteFont->GetFontName());
	if (found != std::wstring::npos)
	{
		std::wstring temp = newPath.substr(found);
		newPath.replace(found, temp.size(), pageName);
	}
	//>Use path and PageName to load the texture using the ContentManager [SpriteFont::m_pTexture]
	//> (ex. c:/Example/ + 'PageName' => c:/Example/somefont_0.png)
	//...
	pSpriteFont->m_pTexture = ContentManager::Load<TextureData>(newPath);

	//**********
	// BLOCK 4 *
	//**********
	//Retrieve the blockId and blockSize
	blockId = pBinReader->Read<BYTE>();
	blockSize = pBinReader->Read<INT32>();
	//Retrieve Character Count (see documentation)
	int numChars = blockSize / 20;
	//Retrieve Every Character, For every Character:
	for (int i = 0; i < numChars; ++i)
	{
		//> Retrieve CharacterId (store Local) and cast to a 'wchar_t'
		wchar_t CharacterId = static_cast<wchar_t>(pBinReader->Read<UINT32>());
		if (!pSpriteFont->IsCharValid(CharacterId))
		{
			Logger::Log(LogLevel::Warning, L"CharacterId is InValid");
			continue;
		}
		//> Check if CharacterId is valid (SpriteFont::IsCharValid), Log Warning and advance to next character if not valid
		//> Retrieve the corresponding FontMetric (SpriteFont::GetMetric) [REFERENCE!!!]
		FontMetric &charMetric = pSpriteFont->GetMetric(CharacterId);
		//> Set IsValid to true [FontMetric::IsValid]
		charMetric.IsValid = true;
		//> Set Character (CharacterId) [FontMetric::Character]
		charMetric.Character = CharacterId;
		//> Retrieve Xposition (store Local)
		const unsigned short Xpos = pBinReader->Read<unsigned short>();
		//> Retrieve Yposition (store Local)
		const unsigned short Ypos = pBinReader->Read<unsigned short>();
		//> Retrieve & Set Width [FontMetric::Width]
		charMetric.Width = pBinReader->Read<unsigned short>();
		//> Retrieve & Set Height [FontMetric::Height]
		charMetric.Height = pBinReader->Read<unsigned short>();
		//> Retrieve & Set OffsetX [FontMetric::OffsetX]
		charMetric.OffsetX = pBinReader->Read<short>();
		//> Retrieve & Set OffsetY [FontMetric::OffsetY]
		charMetric.OffsetY = pBinReader->Read<short>();
		//> Retrieve & Set AdvanceX [FontMetric::AdvanceX]
		charMetric.AdvanceX = pBinReader->Read<short>();
		//> Retrieve & Set Page [FontMetric::Page]
		charMetric.Page = pBinReader->Read<BYTE>();
		//> Retrieve Channel (BITFIELD!!!) 
		std::bitset<4> channel = pBinReader->Read<unsigned char>();
		charMetric.Channel = channel[2] * 0 + channel[1] * 1 + channel[0] * 2 + channel[3] * 3;
		//	> See documentation for BitField meaning [FontMetrix::Channel]
		//> Calculate Texture Coordinates using Xposition, Yposition, TextureWidth & TextureHeight [FontMetric::TexCoord]
		charMetric.TexCoord = DirectX::XMFLOAT2(Xpos / static_cast<float>(pSpriteFont->m_TextureWidth), Ypos / static_cast<float>(pSpriteFont->m_TextureHeight));
		//...
	}
	//DONE :)
	UNREFERENCED_PARAMETER(blockSize);
	UNREFERENCED_PARAMETER(blockId);

	delete pBinReader;
	return pSpriteFont;
}

void SpriteFontLoader::Destroy(SpriteFont* objToDestroy)
{
	SafeDelete(objToDestroy);
}
