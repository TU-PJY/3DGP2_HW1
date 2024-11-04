#pragma once
#include "MeshUtil.h"
#include "ShaderUtil.h"
#include "TextureUtil.h"

////////////////////////////////// �⺻ ���ҽ�
extern BasicObjectShader* ObjectShader;
extern BasicObjectShader* BoundboxShader;

// �� ��¿� �ؽ�ó
extern Texture* LineTex;

// �̹��� ��¿� �Ž�
extern Mesh* ImagePannel;

// ��ī�̹ڽ� ��¿� �Ž�
extern Mesh* SkyboxMesh;

// �ٿ��ڽ� ��¿� �Ž�
extern Mesh* BoundMesh;

// �ٿ�� ����� ��¿� �Ž�
extern Mesh* BoundingSphereMesh;
//////////////////////////////////

// Ŀ���� �Ž� �� �ؽ�ó
// Home Mode
extern Texture* HelpTex; // ���� �̹���
extern Texture* ArrowTex; // ��ũ�� �ȳ� �̹���
extern Texture* ScrollHelpTex;
extern Texture* EnterTex; // ���� �ȳ� �ؽ�Ʈ

// Play Mode
extern Mesh* TerrainMesh; // �ͷ��� �Ž�
extern Texture* TerrainTex; // �ͷ��� �ؽ�ó
extern Texture* TreeTex; // ���� ������ �ؽ�ó
extern Mesh* HelicopterBodyMesh; // �︮���� ��ü �κ�
extern Mesh* HelicopterWingMesh; // �︮���� ���� �κ�
extern Texture* HelicopterTex; // �︮���� �ؽ�ó

extern Mesh* BuildingMesh; // �ǹ� �Ž�
extern Texture* BuildingTex; // �ǹ� �ؽ�ó


void CreateShaderResource(ID3D12RootSignature* RootSignature, ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateMeshResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
void CreateTextureResource(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);