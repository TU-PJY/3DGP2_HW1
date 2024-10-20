#include "ResourceManager.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Framework::Init()에서 실행된다.

////////////////////////////////
BasicObjectShader* BasicShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex;
Mesh* BoundMesh;
////////////////////////////////

// Home Mode
Texture* HelpTex; // 도움말 이미지
Texture* ArrowTex; // 스크롤 안내 이미지
Texture* ScrollHelpTex;
Texture* EnterTex; // 시작 안내 텍스트

// Play Mode
Mesh* TerrainMesh; // 터레인 매쉬
Texture* TerrainTex; // 터레인 텍스처

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 파이프라인 생성이 곧 쉐이더 설정의 마무리이다.
	// 일반 렌더링 쉐이더 생성
	BasicShader = new BasicObjectShader();
	BasicShader->CreateDefaultPipeline(Device, RootSignature);

	// 깊이 검사 미포함 파이프라인 생성
	BasicShader->CreateImageDepthPipelineState(Device, RootSignature);

	// 바운드박스 쉐이더 생성
	BoundboxShader = new BasicObjectShader();
	BoundboxShader->CreateBoundboxPipeline(Device, RootSignature);
	////////////////////////////////
}

void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 이미지 출력용 패널 생성
	ImagePannel = new Mesh;
	ImagePannel->CreateImagePannelMesh(Device, CmdList);

	// 스카이박스 출력용 매쉬 생성
	SkyboxMesh = new Mesh;
	SkyboxMesh->CreateSkyboxMesh(Device, CmdList);

	// 바운드박스 출력용 매쉬 생성
	BoundMesh = new Mesh;
	BoundMesh->CreateBoundboxMesh(Device, CmdList);
	////////////////////////////////

	// Play Mode
	TerrainMesh = new Mesh(Device, CmdList, "Resources//Models//terrain.bin", MeshType::Binary);
}

void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 선 그리기용 텍스처 생성
	LineTex = new Texture(Device, CmdList, L"Resources//Image//line_tex.png");
	////////////////////////////////

	// Home Mode
	HelpTex = new Texture(Device, CmdList, L"Resources//Image//help.png");
	ArrowTex = new Texture(Device, CmdList, L"Resources//Image//arrow.png");
	ScrollHelpTex = new Texture(Device, CmdList, L"Resources//Image//scroll_help.png");
	EnterTex = new Texture(Device, CmdList, L"Resources//Image//press_enter.png");

	// play mode
	TerrainTex = new Texture(Device, CmdList, L"Resources//Image//grass.jpg");
}