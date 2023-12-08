#pragma once
#include <ext_vec3d.h>
#include <ext_matrix.h>

template <typename InputVertex>
class vsTransform
{
public:
	typedef InputVertex Output;
	Output operator()(const InputVertex& in)
	{
		Output output = in;
		output.pos = transform * in.pos;
		return output;
	}
	ext::Matrix<4, 4> transform;
};

template <typename InputVertex>
class vsEmpty
{
public:
	typedef InputVertex Output;
	Output operator()(const InputVertex& in)
	{
		return in;
	}
};