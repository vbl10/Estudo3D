#pragma once
#include <ext_vec3d.h>
#include <ext_canvas.h>
#include <string>
#include <ext_d2d1.h>

class Scene
{
public:
	virtual void Reset() {}
	virtual void Drag(const ext::vec2d<float>& d) {}
	virtual void Update(float fElapsedTime) {}
	virtual void Zoom(bool bIn) {}
	virtual void Draw() = 0;
	virtual std::wstring GetInstructions() = 0;
};