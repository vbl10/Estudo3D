#include <ext_win32.h>
#include <ext_d2d1.h>
#include <chrono>
#include <iostream>
#include <memory>
#include "SceneGouraudSphere.h"
#include "SceneIVTexGouraudSphere.h"
#include "SceneTest.h"

using namespace ext;

std::vector<std::shared_ptr<Scene>> scenes;
int nCurScene = 0;

class MainWindow : public Window, public D2DGraphics
{
public:
	MainWindow()
		:Window(L"Estudo3D", {720,570}), D2DGraphics(hWnd)
	{
		ShowWindow(hWnd, SW_SHOW);
	}
private:
	LRESULT AppProc(HWND, UINT msg, WPARAM wParam, LPARAM lParam) override
	{
		switch (msg)
		{
		case WM_KEYUP:
			if (wParam == 'R')
			{
				scenes[nCurScene]->Reset();
			}
			else if (wParam == VK_TAB)
			{
				nCurScene = (nCurScene + GetKeyState(VK_SHIFT) * 2 - 1 + scenes.size()) % scenes.size();
			}
			break;
		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON)
			{
				auto d = (mpos - cdim / 2).to<float>();
				d *= 0.01f;
				scenes[nCurScene]->Drag(d);
				POINT pt = { cdim.x / 2,cdim.y / 2 };
				ClientToScreen(hWnd, &pt);
				SetCursorPos(pt.x, pt.y);
			}
			break;
		case WM_LBUTTONDOWN:
		{
			POINT pt = { cdim.x / 2,cdim.y / 2 };
			ClientToScreen(hWnd, &pt);
			SetCursorPos(pt.x, pt.y);
			ShowCursorX(false);
		}
			break;
		case WM_LBUTTONUP:
			ShowCursorX(true);
			break;
		case WM_MOUSELEAVE:
			ShowCursorX(true);
			break;
		case WM_MOUSEWHEEL:
			scenes[nCurScene]->Zoom(GET_WHEEL_DELTA_WPARAM(wParam) > 0);
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
		}
		
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
};

//int main()
int CALLBACK wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	MainWindow wnd;
	TextFormat consolas(L"consolas", 16.0f);

	scenes =
	{
		std::make_shared<SceneIVTexGouraudSphere>(wnd.cdim),
		std::make_shared<SceneGouraudSphere>(wnd.cdim),
		std::make_shared<SceneTest>(wnd.cdim)
	};

	auto tp1 = std::chrono::steady_clock::now(), tp2 = tp1;
	float fElapsedTime = 0.0f;
	MSG msg;
	while (1)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}

		scenes[nCurScene]->Update(fElapsedTime);

		wnd.pRenderTarget->BeginDraw();
		wnd.pRenderTarget->Clear();
		scenes[nCurScene]->Draw();
		wnd.pRenderTarget->DrawBitmap(wnd.CreateBitmap(surface));

		wnd.pSolidBrush->SetColor(D2D1::ColorF(0xffffff));
		wnd.pRenderTarget->DrawTextLayout(
			D2D1::Point2F(),
			consolas(
				scenes[nCurScene]->GetInstructions() +
				L"Roda do mouse para dar zoom\n"
				L"\'R\' para recomeçar\n"
				L"TAB para mudar de cena\n"
			),
			wnd.pSolidBrush);

		wnd.pRenderTarget->EndDraw();

		tp2 = std::chrono::steady_clock::now();
		fElapsedTime = std::chrono::duration<float>(tp2 - tp1).count();
		tp1 = std::chrono::steady_clock::now();
	}

	return 0;
}

///Flat shading
//#include <ext_win32.h>
//#include <ext_d2d1.h>
//#include "Pipeline.h"
//#include "EffectFlatShading.h"
//#include "Mesh.h"
//#include <chrono>
//#include <iostream>
//
//using namespace ext;
//
//vec3d<float> mesh_angle = { 0.0f,0.0f,0.0f };
//float mesh_scale = 1.0f;
//class MainWindow : public Window, public D2DGraphics
//{
//public:
//	MainWindow()
//		:Window(L"Estudo3D", { 720,570 }), D2DGraphics(hWnd)
//	{
//		ShowWindow(hWnd, SW_SHOW);
//	}
//private:
//	LRESULT AppProc(HWND, UINT msg, WPARAM wParam, LPARAM lParam) override
//	{
//		switch (msg)
//		{
//		case WM_KEYUP:
//			if (wParam == 'R')
//			{
//				mesh_angle = { 0.0f,0.0f,0.0f };
//				mesh_scale = 1.0f;
//			}
//			break;
//		case WM_MOUSEMOVE:
//			if (wParam & MK_LBUTTON)
//			{
//				auto d = (mpos - cdim / 2).to<float>();
//				d *= 0.01f;
//				mesh_angle.y -= d.x;
//				mesh_angle.x -= d.y;
//				POINT pt = { cdim.x / 2,cdim.y / 2 };
//				ClientToScreen(hWnd, &pt);
//				SetCursorPos(pt.x, pt.y);
//			}
//			break;
//		case WM_LBUTTONDOWN:
//		{
//			POINT pt = { cdim.x / 2,cdim.y / 2 };
//			ClientToScreen(hWnd, &pt);
//			SetCursorPos(pt.x, pt.y);
//			ShowCursorX(false);
//		}
//		break;
//		case WM_LBUTTONUP:
//			ShowCursorX(true);
//			break;
//		case WM_MOUSELEAVE:
//			ShowCursorX(true);
//			break;
//		case WM_MOUSEWHEEL:
//			mesh_scale +=
//				GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? 1.0f : -1.0f;
//			break;
//		case WM_CLOSE:
//			PostQuitMessage(0);
//			return 0;
//			break;
//		}
//
//		return DefWindowProcW(hWnd, msg, wParam, lParam);
//	}
//};
//
////int main()
//int CALLBACK wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
//{
//	MainWindow wnd;
//
//	TextFormat consolas(L"consolas", 16.0f);
//
//	Pipeline<EffectFlatShading> canvas(wnd.cdim, 1.0f);
//	canvas.pEffect = std::make_shared<EffectFlatShading>();
//
//	auto mesh = Mesh<EffectFlatShading::Vertex>::LoadObject(L"bunny.obj");
//	for (auto& v : mesh.verticies)
//		v.pos.y -= 0.1f;
//
//	auto tp1 = std::chrono::steady_clock::now(), tp2 = tp1;
//	float fElapsedTime = 0.0f;
//	MSG msg;
//	while (1)
//	{
//		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessageW(&msg);
//			if (msg.message == WM_QUIT)
//			{
//				break;
//			}
//		}
//
//		canvas.Clear();
//
//		canvas.pEffect->vs.transform =
//			ext::Mat4x4_Translate({ 0.0f,0.0f,4.0f }) *
//			ext::Mat4x4_Scale({ mesh_scale,mesh_scale,mesh_scale }) *
//			ext::Mat4x4_RotateZ(mesh_angle.z) *
//			ext::Mat4x4_RotateX(mesh_angle.x) *
//			ext::Mat4x4_RotateY(mesh_angle.y);
//
//		if (GetAsyncKeyState('D'))
//		{
//			canvas.pEffect->gs.light_dir = 
//				ext::Mat4x4_RotateY(fElapsedTime) * canvas.pEffect->gs.light_dir;
//		}
//		else if (GetAsyncKeyState('A'))
//		{
//			canvas.pEffect->gs.light_dir =
//				ext::Mat4x4_RotateY(-fElapsedTime) * canvas.pEffect->gs.light_dir;
//		}
//		if (GetAsyncKeyState('R'))
//		{
//			canvas.pEffect->gs.light_dir = { 0.0f,0.0f,1.0f };
//		}
//
//		canvas.Draw(mesh);
//
//		wnd->BeginDraw();
//		wnd->Clear();
//		wnd->DrawBitmap(wnd.CreateBitmap(canvas));
//		
//		wnd.pSolidBrush->SetColor(D2D1::ColorF(0xffffff));
//		wnd->DrawTextLayout(
//			D2D1::Point2F(),
//			consolas(
//				L"Sombreamento plano\n"
//				L"Botão esquerdo para girar o objeto\n"
//				L"\'A\' e \'D\' para mudar a direção da luz\n"
//				L"Roda do mouse para dar zoom"),
//			wnd);
//		
//		wnd->EndDraw();
//
//		tp2 = std::chrono::steady_clock::now();
//		fElapsedTime = std::chrono::duration<float>(tp2 - tp1).count();
//		tp1 = std::chrono::steady_clock::now();
//	}
//
//	return 0;
//}