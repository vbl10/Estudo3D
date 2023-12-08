#pragma once
#include "VertexDefault.h"
#include "vsDefaults.h"
#include "Triangle.h"
#include <ext_canvas.h>
#include <algorithm>

class EffectFlatShading
{
public:
	typedef VertexDefault Vertex;
	typedef vsTransform<Vertex> VertexShader;
	class GeometryShader
	{
	public:
		struct Output
		{
			ext::vec3d<float> pos;
			float d;

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
			ext::vec3d<float> n = (in.b.pos - in.a.pos).cross(in.c.pos - in.a.pos);
			n /= n.mod();
			out.a.d = out.b.d = out.c.d = std::clamp(n.dot(light_dir), 0.0f, 1.0f) * (source - ambient) + ambient;

			return out;
		}
		ext::vec3d<float> light_dir = { 0.0f,0.0f,1.0f };
		float source = 1.0f;
		float ambient = 0.0f;
	};
	class PixelShader
	{
	public:
		ext::Color operator()(const GeometryShader::Output& in)
		{
			ext::vec3d<float> vec = { 255.0f,128.0f,0.0f };
			vec *= in.d;
			return
			{
				(unsigned char)vec.x,
				(unsigned char)vec.y,
				(unsigned char)vec.z
			};
		}
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};