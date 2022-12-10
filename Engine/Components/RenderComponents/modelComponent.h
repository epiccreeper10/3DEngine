#pragma once
#include "renderComponent.h"
#include "Renderer/material.h"

namespace neu
{
	class Model;

	class ModelComponent : public RenderComponent
	{
	public:

		CLASS_CLONE(ModelComponent);

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;
		
		virtual void Update() override;
		virtual void Draw(Renderer& renderer) override;

	public:
		std::shared_ptr<Model> model;
		std::shared_ptr<Material> material;

	};
}