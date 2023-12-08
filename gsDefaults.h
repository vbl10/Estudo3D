#pragma once
#include "Triangle.h"
#include <ext_vec3d.h>

template <typename InputVertex>
class gsEmpty
{
public:
	typedef InputVertex Output;
	Triangle<Output> operator()(Triangle<InputVertex> in, size_t id)
	{
		return in;
	}
};

template <typename InputVertex>
class gsFlatShading
{
public:
	struct Output
	{
		InputVertex v;
		ext::vec3d<float>& pos = v.pos;
		float d;
		Output& operator+=(const Output& rhs)
		{
			v += rhs.v;
			return *this;
		}
		Output operator+(const Output& rhs) const
		{
			return Output(*this) += rhs;
		}
		Output& operator-=(const Output& rhs)
		{
			v -= rhs.v;
			return *this;
		}
		Output operator-(const Output& rhs) const
		{
			return Output(*this) -= rhs;
		}
		Output& operator*=(const Output& rhs)
		{
			v *= rhs.v;
			return *this;
		}
		Output operator*(const Output& rhs) const
		{
			return Output(*this) *= rhs;
		}
		Output& operator/=(const Output& rhs)
		{
			v /= rhs.v;
			return *this;
		}
		Output operator/(const Output& rhs) const
		{
			return Output(*this) /= rhs;
		}

		Output& operator*=(const float& rhs)
		{
			v *= rhs;
			return *this;
		}
		Output operator*(const float& rhs) const
		{
			return Output(*this) *= rhs;
		}
		Output& operator/=(const float& rhs)
		{
			v /= rhs;
			return *this;
		}
		Output operator/(const float& rhs) const
		{
			return Output(*this) /= rhs;
		}
	};
	Triangle<Output> operator()(Triangle<InputVertex> in, size_t id)
	{
		Triangle<Output> out;
		out.a.v = in.a;
		out.b.v = in.b;
		out.c.v = in.c;
		ext::vec3d<float> n = (in.b.pos - in.a.pos).cross(in.c.pos - in.a.pos);
		n /= n.mod();
		out.a.d = out.b.d = out.c.d = n.dot(light_dir) * (source - ambient) + ambient;

		return out;
	}
	ext::vec3d<float> light_dir = { 0.0f,0.0f,1.0f };
	float ambient = 0.1f;
	float source = 1.0f;
};