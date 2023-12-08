#pragma once
#include <vector>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <ext_matrix.h>

template <typename Vertex>
struct Mesh
{
	std::vector<Vertex> verticies;
	std::vector<int> indicies;

	static Mesh svLoadObject(const wchar_t* file_path)
	{
		std::ifstream file(file_path, std::ios_base::in);

		Mesh out;
		assert(file.is_open());
		while (file.get() != '\n');

		std::string str, sub;
		for (str.clear(); (str += file.get()).back() != '\n';);
		size_t nVerticies = std::atoi(str.substr(str.find_last_of(' ')).c_str());

		for (str.clear(); (str += file.get()).back() != '\n';);
		size_t nTriangles = std::atoi(str.substr(str.find_last_of(' ')).c_str());

		out.verticies.resize(nVerticies);
		out.indicies.resize(nTriangles * 3);

		constexpr auto npos = std::string::npos;
		size_t p = 0;
		//read verticies
		for (size_t i = 0; i < nVerticies; i++)
		{
			for (str.clear(); (str += file.get()).back() != '\n';);

			p = str.find_first_of(' ');
			assert(p != npos);
			str = str.substr(p + 1);
			p = str.find_first_of(' ');
			assert(p != npos);
			sub = str.substr(0, p);
			out.verticies[i].pos.x = (float)std::atof(sub.c_str());

			p = str.find_first_of(' ');
			assert(p != npos);
			str = str.substr(p + 1);
			p = str.find_first_of(' ');
			assert(p != npos);
			sub = str.substr(0, p);
			out.verticies[i].pos.y = (float)std::atof(sub.c_str());

			p = str.find_first_of(' ');
			assert(p != npos);
			str = str.substr(p + 1);
			out.verticies[i].pos.z = (float)std::atof(str.c_str());
		}
		//read indicies
		for (size_t i = 0; i < nTriangles; i++)
		{
			for (str.clear(); (str += file.get()).back() != '\n';);

			p = str.find_first_of(' ');
			assert(p != npos);
			str = str.substr(p + 1);
			p = str.find_first_of(' ');
			assert(p != npos);
			sub = str.substr(0, p);
			out.indicies[i * 3] = std::atoi(sub.c_str()) - 1;

			p = str.find_first_of(' ');
			assert(p != npos);
			str = str.substr(p + 1);
			p = str.find_first_of(' ');
			assert(p != npos);
			sub = str.substr(0, p);
			out.indicies[i * 3 + 2] = std::atoi(sub.c_str()) - 1;

			p = str.find_first_of(' ');
			assert(p != npos);
			str = str.substr(p + 1);
			out.indicies[i * 3 + 1] = std::atoi(str.c_str()) - 1;
		}
		file.close();
		return out;
	}
	static Mesh svSphere(float fRadius, int long_divisions, int lat_divisions)
	{
		Mesh out;
		constexpr float pi = 3.14159f;
		float stepLat = pi / (float)lat_divisions;
		float stepLong = 2.0f * pi / (float)long_divisions;
		const ext::vec3d<float> p = ext::vec3d<float>{ 0.0f,-1.0f,0.0f };
		for (int i = 1; i < lat_divisions; i++)
		{
			const ext::vec3d<float> q = ext::Mat4x4_RotateX(stepLat * (float)i) * p;
			for (int j = 0; j < long_divisions; j++)
			{
				out.verticies.emplace_back();
				out.verticies.back().pos = ext::Mat4x4_RotateY(stepLong * (float)j) * q;
			}
		}

		for (int i = 0; i < lat_divisions - 2; i++)
		{
			for (int j = 0; j < long_divisions; j++)
			{
				out.indicies.push_back(i * long_divisions + j);
				out.indicies.push_back(i * long_divisions + j + long_divisions);
				out.indicies.push_back(i * long_divisions + (j + 1) % long_divisions);

				out.indicies.push_back(i * long_divisions + (j + 1) % long_divisions);
				out.indicies.push_back(i * long_divisions + j + long_divisions);
				out.indicies.push_back(i * long_divisions + (j + 1) % long_divisions + long_divisions);
			}
		}

		//north pole
		out.verticies.emplace_back();
		out.verticies.back().pos = -p;
		for (int j = 0; j < long_divisions; j++)
		{
			out.indicies.push_back(out.verticies.size() - 1);
			out.indicies.push_back(out.verticies.size() - 1 - long_divisions + (j + 1) % long_divisions);
			out.indicies.push_back(out.verticies.size() - 1 - long_divisions + j);
		}

		//south pole
		out.verticies.emplace_back();
		out.verticies.back().pos = p;
		for (int j = 0; j < long_divisions; j++)
		{
			out.indicies.push_back(j);
			out.indicies.push_back((j + 1) % long_divisions);
			out.indicies.push_back(out.verticies.size() - 1);
		}

		return out;
	}
	static Mesh svNormSphere(float fRadius, int long_divisions, int lat_divisions)
	{
		Mesh out;
		constexpr float pi = 3.14159f;
		float stepLat = pi / (float)lat_divisions;
		float stepLong = 2.0f * pi / (float)long_divisions;
		const ext::vec3d<float> p = ext::vec3d<float>{ 0.0f,-1.0f,0.0f };
		for (int i = 1; i < lat_divisions; i++)
		{
			const ext::vec3d<float> q = ext::Mat4x4_RotateX(stepLat * (float)i) * p;
			for (int j = 0; j < long_divisions; j++)
			{
				out.verticies.emplace_back();
				out.verticies.back().norm = ext::Mat4x4_RotateY(stepLong * (float)j) * q;
				out.verticies.back().pos = out.verticies.back().norm * fRadius;
			}
		}

		for (int i = 0; i < lat_divisions - 2; i++)
		{
			for (int j = 0; j < long_divisions; j++)
			{
				out.indicies.push_back(i * long_divisions + j);
				out.indicies.push_back(i * long_divisions + j + long_divisions);
				out.indicies.push_back(i * long_divisions + (j + 1) % long_divisions);

				out.indicies.push_back(i * long_divisions + (j + 1) % long_divisions);
				out.indicies.push_back(i * long_divisions + j + long_divisions);
				out.indicies.push_back(i * long_divisions + (j + 1) % long_divisions + long_divisions);
			}
		}

		//north pole
		out.verticies.emplace_back();
		out.verticies.back().norm = -p;
		out.verticies.back().pos = -p * fRadius;
		for (int j = 0; j < long_divisions; j++)
		{
			out.indicies.push_back(out.verticies.size() - 1);
			out.indicies.push_back(out.verticies.size() - 1 - long_divisions + (j + 1) % long_divisions);
			out.indicies.push_back(out.verticies.size() - 1 - long_divisions + j);
		}

		//south pole
		out.verticies.emplace_back();
		out.verticies.back().norm = p;
		out.verticies.back().pos = p * fRadius;
		for (int j = 0; j < long_divisions; j++)
		{
			out.indicies.push_back(j);
			out.indicies.push_back((j + 1) % long_divisions);
			out.indicies.push_back(out.verticies.size() - 1);
		}

		return out;
	}
	static Mesh ivTexNormSphere(float fRadius, int long_divisions, int lat_divisions)
	{
		Mesh out;
		constexpr float pi = 3.14159f;
		float stepLat = pi / (float)lat_divisions;
		float stepLong = 2.0f * pi / (float)long_divisions;
		const ext::vec3d<float> p = ext::vec3d<float>{ 0.0f,-1.0f,0.0f };
		for (int i = 1; i < lat_divisions - 1; i++)
		{
			const ext::vec3d<float> q0 = ext::Mat4x4_RotateX(stepLat * (float)i) * p;
			const ext::vec3d<float> q1 = ext::Mat4x4_RotateX(stepLat * (float)(i + 1)) * p;
			for (int j = 0; j < long_divisions; j++)
			{
				out.verticies.emplace_back();
				out.verticies.back().norm = ext::Mat4x4_RotateY(stepLong * (float)j) * q0;
				out.verticies.back().pos = out.verticies.back().norm * fRadius;
				out.verticies.back().tx.y = out.verticies.back().norm.y * -0.5f + 0.5f;
				out.verticies.back().tx.x = 1.0f - (float)j / (float)long_divisions;

				out.verticies.emplace_back();
				out.verticies.back().norm = ext::Mat4x4_RotateY(stepLong * (float)j) * q1;
				out.verticies.back().pos = out.verticies.back().norm * fRadius;
				out.verticies.back().tx.y = out.verticies.back().norm.y * -0.5f + 0.5f;
				out.verticies.back().tx.x = 1.0f - (float)j / (float)long_divisions;

				out.verticies.emplace_back();
				out.verticies.back().norm = ext::Mat4x4_RotateY(stepLong * (float)(j + 1)) * q0;
				out.verticies.back().pos = out.verticies.back().norm * fRadius;
				out.verticies.back().tx.y = out.verticies.back().norm.y * -0.5f + 0.5f;
				out.verticies.back().tx.x = 1.0f - (float)(j + 1) / (float)long_divisions;

				out.verticies.emplace_back();
				out.verticies.back().norm = ext::Mat4x4_RotateY(stepLong * (float)(j + 1)) * q1;
				out.verticies.back().pos = out.verticies.back().norm * fRadius;
				out.verticies.back().tx.y = out.verticies.back().norm.y * -0.5f + 0.5f;
				out.verticies.back().tx.x = 1.0f - (float)(j + 1) / (float)long_divisions;

				out.indicies.push_back(out.verticies.size() - 4);
				out.indicies.push_back(out.verticies.size() - 3);
				out.indicies.push_back(out.verticies.size() - 2);

				out.indicies.push_back(out.verticies.size() - 2);
				out.indicies.push_back(out.verticies.size() - 3);
				out.indicies.push_back(out.verticies.size() - 1);
			}
		}

		////north pole
		//out.verticies.emplace_back();
		//out.verticies.back().norm = -p;
		//out.verticies.back().pos = -p * fRadius;
		//out.verticies.back().tx = { 0.5f,0.0f };
		//for (int j = 0; j < long_divisions; j++)
		//{
		//	out.indicies.push_back(out.verticies.size() - 1);
		//	out.indicies.push_back(out.verticies.size() - 1 - long_divisions + (j + 1) % long_divisions);
		//	out.indicies.push_back(out.verticies.size() - 1 - long_divisions + j);
		//}
		//
		////south pole
		//out.verticies.emplace_back();
		//out.verticies.back().norm = p;
		//out.verticies.back().pos = p * fRadius;
		//out.verticies.back().tx = { 0.5f,1.0f };
		//for (int j = 0; j < long_divisions; j++)
		//{
		//	out.indicies.push_back(j);
		//	out.indicies.push_back((j + 1) % long_divisions);
		//	out.indicies.push_back(out.verticies.size() - 1);
		//}

		return out;
	}
	static Mesh ivTexCube()
	{
		Mesh cube;

		cube.verticies =
		{
			//front
			{ {-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f} },
			{ { 0.5f,-0.5f,-0.5f}, {1.0f, 1.0f} },
			{ { 0.5f, 0.5f,-0.5f}, {1.0f, 0.0f} },
			{ {-0.5f, 0.5f,-0.5f}, {0.0f, 0.0f} },

			//right
			{ {0.5f,-0.5f,-0.5f}, {0.0f, 1.0f} },
			{ {0.5f,-0.5f, 0.5f}, {1.0f, 1.0f} },
			{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
			{ {0.5f, 0.5f,-0.5f}, {0.0f, 0.0f} },

			//back
			{ { 0.5f,-0.5f, 0.5f}, {0.0f, 1.0f} },
			{ {-0.5f,-0.5f, 0.5f}, {1.0f, 1.0f} },
			{ {-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
			{ { 0.5f, 0.5f, 0.5f}, {0.0f, 0.0f} },

			//left
			{ {-0.5f,-0.5f, 0.5f}, {0.0f, 1.0f} },
			{ {-0.5f,-0.5f,-0.5f}, {1.0f, 1.0f} },
			{ {-0.5f, 0.5f,-0.5f}, {1.0f, 0.0f} },
			{ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f} },

			//top
			{ {-0.5f, 0.5f,-0.5f}, {0.0f, 1.0f} },
			{ { 0.5f, 0.5f,-0.5f}, {1.0f, 1.0f} },
			{ { 0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
			{ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f} },

			//bottom
			{ {-0.5f,-0.5f, 0.5f}, {0.0f, 1.0f} },
			{ { 0.5f,-0.5f, 0.5f}, {1.0f, 1.0f} },
			{ { 0.5f,-0.5f,-0.5f}, {1.0f, 0.0f} },
			{ {-0.5f,-0.5f,-0.5f}, {0.0f, 0.0f} }
		};
		cube.indicies =
		{
			//front
			0, 1, 2,
			0, 2, 3,

			//left
			4, 5, 6,
			4, 6, 7,

			//back
			8, 9, 10,
			8, 10, 11,

			//right
			12, 13, 14,
			12, 14, 15,

			//top
			16, 17, 18,
			16, 18, 19,

			//bottom
			20, 21, 22,
			20, 22, 23
		};
		return cube;
	}
	static Mesh ivTexSkinnedCube()
	{
		Mesh cube;

		cube.verticies =
		{
			//front
			{ {-0.5f,-0.5f, -0.5f}, {0.00f, 0.666666f} },
			{ { 0.5f,-0.5f, -0.5f}, {0.25f, 0.666666f} },
			{ { 0.5f, 0.5f, -0.5f}, {0.25f, 0.333333f} },
			{ {-0.5f, 0.5f, -0.5f}, {0.00f, 0.333333f} },

			//right
			{ {0.5f,-0.5f,-0.5f}, {0.25f, 0.666666f} },
			{ {0.5f,-0.5f, 0.5f}, {0.50f, 0.666666f} },
			{ {0.5f, 0.5f, 0.5f}, {0.50f, 0.333333f} },
			{ {0.5f, 0.5f,-0.5f}, {0.25f, 0.333333f} },

			//back
			{ { 0.5f,-0.5f, 0.5f}, {0.50f, 0.666666f} },
			{ {-0.5f,-0.5f, 0.5f}, {0.75f, 0.666666f} },
			{ {-0.5f, 0.5f, 0.5f}, {0.75f, 0.333333f} },
			{ { 0.5f, 0.5f, 0.5f}, {0.50f, 0.333333f} },

			//left
			{ {-0.5f,-0.5f, 0.5f}, {0.75f, 0.666666f} },
			{ {-0.5f,-0.5f,-0.5f}, {1.00f, 0.666666f} },
			{ {-0.5f, 0.5f,-0.5f}, {1.00f, 0.333333f} },
			{ {-0.5f, 0.5f, 0.5f}, {0.75f, 0.333333f} },

			//top
			{ {-0.5f, 0.5f,-0.5f}, {0.00f, 0.333333f} },
			{ { 0.5f, 0.5f,-0.5f}, {0.25f, 0.333333f} },
			{ { 0.5f, 0.5f, 0.5f}, {0.25f, 0.000000f} },
			{ {-0.5f, 0.5f, 0.5f}, {0.00f, 0.000000f} },

			//bottom
			{ {-0.5f,-0.5f, 0.5f}, {0.00f, 1.000000f} },
			{ { 0.5f,-0.5f, 0.5f}, {0.25f, 1.000000f} },
			{ { 0.5f,-0.5f,-0.5f}, {0.25f, 0.666666f} },
			{ {-0.5f,-0.5f,-0.5f}, {0.00f, 0.666666f} }
		};
		cube.indicies =
		{
			//front
			0, 1, 2,
			0, 2, 3,

			//left
			4, 5, 6,
			4, 6, 7,

			//back
			8, 9, 10,
			8, 10, 11,

			//right
			12, 13, 14,
			12, 14, 15,

			//top
			16, 17, 18,
			16, 18, 19,

			//bottom
			20, 21, 22,
			20, 22, 23
		};
		return cube;
	}
	static Mesh svCube()
	{
		Mesh cube;
		cube.verticies =
		{
			{-0.5f,-0.5f,-0.5f},
			{ 0.5f,-0.5f,-0.5f},
			{ 0.5f, 0.5f,-0.5f},
			{-0.5f, 0.5f,-0.5f},
			{-0.5f,-0.5f, 0.5f},
			{ 0.5f,-0.5f, 0.5f},
			{ 0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
		};
		cube.indicies =
		{
			0, 1, 2,
			0, 2, 3,

			1, 5, 6,
			1, 6, 2,

			5, 4, 7,
			5, 7, 6,

			4, 0, 3,
			4, 3, 7,

			3, 2, 6,
			3, 6, 7,

			4, 5, 1,
			4, 1, 0
		};

		return cube;
	}
	static Mesh ivNormCube()
	{
		Mesh cube;

		cube.verticies =
		{
			//front
			{ {-0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f} },
			{ { 0.5f,-0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f} },
			{ { 0.5f, 0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f} },
			{ {-0.5f, 0.5f,-0.5f}, { 0.0f, 0.0f,-1.0f} },

			//right
			{ { 0.5f,-0.5f,-0.5f}, { 1.0f, 0.0f, 0.0f} },
			{ { 0.5f,-0.5f, 0.5f}, { 1.0f, 0.0f, 0.0f} },
			{ { 0.5f, 0.5f, 0.5f}, { 1.0f, 0.0f, 0.0f} },
			{ { 0.5f, 0.5f,-0.5f}, { 1.0f, 0.0f, 0.0f} },

			//back
			{ { 0.5f,-0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f} },
			{ {-0.5f,-0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f} },
			{ {-0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f} },
			{ { 0.5f, 0.5f, 0.5f}, { 0.0f, 0.0f, 1.0f} },

			//left
			{ {-0.5f,-0.5f, 0.5f}, { -1.0f, 0.0f, 0.0f} },
			{ {-0.5f,-0.5f,-0.5f}, { -1.0f, 0.0f, 0.0f} },
			{ {-0.5f, 0.5f,-0.5f}, { -1.0f, 0.0f, 0.0f} },
			{ {-0.5f, 0.5f, 0.5f}, { -1.0f, 0.0f, 0.0f} },

			//top
			{ {-0.5f, 0.5f,-0.5f}, { 0.0f, 1.0f, 0.0f} },
			{ { 0.5f, 0.5f,-0.5f}, { 0.0f, 1.0f, 0.0f} },
			{ { 0.5f, 0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f} },
			{ {-0.5f, 0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f} },

			//bottom
			{ {-0.5f,-0.5f, 0.5f}, { 0.0f,-1.0f, 0.0f} },
			{ { 0.5f,-0.5f, 0.5f}, { 0.0f,-1.0f, 0.0f} },
			{ { 0.5f,-0.5f,-0.5f}, { 0.0f,-1.0f, 0.0f} },
			{ {-0.5f,-0.5f,-0.5f}, { 0.0f,-1.0f, 0.0f} }
		};
		cube.indicies =
		{
			//front
			0, 1, 2,
			0, 2, 3,

			//left
			4, 5, 6,
			4, 6, 7,

			//back
			8, 9, 10,
			8, 10, 11,

			//right
			12, 13, 14,
			12, 14, 15,

			//top
			16, 17, 18,
			16, 18, 19,

			//bottom
			20, 21, 22,
			20, 22, 23
		};
		return cube;
	}

};
