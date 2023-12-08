#pragma once
#include "EffectFinal.h"
#include "EffectGouraud.h"
#include "Scene.h"
#include "Pipeline.h"
#include "Mesh.h"
#include <ext_win32.h>
#include <ext_d2d1.h>
#include "ObjectLoader.h"

class SceneTest : public Scene
{
public:
	SceneTest(const ext::vec2d<int> size)
		:ppline(size, 1.0f), ppline_light(size, 1.0f)
	{
		auto vx = ObjLoadFloats("suzanne.obj", "v ");
		auto vn = ObjLoadFloats("suzanne.obj", "vn ");
		auto fc = ObjLoadInts("suzanne.obj", "f ");
		
		mesh.verticies.resize(vx.size() / 3);
		for (size_t i = 0, e = vx.size() / 3; i < e; i++)
		{
			mesh.verticies[i].pos.x = vx[i * 3 + 0];
			mesh.verticies[i].pos.y = vx[i * 3 + 1];
			mesh.verticies[i].pos.z = vx[i * 3 + 2];
		}
		mesh.indicies.resize(fc.size() / 2);
		for (size_t i = 0, e = fc.size() / 6; i < e; i++)
		{
			mesh.indicies[i * 3 + 0] = fc[i * 6 + 0] - 1;
			mesh.indicies[i * 3 + 2] = fc[i * 6 + 2] - 1;
			mesh.indicies[i * 3 + 1] = fc[i * 6 + 4] - 1;
		
			mesh.verticies[fc[i * 6 + 0] - 1].norm.x = vn[(fc[i * 6 + 1] - 1) * 3 + 0];
			mesh.verticies[fc[i * 6 + 0] - 1].norm.y = vn[(fc[i * 6 + 1] - 1) * 3 + 1];
			mesh.verticies[fc[i * 6 + 0] - 1].norm.z = vn[(fc[i * 6 + 1] - 1) * 3 + 2];
		
			mesh.verticies[fc[i * 6 + 2] - 1].norm.x = vn[(fc[i * 6 + 3] - 1) * 3 + 0];
			mesh.verticies[fc[i * 6 + 2] - 1].norm.y = vn[(fc[i * 6 + 3] - 1) * 3 + 1];
			mesh.verticies[fc[i * 6 + 2] - 1].norm.z = vn[(fc[i * 6 + 3] - 1) * 3 + 2];
		
			mesh.verticies[fc[i * 6 + 4] - 1].norm.x = vn[(fc[i * 6 + 5] - 1) * 3 + 0];
			mesh.verticies[fc[i * 6 + 4] - 1].norm.y = vn[(fc[i * 6 + 5] - 1) * 3 + 1];
			mesh.verticies[fc[i * 6 + 4] - 1].norm.z = vn[(fc[i * 6 + 5] - 1) * 3 + 2];
		}
		//mesh = Mesh<EffectFinal::Vertex>::ivNormCube();
		ppline.pEffect.reset(new EffectFinal);

		mesh_light = Mesh<EffectGouraud::Vertex>::svNormSphere(0.125f, 20, 20);
		ppline_light.pEffect.reset(new EffectGouraud);

		Reset();
	}
	virtual void Reset()
	{
		ppline.pEffect->vs.rotation = { 0.0f,0.0f,0.0f };
		ppline.pEffect->vs.translation = { 0.0f,0.0f,4.0f };
		ppline.pEffect->ps.light_pos = { 0.0f,0.0f, 0.0f };

		ppline_light.pEffect->vs.translation = { 0.0f,1.0f,4.0f };
		ppline_light.pEffect->vs.rotation = { 0.0f,0.0f,0.0f };
	}
	virtual void Drag(const ext::vec2d<float>& d)
	{
		ppline.pEffect->vs.rotation.y -= d.x;
		ppline.pEffect->vs.rotation.x -= d.y;
	}
	virtual void Update(float fElapsedTime)
	{
		for (int i = 0; i < 5; i++)
		{
			if (GetAsyncKeyState("WASD "[i]))
			{
				switch (i)
				{
				case 0:
					ppline_light.pEffect->vs.translation.z += fElapsedTime;
					break;
				case 1:
					ppline_light.pEffect->vs.translation.x -= fElapsedTime;
					break;
				case 2:
					ppline_light.pEffect->vs.translation.z -= fElapsedTime;
					break;
				case 3:
					ppline_light.pEffect->vs.translation.x += fElapsedTime;
					break;
				case 4:
					ppline_light.pEffect->vs.translation.y += fElapsedTime * (GetAsyncKeyState(VK_SHIFT) ? -1.0f : 1.0f);
					break;
				}
			}
		}
		ppline.pEffect->ps.light_pos = ppline_light.pEffect->vs.translation;
	}
	virtual void Zoom(bool bIn) {}
	virtual void Draw() override
	{
		ppline.Clear();
		ppline.Draw(mesh);
		ppline_light.Draw(mesh_light);
	}
	virtual std::wstring GetInstructions()
	{
		return
			L"Scene Test\n";
	}

private:
	Pipeline<EffectFinal> ppline;
	Mesh<EffectFinal::Vertex> mesh;

	Pipeline<EffectGouraud> ppline_light;
	Mesh<EffectGouraud::Vertex> mesh_light;
};