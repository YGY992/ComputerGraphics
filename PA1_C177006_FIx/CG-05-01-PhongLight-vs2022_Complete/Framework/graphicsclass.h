////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include <vector>


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

	void SetAmbient(bool val);
	void SetDiffuse(bool);
	void SetSpecular(bool val);
	void IncreaseIntensity();
	void DecreaseIntensity();

	LightShaderClass* GetLightShdader() { return m_LightShader; }

private:
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
//	ModelClass* m_Model;
	std::vector<ModelClass*> m_Models;

	LightShaderClass* m_LightShader;
	//LightClass* m_Light;
	LightClass* m_Light1, * m_Light2, * m_Light3, * m_Light4;

	float m_rotationX = 0.0f;
	float m_rotationY = 2.0f;
	float m_rotationZ = -12.0f;

	float a = -5.0f;
	float b = 5.0f;

	float intensity = 1.0f;
};

#endif