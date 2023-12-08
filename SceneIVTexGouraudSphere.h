#pragma once
#include "EffectIVTexGouraud.h"
#include "Scene.h"
#include "Pipeline.h"
#define NOMINMAX
#include <Windows.h>

class SceneIVTexGouraudSphere : public Scene
{
public:
	SceneIVTexGouraudSphere(const ext::vec2d<int>& size)
		:ppline(size, 1.0f)
	{
		ppline.pEffect.reset(new EffectIVTexGouraud);
		mesh_sphere = Mesh<EffectIVTexGouraud::Vertex>::ivTexNormSphere(1.0f, 48, 40);
		ppline.pEffect->ps.tex.Shares(ext::Surface(L"map.jpg"));
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
			L"Texture + Gourad (independent verticies)\n"
			L"\'A\' e \'D\' para mudar a direção da luz\n";
	}

private:
	Pipeline<EffectIVTexGouraud> ppline;
	Mesh<EffectIVTexGouraud::Vertex> mesh_sphere;
};