#pragma once
#include "VertexDefault.h"
#include "VS_default.h"
#include "Triangle.h"
#include <ext_canvas.h>
#include <vector>

class EffectColorGradient
{
public:
	typedef VertexDefault Vertex;
	typedef VS_default<Vertex> VertexShader;
	class GeometryShader
	{
	public:
		struct Output
		{
			ext::vec3d<float> pos;
			ext::vec3d<float> color;

			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				color += rhs.color;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				color -= rhs.color;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(const Output& rhs)
			{
				pos *= rhs.pos;
				color *= rhs.color;
				return *this;
			}
			Output operator*(const Output& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const Output& rhs)
			{
				pos /= rhs.pos;
				color /= rhs.color;
				return *this;
			}
			Output operator/(const Output& rhs) const
			{
				return Output(*this) /= rhs;
			}

			Output& operator*=(const float& rhs)
			{
				pos *= rhs;
				color *= rhs;
				return *this;
			}
			Output operator*(const float& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float& rhs)
			{
				pos /= rhs;
				color /= rhs;
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
			if (id % 2)
			{
				out.a.color = ext::vec3d<float>{(float)colors[0].b,(float)colors[0].g,(float)colors[0].r};
				out.b.color = ext::vec3d<float>{(float)colors[2].b,(float)colors[2].g,(float)colors[2].r};
				out.c.color = ext::vec3d<float>{(float)colors[3].b,(float)colors[3].g,(float)colors[3].r };
			}
			else
			{
				out.a.color = ext::vec3d<float>{ (float)colors[0].b,(float)colors[0].g,(float)colors[0].r };
				out.b.color = ext::vec3d<float>{ (float)colors[1].b,(float)colors[1].g,(float)colors[1].r };
				out.c.color = ext::vec3d<float>{ (float)colors[2].b,(float)colors[2].g,(float)colors[2].r };
			}
			return out;
		}
		std::vector<ext::Color> colors;
	};
	class PixelShader
	{
	public:
		ext::Color operator()(const GeometryShader::Output& in)
		{
			auto color = in.color;
			color /= in.pos.z;
			return ext::Color{ (unsigned char)color.x,(unsigned char)color.y, (unsigned char)color.z };
		}
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};