#include "spriteComponent.h"
#include "Framework/actor.h"
#include "Renderer/texture.h"

namespace neu
{
	bool SpriteComponent::Write(const rapidjson::Value& value) const
	{
		return false;
	}

	bool SpriteComponent::Read(const rapidjson::Value& value)
	{
		std::string texture_name;
		READ_DATA(value, texture_name);
		READ_DATA(value, paralax);

		m_texture = g_resourceManager.Get<Texture>(texture_name, g_renderer);

		if (!READ_DATA(value, source))
		{
			source.x = 0;
			source.y = 0;
			source.w = (int)m_texture->GetSize().x;
			source.h = (int)m_texture->GetSize().y;
		}

		return true;
	}
	
	void SpriteComponent::Update()
	{
		//
	}

	void SpriteComponent::Draw(Renderer& renderer)
	{
		renderer.Draw(m_texture, GetSource(), m_owner->GetTransform(), registration, flipHorizontal, paralax);
	}
}
