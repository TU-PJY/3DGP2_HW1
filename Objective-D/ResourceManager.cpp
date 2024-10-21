#include "ResourceManager.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Framework::Init()���� ����ȴ�.

////////////////////////////////
BasicObjectShader* BasicShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex;
Mesh* BoundMesh;
////////////////////////////////

// Home Mode
Texture* HelpTex; // ���� �̹���
Texture* ArrowTex; // ��ũ�� �ȳ� �̹���
Texture* ScrollHelpTex;
Texture* EnterTex; // ���� �ȳ� �ؽ�Ʈ

// Play Mode
Mesh* TerrainMesh; // �ͷ��� �Ž�
Texture* TerrainTex; // �ͷ��� �ؽ�ó
Texture* TreeTex; // ���� ������ �ؽ�ó
Mesh* HelicopterBodyMesh; // �︮���� ��ü �κ�
Mesh* HelicopterWingMesh; // �︮���� ���� �κ�
Texture* HelicopterTex; // �︮���� �ؽ�ó

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// ���������� ������ �� ���̴� ������ �������̴�.
	// �Ϲ� ������ ���̴� ����
	BasicShader = new BasicObjectShader();
	BasicShader->CreateDefaultPipeline(Device, RootSignature);

	// ���� �˻� ������ ���������� ����
	BasicShader->CreateImageDepthPipelineState(Device, RootSignature);

	// �ٿ��ڽ� ���̴� ����
	BoundboxShader = new BasicObjectShader();
	BoundboxShader->CreateBoundboxPipeline(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �̹��� ��¿� �г� ����
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);

	// ��ī�̹ڽ� ��¿� �Ž� ����
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	// �ٿ��ڽ� ��¿� �Ž� ����
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);
	////////////////////////////////

	// Play Mode
	TerrainMesh = new Mesh(Device, CmdList, "Resources//Models//terrain.bin", MeshType::Binary);
	HelicopterBodyMesh = new Mesh(Device, CmdList, "Resources//Models//Gunship.bin", MeshType::Binary);
	HelicopterWingMesh = new Mesh(Device, CmdList, "Resources//Models//Rotor.bin", MeshType::Binary);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �� �׸���� �ؽ�ó ����
	LineTex = new Texture(Device, CmdList, L"Resources//Image//line_tex.png");
	////////////////////////////////

	// Home Mode
	HelpTex = new Texture(Device, CmdList, L"Resources//Image//help.png");
	ArrowTex = new Texture(Device, CmdList, L"Resources//Image//arrow.png");
	ScrollHelpTex = new Texture(Device, CmdList, L"Resources//Image//scroll_help.png");
	EnterTex = new Texture(Device, CmdList, L"Resources//Image//press_enter.png");

	// play mode
	TerrainTex = new Texture(Device, CmdList, L"Resources//Image//grass.jpg");
	TreeTex = new Texture(Device, CmdList, L"Resources//Image//tree.png");
	HelicopterTex = new Texture(Device, CmdList, L"Resources//Image//GunShip.png");
}