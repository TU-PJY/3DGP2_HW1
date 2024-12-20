#include "CBVResource.h"
#include "CBVUtil.h"

CBV FlipCBV; // texture flip
CBV LightCBV;  // light
CBV BoolLightCBV;  // light bool info

// 상수버퍼로 사용할 버퍼 및 힙을 설정한다.
void CreateCBVResource(ID3D12Device* Device) {
	// texture flip
	FlipInfo TexFlip[4]{ {0, 0}, {1, 0}, {0, 1}, {1, 1} };
	ReserveCBV(FlipCBV, 4);
	for (int i = 0; i < 4; ++i)
		CBVUtil::Create(Device, &TexFlip[i], sizeof(TexFlip[i]), FlipCBV, i);

	// light data
	LightInfo Light{
		{10.0f, -10.0f, 5.0f },  // Light Position
		0.0,
		{1.0f, 0.9f, 0.8f },  // Light Color
		0.0,
		{0.5f, 0.5f, 0.5f },  // Ambient Light Color
		1.5,  // Shadow Strength
	};
	ReserveCBV(LightCBV, 1);
	CBVUtil::Create(Device, &Light, sizeof(Light), LightCBV, 0);

	// light bool info
	UseLightInfo Uselightinfo[2]{ {0}, {1} };
	ReserveCBV(BoolLightCBV, 2);
	for (int i = 0; i < 2; ++i)
		CBVUtil::Create(Device, &Uselightinfo[i], sizeof(Uselightinfo[i]), BoolLightCBV, i);
}