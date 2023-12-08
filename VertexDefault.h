#pragma once
#include <ext_vec3d.h>

struct VertexDefault
{
	ext::vec3d<float> pos;

	VertexDefault& operator+=(const VertexDefault& rhs)
	{
		pos += rhs.pos;
		return *this;
	}
	VertexDefault operator+(const VertexDefault& rhs) const
	{
		return VertexDefault(*this) += rhs;
	}
	VertexDefault& operator-=(const VertexDefault& rhs)
	{
		pos -= rhs.pos;
		return *this;
	}
	VertexDefault operator-(const VertexDefault& rhs) const
	{
		return VertexDefault(*this) -= rhs;
	}
	VertexDefault& operator*=(const VertexDefault& rhs)
	{
		pos *= rhs.pos;
		return *this;
	}
	VertexDefault operator*(const VertexDefault& rhs) const
	{
		return VertexDefault(*this) *= rhs;
	}
	VertexDefault& operator/=(const VertexDefault& rhs)
	{
		pos /= rhs.pos;
		return *this;
	}
	VertexDefault operator/(const VertexDefault& rhs) const
	{
		return VertexDefault(*this) /= rhs;
	}

	VertexDefault& operator*=(const float& rhs)
	{
		pos *= rhs;
		return *this;
	}
	VertexDefault operator*(const float& rhs) const
	{
		return VertexDefault(*this) *= rhs;
	}
	VertexDefault& operator/=(const float& rhs)
	{
		pos /= rhs;
		return *this;
	}
	VertexDefault operator/(const float& rhs) const
	{
		return VertexDefault(*this) /= rhs;
	}
};
