#include "GameResource.h"
#include "ResourceFileLink.h"

// 이 파일은 리소스를 관리한다.
// 기본적으로 전역 리소스이며, ResourceManager.h에 먼저 extern 선언한 뒤, 이 파일에 아래와 같이 정의하면 된다.
// Scene::Init()에서 실행된다.

// ResourceFileLink.h에 작성한 파일 경로를 사용할 수 있다.

////////////////////////////////
BasicObjectShader* ObjectShader;
BasicObjectShader* BoundboxShader;
Texture* LineTex;
Mesh* BoundMesh;
Mesh* BoundingSphereMesh;
////////////////////////////////

// Home Mode
Texture* HelpTex; // 도움말 이미지
Texture* ArrowTex; // 스크롤 안내 이미지
Texture* ScrollHelpTex;
Texture* EnterTex; // 시작 안내 텍스트

// Play Mode
Mesh* TerrainMesh; // 터레인 매쉬
Texture* TerrainTex; // 터레인 텍스처
Texture* TreeTex; // 나무 빌보드 텍스처
Mesh* HelicopterBodyMesh; // 헬리콥터 몸체 부분
Mesh* HelicopterWingMesh; // 헬리콥터 날개 부분
Texture* HelicopterTex; // 헬리콥터 텍스처

Mesh* BuildingMesh; // 건물 매쉬
Texture* BuildingTex; // 건물 텍스처

Texture* SkyboxTex; // 스카이 박스 텍스처


void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	////////////////////////////////
	// 파이프라인 생성이 곧 쉐이더 설정의 마무리이다.
	// 일반 렌더링 쉐이더 생성
	ObjectShader = new BasicObjectShader();
	ObjectShader->CreateDefaultPipeline(Device, RootSignature);

	// 깊이 검사 미포함 파이프라인 생성
	ObjectShader->CreateImageDepthPipelineState(Device, RootSignature);

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

	// 바운드스페어 출력용 매쉬 생성
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
	// 선 그리기용 텍스처 생성
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