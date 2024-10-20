#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

////////////////////////////////// �⺻ ���ҽ�
extern BasicObjectShader* BasicShader;
extern BasicObjectShader* BoundboxShader;

// �� ��¿� �ؽ�ó
extern Texture* LineTex;

// �̹��� ��¿� �Ž�
extern Mesh* ImagePannel;

// ��ī�̹ڽ� ��¿� �Ž�
extern Mesh* SkyboxMesh;

// �ٿ��ڽ� ��¿� �Ž�
extern Mesh* BoundMesh;
//////////////////////////////////

// Home Mode
extern Texture* HelpTex; // ���� ǥ�� �̹���
extern Texture* ArrowTex; // ��ũ�� �ȳ� �̹���
extern Texture* ScrollHelpTex;
extern Texture* EnterTex; // ���� �ȳ� �ؽ�Ʈ

// Play Mode
extern Mesh* TerrainMesh; // �ͷ��� �Ž�
extern Texture* TerrainTex; // �ͷ��� �ؽ�ó
extern Texture* TreeTex; // ���� ������ �ؽ�ó

void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);