#pragma once
#include "VertexDefault.h"
#include "vsDefaults.h"
#include "Triangle.h"
#include <ext_canvas.h>
#include <vector>

class EffectSolidColor
{
public:
	typedef VertexDefault Vertex;
	typedef vsEmpty<Vertex> VertexShader;
	class GeometryShader
	{
	public:
		struct Output
		{
			ext::vec3d<float> pos;
			ext::Color color;

			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(const Output& rhs)
			{
				pos *= rhs.pos;
				return *this;
			}
			Output operator*(const Output& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const Output& rhs)
			{
				pos /= rhs.pos;
				return *this;
			}
			Output operator/(const Output& rhs) const
			{
				return Output(*this) /= rhs;
			}

			Output& operator*=(const float& rhs)
			{
				pos *= rhs;
				return *this;
			}
			Output operator*(const float& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float& rhs)
			{
				pos /= rhs;
				return *this;
			}
			Output operator/(const float& rhs) const
			{
				return Output(*this) /= rhs;
			}
		};
		Triangle<Output> operator()(const Triangle<Vertex>& in, size_t id)
		{
			Triangle<Output> out;
			out.a.pos = in.a.pos;
			out.b.pos = in.b.pos;
			out.c.pos = in.c.pos;
			out.a.color = colors[id / 2];
			out.b.color = colors[id / 2];
			out.c.color = colors[id / 2];

			return out;
		}
		std::vector<ext::Color> colors;
	};
	class PixelShader
	{
	public:
		ext::Color operator()(const GeometryShader::Output& in)
		{
			return in.color;
		}
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};