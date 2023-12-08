#pragma once
#include <ext_vec3d.h>
#include "vsDefaults.h"
#include "gsDefaults.h"
#include <ext_canvas.h>
#include <algorithm>
#include <ext_matrix.h>

class EffectGouraud
{
public:
	struct Vertex
	{
		ext::vec3d<float> pos;
		ext::vec3d<float> norm;

		Vertex& operator+=(const Vertex& rhs)
		{
			pos += rhs.pos;
			return *this;
		}
		Vertex operator+(const Vertex& rhs) const
		{
			return Vertex(*this) += rhs;
		}
		Vertex& operator-=(const Vertex& rhs)
		{
			pos -= rhs.pos;
			return *this;
		}
		Vertex operator-(const Vertex& rhs) const
		{
			return Vertex(*this) -= rhs;
		}
		Vertex& operator*=(const Vertex& rhs)
		{
			pos *= rhs.pos;
			return *this;
		}
		Vertex operator*(const Vertex& rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const Vertex& rhs)
		{
			pos /= rhs.pos;
			return *this;
		}
		Vertex operator/(const Vertex& rhs) const
		{
			return Vertex(*this) /= rhs;
		}

		Vertex& operator*=(const float& rhs)
		{
			pos *= rhs;
			return *this;
		}
		Vertex operator*(const float& rhs) const
		{
			return Vertex(*this) *= rhs;
		}
		Vertex& operator/=(const float& rhs)
		{
			pos /= rhs;
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
			ext::vec3d<float> pos, color;
			
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
		
		Output operator()(const Vertex& in)
		{
			Output out;
			auto transform =
				ext::Mat4x4_RotateZ(rotation.z) *
				ext::Mat4x4_RotateX(rotation.x) *
				ext::Mat4x4_RotateY(rotation.y);

			float i = std::clamp(-(transform * in.norm).norm().dot(light_dir), 0.0f, 1.0f) * (source - ambient) + ambient;
			out.color = ext::vec3d<float>{ 255.0f,255.0f,255.0f } *i;

			transform =
				ext::Mat4x4_Translate(translation) *
				ext::Mat4x4_Scale(scale) *
				transform;

			out.pos = transform * in.pos;
			return out;
		};
		ext::vec3d<float> rotation;
		ext::vec3d<float> translation;
		ext::vec3d<float> scale = { 1.0f,1.0f,1.0f };
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
			return 
			{
				(unsigned char)(in.color.x / in.pos.z),
				(unsigned char)(in.color.y / in.pos.z),
				(unsigned char)(in.color.z / in.pos.z)
			};
		}
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};