#pragma once
#include <ext_canvas.h>
#include <ext_vec3d.h>
#include <ext_matrix.h>
#include <vector>
#include "Mesh.h"
#include "Triangle.h"

/*

class Effect
{
public:
	struct Vertex
	{
		ext::vec3d<float> pos;

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
		typedef Vertex Output;
		template <typename InputVertex>
		Output operator()(InputVertex& in)
		{
			Output output;
			output.pos = in.pos;
			output.tx = in.tx;
			return output;
		}
	};
	class GeometryShader
	{
	public:
		typedef Vertex Output;
		template <typename InputVertex>
		Triangle<Output> operator()(Triangle<InputVertex> in, size_t id);
	};
	class PixelShader
	{
	public:
		template <typename InputVertex>
		Color operator()(const InputVertex& v);
	};

	VertexShader vs;
	GeometryShader gs;
	PixelShader ps;
};

*/

extern std::shared_ptr<float[]> depth_buffer;
extern ext::Surface surface;

template <typename Effect>
class Pipeline : public ext::Canvas
{
public:
	//rect of 1x1 will fill the canvas at z = unit_depth
	Pipeline(ext::vec2d<int> size, float unit_depth = 10.0f)
		:
		Canvas(size), 
		scale((float)std::min(size.x,size.y) * unit_depth)
	{
		if (!surface.GetBuffer())
		{
			surface.Shares(*this);
		}
		else
		{
			Shares(surface);
		}
		if (!depth_buffer)
		{
			depth_buffer.reset(new float[size.x * size.y]);
		}
	}

	//Vertex must implement basic math operations,
	//declare 'ext::vec3d<float> pos' as a public member variable
	//and be copyable
	typedef Effect::Vertex Vertex;
	typedef Effect::VertexShader::Output VSOut;
	typedef Effect::GeometryShader::Output GSOut;

	void Clear(ext::Color = { 0,0,0 }) override
	{
		Canvas::Clear();
		memset(depth_buffer.get(), 0, sizeof(float) * size.x * size.y);
	}
	void Draw(const Mesh<Vertex>& mesh)
	{
		ProcessVerticies(mesh.verticies, mesh.indicies);
	}

	std::shared_ptr<Effect> pEffect;
	float scale;
private:
	//apply vertex shader to all verticies
	void ProcessVerticies(const std::vector<Vertex>& verticies, const std::vector<int>& indicies)
	{
		std::vector<VSOut> output(verticies.size());
		for (size_t i = 0, e = verticies.size(); i < e; i++)
			output[i] = pEffect->vs(verticies[i]);
		AssembleTriangles(output, indicies);
	}
	//back-face culling
	void AssembleTriangles(std::vector<VSOut>& verticies, const std::vector<int>& indicies)
	{
		for (size_t i = 0, end = indicies.size() / 3; i < end; i++)
		{
			VSOut& a = verticies[indicies[i * 3 + 0]];
			VSOut& b = verticies[indicies[i * 3 + 1]];
			VSOut& c = verticies[indicies[i * 3 + 2]];
			//test for greater than 0.0f because the y will be negated
			if ((b.pos - a.pos).cross(c.pos - a.pos).dot(a.pos) > 0.0f)
			{
				ProcessTriangle({ a, b, c }, i);
			}
		}
	}
	//geometry shader
	void ProcessTriangle(Triangle<VSOut> input, size_t id)
	{
		//apply geometry shader
		Triangle<GSOut> output = pEffect->gs(input, id);
		PostProcessTriangle(output.a, output.b, output.c);
	}
	//apply perspective projection to the three verticies
	void PostProcessTriangle(GSOut& a, GSOut& b, GSOut& c)
	{
		float z = a.pos.z;
		a /= z;
		a.pos.z = 1.0f / z;
		a.pos.y *= -1.0f;
		*(ext::vec2d<float>*)& a.pos *= scale;
		*(ext::vec2d<float>*)& a.pos += 0.5f * size;

		z = b.pos.z;
		b /= z;
		b.pos.z = 1.0f / z;
		b.pos.y *= -1.0f;
		*(ext::vec2d<float>*)& b.pos *= scale;
		*(ext::vec2d<float>*)& b.pos += 0.5f * size;

		z = c.pos.z;
		c /= z;
		c.pos.z = 1.0f / z;
		c.pos.y *= -1.0f;
		*(ext::vec2d<float>*)& c.pos *= scale;
		*(ext::vec2d<float>*)& c.pos += 0.5f * size;

		DrawTriangle(a, b, c);
	}
	//draw triangle
	void DrawTriangle(GSOut& a, GSOut& b, GSOut& c)
	{
		GSOut d;
		//make intemediate vertex ('d') to split the triangle
		{
			//sort verticies based on height
			if (a.pos.y > b.pos.y)
				std::swap(a, b);
			if (a.pos.y > c.pos.y)
				std::swap(a, c);
			if (b.pos.y > c.pos.y)
				std::swap(c, b);

			//'d' is in between 'a' and 'c' at the height of 'b'
			float i = (b.pos.y - a.pos.y) / (c.pos.y - a.pos.y);
			//linear interpolation
			d = (c - a) * i + a;
		}

		if (b.pos.x > d.pos.x)
			std::swap(b, d);

		//draw flat bottom triangle (abd)
		{
			int y = (int)std::round(a.pos.y);

			const float pre_step_y = (float)y + 0.5f - a.pos.y;

			//deltas for each step in y
			GSOut dy0 = (b - a) / (b.pos.y - a.pos.y);
			GSOut dy1 = (d - a) / (b.pos.y - a.pos.y);

			//start and end for each scan line
			GSOut x0 = a + dy0 * pre_step_y;
			GSOut x1 = a + dy1 * pre_step_y;

			for (int end_y = (int)std::round(b.pos.y); y < end_y; y++)
			{
				//iterators
				int x = (int)std::round(x0.pos.x), end_x = (int)std::round(x1.pos.x);

				const float pre_step_x = (float)x + 0.5f - x0.pos.x;

				//delta for each step in x
				GSOut dx = (x1 - x0) / (x1.pos.x - x0.pos.x);

				GSOut v = x0 + dx * pre_step_x;

				for (; x < end_x; x++)
				{
					//v.pos.z is actually 1/z after perspective projection
					if (x >= 0 && x < size.x && y >= 0 && y < size.y && v.pos.z > depth_buffer[x + y * size.x])
					{
						depth_buffer[x + y * size.x] = v.pos.z;
						PutPixel({ x, y }, pEffect->ps(v));
					}
					v += dx;
				}
				x0 += dy0;
				x1 += dy1;
			}
		}

		//draw flat top triangle (bdc)
		{
			int y = (int)std::round(b.pos.y);

			const float pre_step_y = (float)y + 0.5f - b.pos.y;

			//deltas for each step in y
			GSOut dy0 = (c - b) / (c.pos.y - b.pos.y);
			GSOut dy1 = (c - d) / (c.pos.y - b.pos.y);

			//start and end for each scan line
			GSOut x0 = b + dy0 * pre_step_y;
			GSOut x1 = d + dy1 * pre_step_y;

			for (int end_y = (int)std::round(c.pos.y); y < end_y; y++)
			{
				//iterators
				int x = (int)std::round(x0.pos.x), end_x = (int)std::round(x1.pos.x);

				const float pre_step_x = (float)x + 0.5f - x0.pos.x;

				//delta for each step in x
				GSOut dx = (x1 - x0) / (x1.pos.x - x0.pos.x);

				GSOut v = x0 + dx * pre_step_x;

				for (; x < end_x; x++)
				{
					//v.pos.z is actually 1/z after perspective projection
					if (x >= 0 && x < size.x && y >= 0 && y < size.y && v.pos.z > depth_buffer[x + y * size.x])
					{
						depth_buffer[x + y * size.x] = v.pos.z;
						PutPixel({ x, y }, pEffect->ps(v));
					}
					v += dx;
				}
				x0 += dy0;
				x1 += dy1;
			}
		}
	}
};