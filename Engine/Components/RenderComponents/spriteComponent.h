#pragma once
#include "renderComponent.h"
#include "Math/rect.h"

namespace neu
{
	class Texture;

	class SpriteComponent : public RenderComponent
	{
	public:

		CLASS_CLONE(SpriteComponent);

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

		virtual void Update() override;
		virtual void Draw(Renderer& renderer) override;

	public:
		std::shared_ptr<Texture> m_texture;
		Vector2 paralax = Vector2::ONE;
	};
}