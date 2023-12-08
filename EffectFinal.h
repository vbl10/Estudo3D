#pragma once
#include <ext_vec3d.h>
#include <ext_matrix.h>
#include "gsDefaults.h"
#include <ext_canvas.h>
#include <algorithm>

class EffectFinal
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
			ext::vec3d<float> pos, ppos;
			ext::vec3d<float> norm;

			Output& operator+=(const Output& rhs)
			{
				pos += rhs.pos;
				ppos += rhs.ppos;
				norm += rhs.norm;
				return *this;
			}
			Output operator+(const Output& rhs) const
			{
				return Output(*this) += rhs;
			}
			Output& operator-=(const Output& rhs)
			{
				pos -= rhs.pos;
				ppos -= rhs.ppos;
				norm -= rhs.norm;
				return *this;
			}
			Output operator-(const Output& rhs) const
			{
				return Output(*this) -= rhs;
			}
			Output& operator*=(const Output& rhs)
			{
				pos *= rhs.pos;
				ppos *= rhs.ppos;
				norm *= rhs.norm;
				return *this;
			}
			Output operator*(const Output& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const Output& rhs)
			{
				pos /= rhs.pos;
				ppos /= rhs.ppos;
				norm /= rhs.norm;
				return *this;
			}
			Output operator/(const Output& rhs) const
			{
				return Output(*this) /= rhs;
			}

			Output& operator*=(const float& rhs)
			{
				pos *= rhs;
				ppos *= rhs;
				norm *= rhs;
				return *this;
			}
			Output operator*(const float& rhs) const
			{
				return Output(*this) *= rhs;
			}
			Output& operator/=(const float& rhs)
			{
				pos /= rhs;
				ppos /= rhs;
				norm /= rhs;
				return *this;
			}
			Output operator/(const float& rhs) const
			{
				return Output(*this) /= rhs;
			}
		};
		Output operator()(const Vertex& in)
		{
			auto mat =
				ext::Mat4x4_RotateZ(rotation.z) *
				ext::Mat4x4_RotateX(rotation.x) *
				ext::Mat4x4_RotateY(rotation.y);

			Output out;

			out.norm = mat * in.norm;

			mat =
				ext::Mat4x4_Translate(translation) *
				mat;

			out.ppos = out.pos = mat * in.pos;

			return out;
		}
		ext::vec3d<float> translation, rotation;
	};
	typedef gsEmpty<VertexShader::Output> GeometryShader;
	class PixelShader
	{
	public:
		ext::Color operator()(VertexShader::Output in)
		{
			in.ppos /= in.pos.z;
			in.norm /= in.pos.z;
			in.norm = in.norm.norm();

			//light ray
			const ext::vec3d<float> l = in.ppos - light_pos;

			float ldn = l.dot(in.norm);

			//light reflection
			ext::vec3d<float> lr = (-l + in.norm * ldn * 2.0f).norm();

			//using lr.dot({0.0f,0.0f,1.0f}) = lr.z
			float spec = std::pow(std::max(0.0f, lr.z), specular);

			float i = -(l.norm()).dot(in.norm);
			float d = l.mod();
			i /= light_char.dot(ext::vec3d<float>{ d * d, d, 1.0f });
			i += spec;
			i = std::clamp(i, 0.0f, 1.0f) * (source - ambient) + ambient;

			return
			{
				(unsigned char)(i * 255.0f),
				(unsigned char)(i * 255.0f),
				(unsigned char)(i * 255.0f)
			};
		}
		ext::vec3d<float> light_pos, light_char = { 0.25f,0.25f,1.0f };
		float specular = 60.0f;
		float source = 1.0f, ambient = 0.125f;
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};