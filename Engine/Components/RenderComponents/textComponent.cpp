#include "textComponent.h"
#include "Renderer/texture.h"
#include "Renderer/font.h"
#include "Components/includes.h"
#include "Framework/actor.h"

namespace neu 
{
	bool TextComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool TextComponent::Read(const rapidjson::Value& value)
	{
		READ_DATA(value, text);
		READ_DATA(value, font_name);
		READ_DATA(value, font_size);
		READ_DATA(value, registration);
		READ_DATA(value, color);

		m_font = g_resourceManager.Get<Font>(font_name, font_size);

		m_texture = std::make_unique<Texture>();

		SetText(text);
		
		return true;
	}

	void TextComponent::Update()
	{
		//
	}

	void TextComponent::Draw(Renderer& renderer)
	{
		g_renderer.Draw(m_texture, m_owner->GetTransform(), registration);
	}

	void TextComponent::SetText(const std::string& text)
	{
		m_texture->CreateFromSurface(m_font->CreateSurface(text, color), g_renderer);
	}
}