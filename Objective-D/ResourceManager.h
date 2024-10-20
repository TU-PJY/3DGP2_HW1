#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

////////////////////////////////// 기본 리소스
extern BasicObjectShader* BasicShader;
extern BasicObjectShader* BoundboxShader;

// 선 출력용 텍스처
extern Texture* LineTex;

// 이미지 출력용 매쉬
extern Mesh* ImagePannel;

// 스카이박스 출력용 매쉬
extern Mesh* SkyboxMesh;

// 바운드박스 출력용 매쉬
extern Mesh* BoundMesh;
//////////////////////////////////

// Home Mode
extern Texture* HelpTex; // 도움말 표시 이미지
extern Texture* ArrowTex; // 스크롤 안내 이미지
extern Texture* ScrollHelpTex;
extern Texture* EnterTex; // 시작 안내 텍스트

// Play Mode
extern Mesh* TerrainMesh; // 터레인 매쉬
extern Texture* TerrainTex; // 터레인 텍스처
extern Texture* TreeTex; // 나무 빌보드 텍스처

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);