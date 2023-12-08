#pragma once
#include <ext_vec3d.h>
#include "vsDefaults.h"
#include "gsDefaults.h"
#include <ext_canvas.h>
#include <algorithm>
#include <ext_matrix.h>

class EffectIVTexGouraud
{
public:
	struct Vertex
	{
		ext::vec3d<float> pos;
		ext::vec3d<float> norm;
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
	class VertexShader
	{
	public:
		struct Output
		{
			ext::vec3d<float> pos;
			float i;
			ext::vec2d<float> tx;
			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				i += rhs.i;
				tx += rhs.tx;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				i -= rhs.i;
				tx -= rhs.tx;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(const Output& rhs)
			{
				pos *= rhs.pos;
				i *= rhs.i;
				tx *= rhs.tx;
				return *this;
			}
			Output operator*(const Output& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const Output& rhs)
			{
				pos /= rhs.pos;
				i /= rhs.i;
				tx /= rhs.tx;
				return *this;
			}
			Output operator/(const Output& rhs) const
			{
				return Output(*this) /= rhs;
			}

			Output& operator*=(const float& rhs)
			{
				pos *= rhs;
				i *= rhs;
				tx *= rhs;
				return *this;
			}
			Output operator*(const float& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float& rhs)
			{
				pos /= rhs;
				i /= rhs;
				tx /= rhs;
				return *this;
			}
			Output operator/(const float& rhs) const
			{
				return Output(*this) /= rhs;
			}
		};

		Output operator()(const Vertex& in)
		{
			Output out;
			auto transform =
				ext::Mat4x4_RotateZ(rotation.z) *
				ext::Mat4x4_RotateX(rotation.x) *
				ext::Mat4x4_RotateY(rotation.y);

			out.i = std::clamp(-(transform * in.norm).dot(light_dir), 0.0f, 1.0f) * (source - ambient) + ambient;

			transform =
				ext::Mat4x4_Translate(translation) *
				ext::Mat4x4_Scale(scale) *
				transform;

			out.pos = transform * in.pos;

			out.tx = in.tx;

			return out;
		};
		ext::vec3d<float> rotation;
		ext::vec3d<float> translation;
		ext::vec3d<float> scale;
		ext::vec3d<float> light_dir = { 0.0f,0.0f,1.0f };
		float source = 1.0f;
		float ambient = 0.0f;
	};
	typedef gsEmpty<VertexShader::Output> GeometryShader;
	class PixelShader
	{
	public:
		ext::Color operator()(const VertexShader::Output& in)
		{
			auto color = tex[(tex.GetSize().to<float>() * in.tx / in.pos.z).to<int>()];
			float i = in.i / in.pos.z;
			color.r = (unsigned char)((float)color.r * i);
			color.g = (unsigned char)((float)color.g * i);
			color.b = (unsigned char)((float)color.b * i);
			return color;
		}
		ext::Surface tex;
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};