////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_TextureShader = 0;
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

	height = screenHeight;
	width = screenWidth;


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
	m_Camera->SetPosition(10.0f, 10.0f, -20.0f);
//	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);	// for cube model
//	m_Camera->SetPosition(0.0f, 0.5f, -3.0f);	// for chair model
	
	// Create the model object.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}

	// Initialize the model object.
	//result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/DDG51.obj", L"./data/DDG51.dds");
	//result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/sea.dds");
//	result = m_Model->Initialize(m_D3D->GetDevice(), L"./data/chair.obj", L"./data/chair_d.dds");

	for (int i = 0; i < 10; i++)
	{
		ModelClass* model = new ModelClass;
		if (!model) return false;

		result = model->CallInitialize(m_D3D->GetDevice(), i);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize a model object.", L"Error", MB_OK);
			return false;
		}
		models.push_back(model);
	}

	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/sky.dds", screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the billboard object.
	m_BillboardModel = new ModelClass;
	if (!m_BillboardModel)
	{
		return false;
	}

	// Initialize the billboard object.
	result = m_BillboardModel->Initialize(m_D3D->GetDevice(), L"./data/cube.obj", L"./data/lighthouse.dds", 1);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the billboard object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}


	m_Camera->Render();
	XMMATRIX baseViewMatrix;
	m_Camera->GetViewMatrix(baseViewMatrix);

	//Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd,
		screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the TextClass object.", L"Error", MB_OK);
		return false;
	}

	//Initialize the text object.
	for (int i = 0; i < 5; i++)
	{
		TextClass* text = new TextClass;
		if (!text) return false;

		result = text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize a Text object.", L"Error", MB_OK);
			return false;
		}
		texts.push_back(text);
	}

	// Initialize the Title object.

	m_Title = new BitmapClass;
	if (!m_Title)
	{
		return false;
	}
	result = m_Title->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title.dds", screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}


	//result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	//if (!result)
	//{
	//	MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
	//	return false;
	//}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the texture shader object.
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

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

	return;
}


bool GraphicsClass::Frame(int fps, int cpu)
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.01f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}


	for (int i = 0; i < 5; i++)
	{
		switch (i)
		{
		case 0:
			result = texts[i]->SetFPS(fps, m_D3D->GetDeviceContext());
			break;
		case 1:
			result = texts[i]->SetCPU(cpu, m_D3D->GetDeviceContext());
			break;
		case 2:
			result = texts[i]->SetIndexCount(GetIndexCount() / 3, m_D3D->GetDeviceContext());
			break;
		case 3:
			result = texts[i]->SetObjectCount(GetObjectCount(), m_D3D->GetDeviceContext());
			break;
		case 4:
			result = texts[i]->SetResolution(width, height, m_D3D->GetDeviceContext());
			break;
		}
	}

	if (!result)
	{
		return false;
	}

	//// Set the frames per second.
	//result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	//if (!result)
	//{
	//	return false;
	//}

	//// Set the cpu usage.
	//result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	//if (!result)
	//{
	//	return false;
	//}

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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;

	//XMVECTOR translation = worldMatrix.r[3]; // 4번째 row가 위치 벡터임
	XMFLOAT3 position;
	//XMStoreFloat3(&position, translation);

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->GetOrthoMatrix(orthoMatrix);

	XMMATRIX identity = XMMatrixIdentity();

	//result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	//if (!result)
	//{
	//	return false;
	//}



	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0, false);
	if (!result)
	{
		return false;
	}
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(),  1, worldMatrix, identity, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	//worldMatrix *= XMMatrixRotationX(rotation);
	//worldMatrix *= XMMatrixRotationY(rotation);

	//worldMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	for (int i = 0; i < 5; i++)
	{
		texts[i]->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	}


	rot += (float)XM_PI * 0.01f;
	if (rot > 360.0f)
	{
		rot -= 360.0f;
	}

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (int i = 0; i < models.size(); i++)
	{
		XMMATRIX m_WorldMatrix = worldMatrix;

		switch (i)
		{
		case 0: //바닥
			m_WorldMatrix *= XMMatrixScaling(150.0f, 100.0f, 150.0f);
			m_WorldMatrix *= XMMatrixTranslation(0.0f, -50.0f, 0.0f);
			break;
		case 1: //군함
			m_WorldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);
			m_WorldMatrix *= XMMatrixRotationY(-XM_PI/4 );
			m_WorldMatrix *= XMMatrixTranslation((position.x - 30.0f), 0.0f, position.z-3.0f);
			break;
		case 2:
			m_WorldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);
			//m_WorldMatrix *= XMMatrixRotationX(-XM_PI / 2);
			m_WorldMatrix *= XMMatrixTranslation((position.x - 15.0f), 0.0f, position.z + 30.0f);
			break;
		case 3:
			m_WorldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);
			//m_WorldMatrix *= XMMatrixRotationX(-XM_PI / 2);
			m_WorldMatrix *= XMMatrixTranslation((position.x + 25.0f), 0.0f, position.z + 50.0f);
			break;
		case 4: //물고기
			m_WorldMatrix *= XMMatrixScaling(0.4f, 0.4f, 0.4f);
			m_WorldMatrix *= XMMatrixRotationX(-XM_PI / 2);
			m_WorldMatrix *= XMMatrixRotationX(-rot * 3 / 2);
			m_WorldMatrix *= XMMatrixTranslation((position.x + 60.0f), 0.5f, position.z + 3.0f);
			m_WorldMatrix *= XMMatrixRotationX(rot * 3 / 2);
			m_WorldMatrix *= XMMatrixTranslation((position.x - 15.0f), 0.5f, position.z - 40.0f);
			break;
		case 5:
			m_WorldMatrix *= XMMatrixScaling(0.02f, 0.02f, 0.02f);
			m_WorldMatrix *= XMMatrixRotationY(-XM_PI);
			m_WorldMatrix *= XMMatrixRotationX(-rot);
			m_WorldMatrix *= XMMatrixTranslation((position.x + 60.0f), 0.0f, position.z - 3.0f);
			m_WorldMatrix *= XMMatrixRotationX(rot);
			m_WorldMatrix *= XMMatrixTranslation((position.x - 30.0f), 0.0f, position.z - 30.0f);
			break;
		case 6: //갈모매
			m_WorldMatrix *= XMMatrixScaling(0.2f, 0.2f, 0.2f);
			m_WorldMatrix *= XMMatrixRotationX(XM_PI / 2);
			m_WorldMatrix *= XMMatrixRotationY(-XM_PI/6);
			m_WorldMatrix *= XMMatrixTranslation((position.x - 4 * i), 25.0f, position.z - 40.0f);
			break;
		case 7: //어선
			m_WorldMatrix *= XMMatrixScaling(0.3f, 0.3f, 0.3f);
			m_WorldMatrix *= XMMatrixTranslation((position.x + 10.0f), 0.0f, position.z - 3.0f);
			m_WorldMatrix *= XMMatrixRotationY(rot/2);
			m_WorldMatrix *= XMMatrixTranslation((position.x + 30.0f), 0.0f, position.z - 30.0f);
			break;
		case 8: //해적선
			m_WorldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);
			m_WorldMatrix *= XMMatrixRotationY(XM_PI / 6);
			m_WorldMatrix *= XMMatrixTranslation((position.x + 40.0f), 0.0f, position.z - 20.0f);
			break;
		case 9:
			m_WorldMatrix *= XMMatrixScaling(0.1f, 0.1f, 0.1f);
			m_WorldMatrix *= XMMatrixRotationY(-XM_PI * 2 / 3);
			m_WorldMatrix *= XMMatrixTranslation((position.x + -35.0f), 0.0f, position.z - 40.0f);
			break;

		}
		models[i]->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), models[i]->GetIndexCount(), models[i]->GetInstanceCount(),
			m_WorldMatrix, viewMatrix, projectionMatrix, models[i]->GetTexture());
		if (!result)
		{
			return false;
		}
	}


	XMFLOAT3 cameraPosition = m_Camera->GetPosition();
	XMFLOAT3 modelPosition = { 0.0f, 0.0f, 0.0f }; // 모델의 중심 위치

	// 카메라와 모델 간의 방향 계산 (X-Z 평면)
	float deltaX = modelPosition.x - cameraPosition.x;
	float deltaZ = modelPosition.z - cameraPosition.z;

	float angle = atan2(deltaX, deltaZ); // 라디안

	// 회전 행렬
	XMMATRIX rotationMatrix = XMMatrixRotationY(angle);

	// 스케일 및 위치 변환
	XMMATRIX scaleMatrix = XMMatrixScaling(10.0f, 15.0f, 0.01f);
	XMMATRIX translationMatrix = XMMatrixTranslation(modelPosition.x, modelPosition.y + 6.5f, modelPosition.z);

	XMMATRIX b_WorldMatrix = scaleMatrix * rotationMatrix * translationMatrix;

	m_BillboardModel->Render(m_D3D->GetDeviceContext());

	if (!m_TextureShader->Render(m_D3D->GetDeviceContext(), m_BillboardModel->GetIndexCount(), 1, b_WorldMatrix, viewMatrix, projectionMatrix,
		m_BillboardModel->GetTexture()))
	{
		return false;
	}

	if (title) //타이틀 화면 띄우기
	{
		m_D3D->TurnZBufferOff();
		result = m_Title->Render(m_D3D->GetDeviceContext(), 0, 0, true);
		if (!result)
		{
			return false;
		}
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Title->GetIndexCount(), 1, worldMatrix, identity, orthoMatrix, m_Title->GetTexture());
		if (!result)
		{
			return false;
		}
		m_D3D->TurnZBufferOn();
	}
	
	//m_Model->Render(m_D3D->GetDeviceContext());

	// Render the model using the texture shader.


	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

CameraClass* GraphicsClass::GetCamera()
{
	return m_Camera;
}

int GraphicsClass::GetIndexCount()
{
	int sum = 0;
	for (int i = 0; i < models.size(); i++)
	{
		sum += models[i]->GetIndexCount() * models[i]->GetInstanceCount();
	}
	return sum;
}

int GraphicsClass::GetObjectCount()
{
	int sum = 0;
	for (int i = 0; i < models.size(); i++)
	{
		sum += 1 * models[i]->GetInstanceCount();
	}
	return sum;
}

void GraphicsClass::SetTitleFalse()
{
	title = false;
}

bool GraphicsClass::GetTitleB()
{
	return title;
}