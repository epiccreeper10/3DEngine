#include "model.h"
#include "Core/file.h"
#include "Core/logger.h"
#include "Math/mathUtils.h"
#include <sstream>
#include <iostream>

namespace neu
{
	Model::Model(const std::string& filename)
	{
		Load(filename);
		m_radius = CalculateRadius();
	}

	bool Model::Create(std::string filename, ...)
	{

		if (!Load(filename))
		{
			LOG("Error could not create model : %s", filename.c_str());
			return false;
		}
		return true;
	}

	void Model::Draw(Renderer& renderer, const Vector2& position, float angle, const Vector2& scale)
	{
		if (!m_points.size()) return;
		for (int i = 0; i < m_points.size(); i++)
		{
			neu::Vector2 p1 = Vector2::Rotate((m_points[i] * scale), angle) + position;
			neu::Vector2 p2 = Vector2::Rotate((m_points[(i + 1) % m_points.size()] * scale), angle) + position;

			renderer.DrawLine(p1, p2, m_color);
		}
	}
	void Model::Draw(Renderer& renderer, const Transform& transform)
	{

		////Matrix3x3 mx = transform.matrix;
		////if (!m_points.size()) return;

		//for (int i = 0; i < m_points.size(); i++)
		//{
		//	vl::Vector2 p1 = mx * m_points[i];
		//	vl::Vector2 p2 = mx * m_points[(i + 1) % m_points.size()];

		//	renderer.DrawLine(p1, p2, m_color);
		//}
	}
	bool Model::Load(const std::string& filename)
	{
		std::string buffer;
		if (!neu::ReadFile(filename, buffer))
		{
			LOG("Error could not load file : %s", filename.c_str());
			return false;
		}

		// color being implemented 
		std::istringstream stream(buffer);
		stream >> m_color;

		// read number of points 
		std::string line;
		std::getline(stream, line);

		// get num of points
		size_t numPoints = std::stoi(line);

		// read points
		for (size_t i = 0; i < numPoints; i++)
		{
			Vector2 point;

			stream >> point;

			m_points.push_back(point);
		}

		//std::cout << line << std::endl;
		return true;
	}

	float Model::CalculateRadius()
	{
		float rad = 0;

		// find the largest radius
		for (auto& point : m_points)
		{
			if (point.Length() > rad) rad = point.Length();
		}

		return rad;
	}
}