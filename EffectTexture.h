#pragma once
#include "Mesh.h"
#include "Triangle.h"
#include "vsDefaults.h"
#include "gsDefaults.h"
#include <ext_vec2d.h>
#include <ext_vec3d.h>
#include <ext_canvas.h>
#include <ext_matrix.h>

class EffectTexture
{
public:
	struct Vertex
	{
		ext::vec3d<float> pos;
		ext::vec2d<float> tx;
		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			tx += rhs.tx;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			tx -= rhs.tx;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(const Vertex& rhs)
		{
			pos *= rhs.pos;
			tx *= rhs.tx;
			return *this;
		}
		Vertex operator*(const Vertex& rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const Vertex& rhs)
		{
			pos /= rhs.pos;
			tx /= rhs.tx;
			return *this;
		}
		Vertex operator/(const Vertex& rhs) const
		{
			return Vertex(*this) /= rhs;
		}

		Vertex& operator*=(const float& rhs)
		{
			pos *= rhs;
			tx *= rhs;
			return *this;
		}
		Vertex operator*(const float& rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const float& rhs)
		{
			pos /= rhs;
			tx /= rhs;
			return *this;
		}
		Vertex operator/(const float& rhs) const
		{
			return Vertex(*this) /= rhs;
		}
	};
	typedef vsTransform<Vertex> VertexShader;
	typedef gsEmpty<Vertex> GeometryShader;
	class PixelShader
	{
	public:
		ext::Color operator()(const Vertex& v)
		{
			return  tex[(tex.GetSize().to<float>() * v.tx / v.pos.z).to<int>()];
		}
		ext::Surface tex;
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};