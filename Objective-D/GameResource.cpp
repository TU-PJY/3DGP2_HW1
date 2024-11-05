#include "GameResource.h"
#include "ResourceFileLink.h"

// �� ������ ���ҽ��� �����Ѵ�.
// �⺻������ ���� ���ҽ��̸�, ResourceManager.h�� ���� extern ������ ��, �� ���Ͽ� �Ʒ��� ���� �����ϸ� �ȴ�.
// Scene::Init()���� ����ȴ�.

// ResourceFileLink.h�� �ۼ��� ���� ��θ� ����� �� �ִ�.

////////////////////////////////
BasicObjectShader* ObjectShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
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

Mesh* BuildingMesh; // �ǹ� �Ž�
Texture* BuildingTex; // �ǹ� �ؽ�ó

Texture* SkyboxTex; // ��ī�� �ڽ� �ؽ�ó


void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// ���������� ������ �� ���̴� ������ �������̴�.
	// �Ϲ� ������ ���̴� ����
	ObjectShader = new BasicObjectShader();
	ObjectShader->CreateDefaultPipeline(Device, RootSignature);

	// ���� �˻� ������ ���������� ����
	ObjectShader->CreateImageDepthPipelineState(Device, RootSignature);

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

	// �ٿ�彺��� ��¿� �Ž� ����
	BoundingSphereMesh = new Mesh(Device, CmdList, BOUNDING_SPHERE_MESH_DIRECTORY, MESH_TYPE_TEXT);
	////////////////////////////////

	// Play Mode
	TerrainMesh = new Mesh(Device, CmdList, "Resources//Models//terrain.bin", MESH_TYPE_BIN);
	HelicopterBodyMesh = new Mesh(Device, CmdList, "Resources//Models//Gunship.bin", MESH_TYPE_BIN);
	HelicopterWingMesh = new Mesh(Device, CmdList, "Resources//Models//Rotor.bin", MESH_TYPE_BIN);
	BuildingMesh = new Mesh(Device, CmdList, "Resources//Models//house_01.bin", MESH_TYPE_BIN);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �� �׸���� �ؽ�ó ����
	LineTex = new Texture(Device, CmdList, LINE_TEXTURE_DIRECTORY, TEXTURE_TYPE_WIC);
	////////////////////////////////

	// Home Mode
	HelpTex = new Texture(Device, CmdList, L"Resources//Image//help.png", TEXTURE_TYPE_WIC);
	ArrowTex = new Texture(Device, CmdList, L"Resources//Image//arrow.png", TEXTURE_TYPE_WIC);
	ScrollHelpTex = new Texture(Device, CmdList, L"Resources//Image//scroll_help.png", TEXTURE_TYPE_WIC);
	EnterTex = new Texture(Device, CmdList, L"Resources//Image//press_enter.png", TEXTURE_TYPE_WIC);

	// play mode
	TerrainTex = new Texture(Device, CmdList, L"Resources//Image//grass.jpg", TEXTURE_TYPE_WIC);
	TreeTex = new Texture(Device, CmdList, L"Resources//Image//tree.png", TEXTURE_TYPE_WIC);
	HelicopterTex = new Texture(Device, CmdList, L"Resources//Image//GunShip.png", TEXTURE_TYPE_WIC);
	BuildingTex = new Texture(Device, CmdList, L"Resources//Image//texture_palette.png", TEXTURE_TYPE_WIC);
	SkyboxTex = new Texture(Device, CmdList, L"Resources//Image//texture_sky.png", TEXTURE_TYPE_WIC);
}