#include "visuals.h"
#include "Overlay.h"
#include "xorstr.hpp"
#include "PMemHelper.h"
#include <string>
#include <cstring>
#include <locale>
#include <codecvt>
#include "math.hpp"

PRENDER* PRENDER::Instance()
{
	static PRENDER instance;
	return &instance;
	

}


void PRENDER::Render()
{
	//Game::UPlayer player;
	///*if (eftdata->offsets.gameWorld == 0 || eftdata->offsets.localGameWorld == 0 || eftdata->offsets.fpsCamera == 0) {
	//	return;
	//}*/
	//player.dist=0;
	//D3DCOLOR color = 0;
	//auto xyiP = eftdata->players;
	//for (int i = 0; i < entities.size(); i++)
	//{
	//	if (!player.instance)
	//		continue;

	//	if (player.IsLocal == true) {
	//		eftdata->localPlayer = player;
	//		continue;
	//	}

	//	EFTData::Instance()->DrawSkeleton(player.instance);

	//	distance = local_player.location.Distance(player.location);
	//	if (GetKeyState(VK_NUMPAD5)) {
	//		if (distance > 2500.f)
	//			continue;
	//	}
	//	else {
	//		if (distance > MaxDrawDistance)
	//			continue;
	//	}
	//	if (GetKeyState(VK_NUMPAD6)) {
	//		if (distance > 100.f)
	//			continue;
	//	}
	//	if (distance <= 150.f)
	//		color = D3DCOLOR_ARGB(255, 243, 77, 77); //color red, if less than 150m
	//	else if (distance > 150.f && distance <= 300.f)
	//		color = D3DCOLOR_ARGB(255, 130, 236, 23); //color yellow, if less than 300m and greater than 150m


	//	D2D1_POINT_2F screen_pos;
	//	if (Game::WorldToScreenX())
	//	{
	//		String((int)screen_pos.x, (int)screen_pos.y, color, pFont, true, _xor_("%0.2fm").operator const char* (), distance);

	//		String((int)screen_pos.x, (int)screen_pos.y + 13, D3DCOLOR_ARGB(250, 255, 91, 91), pFont2, true, _xor_("%s").operator const char* (), player.name.c_str());

	//	}

	//	if (WorldToScreenv2(player.headPos, screen_pos))
	//	{
	//		float headSZ = 20;
	//		headSZ = 2 + (20 / distance);
	//		Circle((int)screen_pos.x, (int)screen_pos.y - headSZ, 4, 0, 1, true, 32, D3DCOLOR_ARGB(255, 255, 255, 255));
	//	}


	//}
	//std::vector<EFTExtract> xyie(EFTData::Instance()->extracts);
	//auto xyie = eftdata->extracts;
	//for (auto extract : xyie)
	//{
	//	if (!extract.instance)
	//		continue;
	//	distance2 = local_player.location.Distance(extract.location);
	//	D2D1_POINT_2F screen_pos;

	//	if (WorldToScreenv2(extract.location, screen_pos))
	//	{
	//		String((int)screen_pos.x, (int)screen_pos.y, D3DCOLOR_ARGB(255, 199, 45, 199), pFont, true, _xor_("%0.2fm").operator const char* (), distance2);
	//		String((int)screen_pos.x, (int)screen_pos.y + 13, D3DCOLOR_ARGB(199, 45, 236, 199), pFont, true, "%s", extract.name.c_str());

	//	}

	//}

	//std::vector<EFTPlayer> xyiP(EFTData::Instance()->players);
	//if (GetKeyState(VK_NUMPAD4)) {
	//	String(3, 28, D3DCOLOR_ARGB(255, 255, 0, 0), pFont, true, "–ÛÍË-«‡„Â·ÛÍË");

	//	if (globals.HardSettings != 0 && memio->ReadData<uint64_t>(globals.HardSettings + 0x1F4) != 5.0f) {
	//		memio->WriteData(globals.HardSettings + 0x1F4, 5.0f);
	//	}

	//	if (globals.GameWorld != 0 && memio->ReadData<uint64_t>(globals.GameWorld + 0x14) != 0) {
	//		memio->WriteData(globals.GameWorld + 0x14, 0);
	//	}

	//}

	//if (GetKeyState(VK_NUMPAD1)) {
	//	String(3, 1, D3DCOLOR_ARGB(255, 130, 236, 23), pFont, true, "ÕŒ” –≈ Œ»À");
	//}
	//if (GetKeyState(VK_NUMPAD3)) {
	//	String(3, 10, D3DCOLOR_ARGB(255, 130, 236, 23), pFont, true, "¿ÕÀ»Ã»“≈ƒ —“¿Ã»Õ¿");
	//}
	//if (GetKeyState(VK_NUMPAD9)) {
	//	String(3, 46, D3DCOLOR_ARGB(255, 251, 0, 255), pFont, true, "–Œ«Œ¬€≈ Œ◊ »");
	//}
	//float MaxDrawDistanceBaseitems = 100.f;
	//std::vector<EFTLoot> xyi(EFTData::Instance()->loot);
	//auto xyi = eftdata->loot;
	//if (GetKeyState(VK_NUMPAD0)) {
	//	String(3, 19, D3DCOLOR_ARGB(255, 130, 236, 23), pFont, true, "À”””“≈÷÷");
	//	if (GetKeyState(VK_NUMPAD8)) {
	//		String(3, 37, D3DCOLOR_ARGB(255, 0, 102, 51), pFont, true, "‘»À‹“≈–");
	//	}
	//	for (auto& loot : xyi)
	//	{




	//		if (!loot.instance)
	//			continue;
	//		if (GetKeyState(VK_NUMPAD8) && loot.Find != true) {
	//			continue;
	//		}


	//		distance = local_player.location.Distance(loot.origin);

	//		if (distance > MaxDrawDistanceBaseitems && !loot.Find)
	//			if (distance > MaxDrawDistanceBaseitems)
	//			continue;

	//		D2D1_POINT_2F screen_pos;


	//		if (WorldToScreenv2(loot.origin, screen_pos))
	//		{
	//			String((int)screen_pos.x, (int)screen_pos.y, loot.color, pFont, true, _xor_("[%0.1fm] | %s").operator const char* (), distance, loot.ShortName.c_str());

	//		}




	//	}
	//}
	
}

float D3DXVec3Dot2(FVector a, FVector b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}



