#pragma once
#include "renderComponent.h"
#include "Math/color.h"
#include "Math/vector2.h"
#include <memory>

namespace neu
{
	class Font;
	class Texture;

	class TextComponent : public RenderComponent
	{
	public:

		CLASS_CLONE(TextComponent);

		virtual bool Write(const rapidjson::Value& value) const;
		virtual bool Read(const rapidjson::Value& value);

		virtual void Update() override;
		virtual void Draw(Renderer& renderer) override;

		void SetText(const std::string& text);
		
	public:
		std::string text;
		std::string font_name;
		int font_size;
		Vector2 registration;
		Color color;

		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture> m_texture;
	};
}