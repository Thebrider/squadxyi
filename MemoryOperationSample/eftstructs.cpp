#include "eftstructs.h"
#include "xorstr.hpp"
#include "visuals.h"
#include <xmmintrin.h>  
#include <emmintrin.h>
#include <fstream>
#include <locale>
#include <codecvt>
#include "Overlay.h"
#include "PMemHelper.h"
#include <boost/algorithm/string.hpp>

std::vector<Game::UPlayer> entities = {};
EFTData* EFTData::Instance()
{
	static EFTData instance;
	return &instance;



}
/* All one time initialization in here*/
bool EFTData::InitOffsets()
{
	Game::gameInstance = memio->ReadData<uintptr_t>(globals.World + 0x180); // Offsets::Classes::UWorld::OwningGameInstance /
	Game::persistentLevel = memio->ReadData<uintptr_t>(globals.World + 0x30); // Offsets::Classes::UWorld::PersistentLevel /
	Game::localPlayerPtr = memio->ReadData<uintptr_t>(Game::gameInstance + 0x38); // Offsets::Classes::UWorld::UGameInstance::LocalPlayers /
	Game::localPlayer = memio->ReadData<uintptr_t>(Game::localPlayerPtr);  // LocalPlayer lol /
	Game::playerController = memio->ReadData<uintptr_t>(Game::localPlayer + 0x30); // Offsets::Classes::UWorld::UGameInstance::UPlayer::PlayerController /
	Game::PlayerStateLocalPlayer = memio->ReadData<uintptr_t>(Game::playerController + 0x230);// Offsets::Classes::AActor::UObject::AController::PlayerState /
	Game::LocalTeamId.TeamId = memio->ReadData<int>(Game::PlayerStateLocalPlayer + 0x400);// Offsets::Classes::1PlayerState::Ainfo::AActor::UObject::ASQPlayerState::TeamId 0x420
	Game::pawn = memio->ReadData<uintptr_t>(Game::playerController + 0x2a8); // Offsets::Classes::UWorld::UGameInstance::UPlayer::APlayerController::AcknowledgedPawn /
	Game::cameraManager = memio->ReadData<uintptr_t>(Game::playerController + 0x2c0); //Offsets::Classes::UWorld::UGameInstance::UPlayer::APlayerController::PlayerCameraManager aussi offset_camera_manager
	Game::cameraCache = memio->ReadData<Game::FCameraCacheEntry>(Game::cameraManager + 0x1af0); // offset_camera_cache
	Game::actorsArray = memio->ReadData<uintptr_t>(Game::persistentLevel + 0x98); // Offsets::Classes::UWorld::ULevel::UNetConnection::OwningActor
	Game::actorsCount = memio->ReadData<int>(Game::persistentLevel + 0xa0); // Offsets::Classes::UWorld::ULevel::UNetConnection::MaxPacket

	// stores all data
	std::vector<Game::UPlayer> tmp{};


	for (int i = 0; i < Game::actorsCount; i++)
	{

		uint64_t Entity = memio->ReadData<uint64_t>(Game::actorsArray + (i * 0x8));
		if (!Entity) continue;

		Game::UPlayer player;
		player.instance = Entity;
		player.objectId = memio->ReadData<int>(Entity + 0x18); // offset_actor_id

		//if (GetNameById(player.objectId).find("BP_Soldier") != std::string::npos)
		{
			player.instigator = memio->ReadData<uintptr_t>(player.instance + 0x120); // AActor::Instigator 
			player.PlayerState = memio->ReadData<uintptr_t>(player.instigator + 0x230); // APawn::PlayerState
			player.team = memio->ReadData<int>(player.PlayerState + 0x400); // ASQPlayerState::Teamid
			player.root_component = memio->ReadData<uintptr_t>(player.instance + 0x138); // AActor::RootComponent

			player.health = memio->ReadData<float>(Entity + 0x374); //AActor::Uobject::ASQSolider::Health 0x1c1				
		}
		// adds info to the vector
		tmp.push_back(player);
	}


	entities = tmp;
}



//FVector EFTData::GetPosition(uint64_t transform)
//{
//	auto transform_internal = memio->ReadData<uint64_t>(transform + 0x10);
//	if (transform_internal == 0) {
//		return *new FVector;
//	}
//
//	auto matrices = memio->ReadData<uint64_t>(transform_internal + 0x38);
//
//	auto index = memio->ReadData<int>(transform_internal + 0x40);
//	if (matrices == 0) {
//		return *new FVector;
//	}
//
//	memio->Read_Address(globals.process_id,(uintptr_t)(matrices + 0x18), &matrix_list_base, sizeof(matrix_list_base), false);
//	if (matrix_list_base == 0) {
//		return *new FVector;
//	}
//
//	memio->Read_Address(globals.process_id,(uintptr_t)(matrices + 0x20), &dependency_index_table_base, sizeof(dependency_index_table_base), false);
//	if (dependency_index_table_base == 0) {
//		return *new FVector;
//	}
//	
//	static auto get_dependency_index = [this](uint64_t base, int32_t index) 
//	{
//		memio->Read_Address(globals.process_id,(uintptr_t)(base + index * 4), &index, sizeof(index), false);
//		return index;
//	};
//
//	static auto get_matrix_blob = [this](uint64_t base, uint64_t offs, float* blob, uint32_t size) {
//		memio->Read_Address(globals.process_id,(uintptr_t)(base + offs), blob, size, false);
//	};
//
//	int32_t index_relation = get_dependency_index(dependency_index_table_base, index);
//
//	FVector ret_value;
//	{
//		float* base_matrix3x4 = (float*)malloc(64),
//			* matrix3x4_buffer0 = (float*)((uint64_t)base_matrix3x4 + 16),
//			* matrix3x4_buffer1 = (float*)((uint64_t)base_matrix3x4 + 32),
//			* matrix3x4_buffer2 = (float*)((uint64_t)base_matrix3x4 + 48);
//
//		get_matrix_blob(matrix_list_base, index * 48, base_matrix3x4, 16);
//
//		__m128 xmmword_1410D1340 = { -2.f, 2.f, -2.f, 0.f };
//		__m128 xmmword_1410D1350 = { 2.f, -2.f, -2.f, 0.f };
//		__m128 xmmword_1410D1360 = { -2.f, -2.f, 2.f, 0.f };
//
//		while (index_relation >= 0) 
//		{
//			uint32_t matrix_relation_index = 6 * index_relation;
//
//			// paziuret kur tik 3 nureadina, ten translationas, kur 4 = quatas ir yra rotationas.
//			get_matrix_blob(matrix_list_base, 8 * matrix_relation_index, matrix3x4_buffer2, 16);
//			__m128 v_0 = *(__m128*)matrix3x4_buffer2;
//
//			get_matrix_blob(matrix_list_base, 8 * matrix_relation_index + 32, matrix3x4_buffer0, 16);
//			__m128 v_1 = *(__m128*)matrix3x4_buffer0;
//
//			get_matrix_blob(matrix_list_base, 8 * matrix_relation_index + 16, matrix3x4_buffer1, 16);
//			__m128i v9 = *(__m128i*)matrix3x4_buffer1;
//
//			__m128* v3 = (__m128*)base_matrix3x4; // r10@1
//			__m128 v10; // xmm9@2
//			__m128 v11; // xmm3@2
//			__m128 v12; // xmm8@2
//			__m128 v13; // xmm4@2
//			__m128 v14; // xmm2@2
//			__m128 v15; // xmm5@2
//			__m128 v16; // xmm6@2
//			__m128 v17; // xmm7@2
//
//			v10 = _mm_mul_ps(v_1, *v3);
//			v11 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, 0));
//			v12 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, 85));
//			v13 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, -114));
//			v14 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, -37));
//			v15 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, -86));
//			v16 = _mm_castsi128_ps(_mm_shuffle_epi32(v9, 113));
//
//			v17 = _mm_add_ps(
//				_mm_add_ps(
//					_mm_add_ps(
//						_mm_mul_ps(
//							_mm_sub_ps(
//								_mm_mul_ps(_mm_mul_ps(v11, (__m128)xmmword_1410D1350), v13),
//								_mm_mul_ps(_mm_mul_ps(v12, (__m128)xmmword_1410D1360), v14)),
//							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), -86))),
//						_mm_mul_ps(
//							_mm_sub_ps(
//								_mm_mul_ps(_mm_mul_ps(v15, (__m128)xmmword_1410D1360), v14),
//								_mm_mul_ps(_mm_mul_ps(v11, (__m128)xmmword_1410D1340), v16)),
//							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 85)))),
//					_mm_add_ps(
//						_mm_mul_ps(
//							_mm_sub_ps(
//								_mm_mul_ps(_mm_mul_ps(v12, (__m128)xmmword_1410D1340), v16),
//								_mm_mul_ps(_mm_mul_ps(v15, (__m128)xmmword_1410D1350), v13)),
//							_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(v10), 0))),
//						v10)),
//				v_0);
//
//			*v3 = v17;
//
//			index_relation = get_dependency_index(dependency_index_table_base, index_relation);
//		}
//
//		ret_value = *(FVector*)base_matrix3x4;
//		delete[] base_matrix3x4;
//	}
//
//	return ret_value;
//}

struct TransformAccessReadOnly
{
	uint64_t	pTransformData;
	int			index;
};

struct TransformData
{
	uint64_t pTransformArray;
	uint64_t pTransformIndices;
};

struct Matrix34
{
	__m128 vec0;
	__m128 vec1;
	__m128 vec2;
};

int ToMeters(float x)
{
	return (int)(x / 101.266f);
}

bool EFTData::Read()
{

	if (this->offsets.localGameWorld == 0) {
		return false;
	}
	// Accumulate players.
	int pl = 0;

	Game::UPlayer player;


	for (int i = 0; i < entities.size(); i++)
	{
		auto Entities = entities[i];
		if (Entities.instance == Game::GetLocalPlayer())
			continue;
		Vector3 worldPosition = Game::GetCoordinate(Entities.root_component);
		Vector3 screenPosition;


		Game::WorldToScreenX(worldPosition, Game::cameraCache.POV, screenPosition);

		//if (screenPosition.z < 1.0f) continue;
		player.dist = Game::cameraCache.POV.Location.Distance(worldPosition);
		player.distR = ToMeters(player.dist);
		player.healthR = Entities.health;
		//if (screenPosition.z >= 1.0f)
		{

			// Player
			if (player.distR <= 1000 && Entities.health > 0 && Entities.team != Game::LocalTeamId.TeamId)
			{
				//DrawPlayerBar(screenPosition.x, screenPosition.y + 25, &Col.glassblack, &Col.red, ("[" + std::to_string(distR) + "m]" "SOL" "[" + std::to_string(Entities.health) + "]").c_str());
				
				String((int)screenPosition.x, (int)screenPosition.y, D3DCOLOR_ARGB(255, 199, 45, 199), pFont, true, _xor_("%0.2fm").operator const char* (), std::to_string(player.distR));
				pl++;
				if (pl == 200) {
					break;
				}

			}




		}


	}
}


// Get view matrix.
/*bool EFTData::GetMatrix(int& counter) {
	
		if ((counter >= 500 || this->offsets.fpsCamera == 0) && this->offsets.localGameWorld !=0 ) {
			counter = 0;
			auto tagged_objects = memio->ReadData<std::array<uint64_t, 2>>(globals.GOM + offsetof(EFTStructs::GameObjectManager, lastMainCamerataggedObjects));
			if (!tagged_objects[0] || !tagged_objects[1])
				return false;

			if (!(this->offsets.fpsCamera = GetObjectFromList(tagged_objects[1] , tagged_objects[0],  _xor_("FPS Camera"))))
				return false;			
		}
		int64_t temp = this->offsets.fpsCamera;
		if (!(temp = memio->ReadData<uint64_t>(temp + 0x30)) || !(temp = memio->ReadData<uint64_t>(temp + 0x18)))
			return false;

		//	printf("temp: 0x%X\n", temp);
		D3DXMATRIX temp_matrix;
		memio->Read_Address(globals.process_id, temp + 0xDC, &temp_matrix, sizeof(temp_matrix), false);
		D3DXMatrixTranspose(&this->viewMatrix, &temp_matrix);
	
		
}*/






