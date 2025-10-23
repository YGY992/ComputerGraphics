////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	//m_Model = 0;
	m_LightShader = 0;
	m_Light1 = 0;
	m_Light2 = 0;
	m_Light3 = 0;
	m_Light4 = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 2.0f, -12.0f);	// for cube
//	m_Camera->SetPosition(0.0f, 0.5f, -3.0f);	// for chair
		
	// Create the model object.
	//m_Model = new ModelClass;
	//if(!m_Model)
	//{
	//	return false;
	//}

	// Initialize the model object.
//	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/seafloor.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/chair.obj", L"./data/chair_d.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/daraq.obj", L"./data/daraq.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/dummy.obj", L"./data/dummy.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/trainer.obj", L"./data/trainer.dds");

	for (int i = 0; i < 4; i++)
	{
		ModelClass* model = new ModelClass;
		if (!model) return false;

		result = model->CallInitialize(m_D3D->GetDevice(), i);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize a model object.", L"Error", MB_OK);
			return false;
		}
		m_Models.push_back(model);
	}

	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the first light object.
	m_Light1 = new LightClass;
	if (!m_Light1)
	{
		return false;
	}

	// Initialize the first light object.
	//m_Light1->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->SetPosition(0.0f, 1.0f, 3.0f);

	// Initialize the light object.
	m_Light1->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	//m_Light1->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Light->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_Light1->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	m_Light->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	//	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	//	m_Light->SetDirection(1.0f, 0.0f, 0.0f);
	m_Light1->SetDirection(0.0f, -1.0f, 3.0f);
	m_Light1->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light1->SetSpecularPower(32.0f);

	// Create the second light object.
	m_Light2 = new LightClass;
	if (!m_Light2)
	{
		return false;
	}

	// Initialize the second light object.
	m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light2->SetPosition(-200.0f, 2.0f, -0.0f);

	// Create the third light object.
	m_Light3 = new LightClass;
	if (!m_Light3)
	{
		return false;
	}

	// Initialize the third light object.
	m_Light3->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Light3->SetPosition(3000.0f, 2.0f, 0.0f);

	// Create the fourth light object.
	m_Light4 = new LightClass;
	if (!m_Light4)
	{
		return false;
	}

	// Initialize the fourth light object.
	m_Light4->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Light4->SetPosition(200.0f, 2.0f, 0.0f);


	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the model object.
	//if(m_Model)
	//{
	//	m_Model->Shutdown();
	//	delete m_Model;
	//	m_Model = 0;
	//}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the light object.
	if (m_Light1)
	{
		delete m_Light1;
		m_Light1 = 0;
	}

	if (m_Light2)
	{
		delete m_Light2;
		m_Light2 = 0;
	}

	if (m_Light3)
	{
		delete m_Light3;
		m_Light3 = 0;
	}

	if (m_Light4)
	{
		delete m_Light4;
		m_Light4 = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	XMFLOAT3 camPos = m_Camera->GetPosition();

	// 간단한 WASD 입력
	float speed = 0.1f; // 이동 속도
	if (GetAsyncKeyState('W') & 0x8000) camPos.z += speed;
	if (GetAsyncKeyState('S') & 0x8000) camPos.z -= speed;
	if (GetAsyncKeyState('A') & 0x8000) camPos.x -= speed;
	if (GetAsyncKeyState('D') & 0x8000) camPos.x += speed;
	if (GetAsyncKeyState('Z') & 0x8000) camPos.y += speed;
	if (GetAsyncKeyState('C') & 0x8000) camPos.y -= speed;

	a -= 0.1f;
	b += 0.1f;
	
	//m_Light2->SetPosition(a, 2.0f, 5.0f);
	//m_Light3->SetPosition();
	//m_Light4->SetPosition(b, 2.0f, 5.0f);

	m_Camera->SetPosition(camPos.x, camPos.y, camPos.z);

	static float rotation = 0.0f;

	//m_Camera->SetPosition(0.0f, 2.0f, -12.0f);

	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// 다중 조명 데이터 구성
	XMFLOAT4 pointDiffuse[4];
	XMFLOAT4 pointPos[4];

	pointDiffuse[0] = m_Light1->GetDiffuseColor();  pointPos[0] = m_Light1->GetPosition();
	pointDiffuse[1] = m_Light2->GetDiffuseColor();  pointPos[1] = m_Light2->GetPosition();
	pointDiffuse[2] = m_Light3->GetDiffuseColor();  pointPos[2] = m_Light3->GetPosition();
	pointDiffuse[3] = m_Light4->GetDiffuseColor();  pointPos[3] = m_Light4->GetPosition();

	for (int i = 1; i < 4; i++)
	{
		pointDiffuse[i].x = pointDiffuse[i].x * intensity;
		pointDiffuse[i].y = pointDiffuse[i].y * intensity;
		pointDiffuse[i].z = pointDiffuse[i].z * intensity;
		pointDiffuse[i].w = 1.0f;
	}

	// Ambient/Specular 등은 1번 라이트 기준으로 사용
	XMFLOAT4 ambientColor = m_Light1->GetAmbientColor();
	XMFLOAT4 specularColor = m_Light1->GetSpecularColor();
	float specularPower = m_Light1->GetSpecularPower();
	XMFLOAT3 lightDir = m_Light1->GetDirection();
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_rotationY += 0.03f;

	if (m_rotationY > XM_2PI) m_rotationY -= XM_2PI;

	for (int i = 0; i < m_Models.size(); ++i)
	{


		ModelClass* model = m_Models[i];

		XMMATRIX modelWorldMatrix = worldMatrix;

		if (i == 0)
		{
			//modelWorldMatrix *= XMMatrixScaling(800.0f, 800.0f, 0.0f);

			m_rotationZ = 0.6f;
			//modelWorldMatrix *= XMMatrixRotationX(m_rotationZ);

			//modelWorldMatrix *= XMMatrixTranslation(0.0f, 200.0f, 400.0f);
			//z -= 1.0f;
			//modelWorldMatrix *= XMMatrixScaling(80.0f, 80.0f, 80.0f);
			//modelWorldMatrix *= XMMatrixTranslation(0.0f, -45.0f, 0.0f);
			modelWorldMatrix *= XMMatrixScaling(80.0f, 1.0f, 80.0f);
			modelWorldMatrix *= XMMatrixTranslation(0.0f, -5.0f, 0.0f);

		}
		else
		{
			modelWorldMatrix *= XMMatrixScaling(0.025f, 0.025f, 0.025f);

			modelWorldMatrix *= XMMatrixRotationY(m_rotationY);

			modelWorldMatrix *= XMMatrixTranslation((float)(i - 2) * 5.0f, -1.0f, 5.0f);
		}

		model->Render(m_D3D->GetDeviceContext());

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), model->GetIndexCount(),
			modelWorldMatrix, viewMatrix, projectionMatrix,
			model->GetTexture(),
			m_Light1->GetDirection(), m_Light1->GetAmbientColor(), m_Light1->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light1->GetSpecularColor(), m_Light1->GetSpecularPower(),
			pointPos, pointDiffuse);

		if (!result)
			return false;
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

void GraphicsClass::SetAmbient(bool val)
{
	if (!val)
	{
		m_Light1->SetAmbientColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_Light1->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	}
}


void GraphicsClass::SetDiffuse(bool val)
{
	if (!val)
	{
		m_Light1->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light2->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light3->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
		m_Light4->SetDiffuseColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		m_Light1->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Light2->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
		m_Light3->SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
		m_Light4->SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	}
}

void GraphicsClass::SetSpecular(bool val)
{
	if (!val)
	{
		m_Light1->SetSpecularPower(100000.f);
	}
	else
	{
		m_Light1->SetSpecularPower(32.0f);
	}
}


void GraphicsClass::IncreaseIntensity()
{
	intensity += 0.05f;

	if (intensity > 100.0f)
	{
		intensity = 100.0f;
	}
}

void GraphicsClass::DecreaseIntensity()
{
	intensity -= 0.05f;

	if (intensity < 0.0f)
	{
		intensity = 0.0f;
	}
}

