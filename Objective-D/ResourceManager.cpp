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

// ���� �̹���
Texture* HelpTex;

// ��ũ�� �ȳ� �̹���
Texture* ArrowTex;
Texture* ScrollHelpTex;

// ���� �ȳ� �ؽ�Ʈ
Texture* EnterTex;

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

}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// �� �׸���� �ؽ�ó ����
	LineTex = new Texture(Device, CmdList, L"Resources//Image//line_tex.png");
	////////////////////////////////

	HelpTex = new Texture(Device, CmdList, L"Resources//Image//help.png");
	ArrowTex = new Texture(Device, CmdList, L"Resources//Image//arrow.png");
	ScrollHelpTex = new Texture(Device, CmdList, L"Resources//Image//scroll_help.png");
	EnterTex = new Texture(Device, CmdList, L"Resources//Image//press_enter.png");
}