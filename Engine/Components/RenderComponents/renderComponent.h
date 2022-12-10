#pragma once
#include "Components/includes.h"
#include "Math/rect.h"
#include "Math/vector2.h"

namespace neu
{
	class Renderer;

	class RenderComponent : public Component
	{
	public:
		virtual void Draw(Renderer& renderer) = 0;

		virtual Rect& GetSource() { return source; }
		void SetFlipHorizontal(bool flip = true) { flipHorizontal = flip; }
		bool GetFlipHorizontal() { return flipHorizontal; }

	protected:
		Rect source;
		bool flipHorizontal{ false };
		Vector2 registration = Vector2{ 0.5f, 0.5f };
	};
}