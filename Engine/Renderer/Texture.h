#pragma once
#include "Math/Vector2.h"
#include "Renderer.h"
#include "Resource/Resource.h"
#include <string>

struct SDL_Texture;
struct SDL_Surface;

namespace neu
{
	// !! forward declaration for Renderer below
	class Renderer;

	class Texture : public Resource
	{
	public:
		Texture() = default;
		~Texture();

		bool Create(std::string filename, ...) override;
		bool CreateFromSurface(SDL_Surface* surface, Renderer& renderer);

		void Bind() { glBindTexture(m_target, m_texture); }

		bool Load(const std::string& filename, Renderer& renderer);

		Vector2 GetSize() const;

		friend class Renderer;

	private:
		GLuint m_texture = 0;
		GLenum m_target = GL_TEXTURE_2D;
		GLuint m_unit = GL_TEXTURE0;
		//SDL_Texture* m_texture = nullptr;
	};
}
