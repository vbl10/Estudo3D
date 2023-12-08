#pragma once
#include "Scene.h"
#include "Pipeline.h"
#include "EffectGouraud.h"
#define NOMINMAX
#include <Windows.h>

class SceneGouraudSphere : public Scene
{
public:
	SceneGouraudSphere(const ext::vec2d<int>& size)
		:ppline(size, 1.0f)
	{
		ppline.pEffect.reset(new EffectGouraud);
		mesh_sphere = Mesh<EffectGouraud::Vertex>::svNormSphere(0.5f, 24, 20);
		Reset();
	}
	virtual void Reset() override
	{
		ppline.pEffect->vs.translation = { 0.0f,0.0f,4.0f };
		ppline.pEffect->vs.rotation = { 0.0f,0.0f,0.0f };
		ppline.pEffect->vs.scale = 1.0f;
		ppline.pEffect->vs.light_dir = { 0.0f,0.0f,1.0f };
	}
	virtual void Drag(const ext::vec2d<float>& d) override
	{
		ppline.pEffect->vs.rotation.y -= d.x;
		ppline.pEffect->vs.rotation.x -= d.y;

	}
	virtual void Update(float fElapsedTime) override
	{
		if (GetAsyncKeyState('D'))
		{
			ppline.pEffect->vs.light_dir =
				ext::Mat4x4_RotateY(fElapsedTime) * ppline.pEffect->vs.light_dir;
		}
		else if (GetAsyncKeyState('A'))
		{
			ppline.pEffect->vs.light_dir =
				ext::Mat4x4_RotateY(-fElapsedTime) * ppline.pEffect->vs.light_dir;
		}
	}
	virtual void Zoom(bool bIn) override
	{
		ppline.pEffect->vs.scale += bIn ? 1.0f : -1.0f;
	}
	virtual void Draw() override
	{
		ppline.Clear();
		ppline.Draw(mesh_sphere);
	}
	virtual std::wstring GetInstructions() override
	{
		return
			L"Efeito Gouraud\n"
			L"\'A\' e \'D\' para mudar a direção da luz\n";
	}

private:
	Pipeline<EffectGouraud> ppline;
	Mesh<EffectGouraud::Vertex> mesh_sphere;
};