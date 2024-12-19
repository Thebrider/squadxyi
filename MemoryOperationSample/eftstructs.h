#pragma once
#include <string>
#include "math.hpp"
#include <vector>
#include <list>

#include "visuals.h"
#include "globals.h"
#include "PMemHelper.h"
#include <d3dx9math.h>
#pragma comment (lib, "d3dx9.lib")
#define OFF_WORLD				0x735b820

#define M_PI 3.14159265358979323846264338327950288419716939937510
// This is retarded but fuck it, deal with it, all objects should have their full struct
// in the namespace EFTStructs.
struct EFTOffsets
{
	static constexpr int64_t offs_gameObjectManager = 0x17FFD28;

	uint64_t gameObjectManager = 0x0;
	uint64_t gameWorld = 0x0;
	uint64_t localGameWorld = 0x0;
	uint64_t fpsCamera = 0x0;
	uint64_t OpticCamera = 0x0;

	struct
	{
		static constexpr int64_t itemList = 0x80; //UPDATED 1/11/2020
		static constexpr int64_t registeredPlayers = 0xA0; //UPDATED 1/11/2020
		static constexpr int64_t m_LocalPlayerID = 0x30;
	} localGameWorld_offsets;

	struct
	{
		static constexpr int64_t length = 0x10; //good
		static constexpr int64_t stringBase = 0x14; //good
	} unicodeString;

	struct
	{
		static constexpr int64_t information = 0x28; //updated
		static constexpr int64_t id = 0x60; //updated
	} profile;

	struct
	{
		static constexpr int64_t playerName = 0x10; //updated
	} information;

	struct
	{
		static constexpr int64_t angles_0 = 0x1D0;
		static constexpr int64_t angles_1 = 0x1D8;
		static constexpr int64_t position = 0x2F0;
	} movementContext;

	struct
	{
		static constexpr int64_t Name = 0x10; // [50] Name : String
		static constexpr int64_t ShortName = 0x18; // [50] Name : String
		static constexpr int64_t ID = 0x58; // [50] Name : String
		static constexpr int64_t QuestData = 0x9C; // [50] Name : String
		static constexpr int64_t Rarity = 0xCC; // [50] Name : String
		static constexpr int64_t Type = 0x108; // [50] Name : String
		static constexpr int64_t Grids = 0x110; // [50] Name : String

		
	} item;

	struct
	{
		static constexpr int64_t unkPattrn = 0x10; // [50] Name : String		
	} oject;
	struct
	{
		static constexpr int64_t lootbaseobject = 0x10; // [50] Name : String
		static constexpr int64_t lootItembase = 0xB0; // [50] Name : String
		static constexpr int64_t ConteinerItemOvner = 0x110; // [50] Name : String
	} lootiteractiveclass;
	struct
	{
		static constexpr int64_t LootItemBase = 0xC0; // [50] Name : String		
	} conteineritemovner;
	struct
	{
		static constexpr int64_t LootIteractiveclass = 0x28; // [50] Name : String
	} unkPattrn;
	struct
	{
		std::vector<int64_t> GameLootObjectName{ 0x30, 0x60 };
		std::vector<int64_t> Position{ 0x30, 0x30, 0x8, 0x38};
		std::vector<int64_t> PositionFromGameObject{ 0x30, 0x30, 0x8, 0x28};
	} lootbaseobject;
	struct
	{
		static constexpr int64_t Item = 0x40; // [50] Name : String
		static constexpr int64_t ItemsInCase = 0x70; // [50] Name : String
	} lootitembase;


	struct
	{
		static constexpr int64_t m_pPlayerProfile = 0x520;
		static constexpr int64_t movementContext = 0x40;
		static constexpr int64_t proceduralWeaponAnimation = 0x1A0; //updayed 1/11/2020 //pwa
		static constexpr int64_t playerBody = 0xA8; //updated 1/11/2020
		static constexpr int64_t m_pHealthController = 0x558; //updated 1/11/2020
		static constexpr int64_t profile = 0x520; //updayed 1/11/2020

	} Player;
};

namespace EFTStructs
{
	struct BaseObject
	{
		uint64_t previousObjectLink; //-0x0008
		uint64_t nextObjectLink; //0x0008
		uint64_t object; //0x0010
	};

	struct GameObjectManager
	{
		uint64_t lastTaggedObject; //0x0000
		uint64_t taggedObjects; //0x0008
		uint64_t lastMainCamerataggedObjects; //0x0010
		uint64_t firstMainCamerataggedObjects; //0x0018
		uint64_t lastActiveObject; //0x0020
		uint64_t activeObjects; //0x0028
	}; //Size: 0x0020

	class ListInternal
	{
	public:
		char pad_0x0000[0x20]; //0x0000
		intptr_t* firstEntry; //0x0020 
	}; //Size=0x0028

	class List
	{
	public:
		char pad_0x0000[0x10]; //0x0000
		ListInternal* listBase; //0x0010 
		__int32 itemCount; //0x0018 
	}; //Size=0x001C
}

struct EFTPlayer
{
	intptr_t	 instance;
	FVector		 headPos;
	std::		 string name;
	FVector		 location;
	bool		 IsPlayer;
	bool		 IsLocal;
	int			 DateReg;
	int			 countP = 0;
	int			 countS = 0;
	int			 countPS = 0;
	int			 WildSpawnType;
};

struct EFTExtract
{
	intptr_t	 instance;
	std::string  name;
	FVector		 location;
};


struct EFTLoot
{
	intptr_t	instance;
	std::string object;
	std::string id;
	std::string name;
	std::string ShortName;
	bool Find = false;
	bool QuestI = false;
	int Rarity = 0;
	int Type = 0;
	std::vector<EFTLoot> ItemInCase;
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 213, 0);
	FVector	origin;


};


enum BodyParts : uint64_t
{
	Head = 0x20,
	Thorax = 0x28,
	Stomach = 0x30,
	LeftArm = 0x38,
	RightArm = 0x40,
	LeftLeg = 0x48,
	RightLeg = 0x50,
	value_max
};

class UPlayer {
private:
public:
	uintptr_t instance;
	uintptr_t mesh;
	uintptr_t root_component;
	uintptr_t instigator;
	uintptr_t PlayerState;
	std::string name;
	int objectId;
	Vector3 origin;
	float health;
	float dist;
	int team;
};

class EFTData
{
public:
	static EFTData* Instance();

	bool InitOffsets();
	bool Getitem();
	bool GetMatrix(int &counter);
	//FVector GetPosition(uint64_t pMatrix, uint32_t index);
	FVector GetPosition(uint64_t transform);
	bool Read();
	void ClothingChams(uint64_t PlayerBody);
	void NullMaterials(uint64_t materials);
	D3DXMATRIX viewMatrix;
	EFTPlayer localPlayer;
	std::vector<EFTPlayer> players;
	std::vector<EFTExtract> extracts;
	std::vector<EFTLoot> loot;
	int          playercount;
	int          scavcount;
	int          pscavcount;
	EFTLoot GetItems(uint64_t adress, bool incase, int counter);
	bool IsAiming(uint64_t instance);

	uint64_t get_mpcamera(uint64_t instance);
	D3DXMATRIX getoptic_matrix(uint64_t instance);
	bool open_extract(uint64_t extract);


	EFTOffsets offsets;

	int32_t DataReg(uint64_t instance);
	bool IsPlayer(uint64_t instance);
private:
	uint64_t matrix_list_base = 0;
	uint64_t dependency_index_table_base = 0;

	uint64_t GetObjectFromList(uint64_t listPtr, uint64_t lastObjectPtr, const char* objectName);
	uint64_t getbone_matrix(uint64_t instance);





	//std::vector<std::wstring> names;
};

enum Bones : int
{
	HumanBase = 0,
	HumanPelvis = 14,
	HumanLThigh1 = 15,
	HumanLThigh2 = 16,
	HumanLCalf = 17,
	HumanLFoot = 18,
	HumanLToe = 19,
	HumanRThigh1 = 20,
	HumanRThigh2 = 21,
	HumanRCalf = 22,
	HumanRFoot = 23,
	HumanRToe = 24,
	HumanSpine1 = 29,
	HumanSpine2 = 36,
	HumanSpine3 = 37,
	HumanLCollarbone = 89,
	HumanLUpperarm = 90,
	HumanLForearm1 = 91,
	HumanLForearm2 = 92,
	HumanLForearm3 = 93,
	HumanLPalm = 94,
	HumanRCollarbone = 110,
	HumanRUpperarm = 111,
	HumanRForearm1 = 112,
	HumanRForearm2 = 113,
	HumanRForearm3 = 114,
	HumanRPalm = 115,
	HumanNeck = 132,
	HumanHead = 133
};
namespace Game
{
	static std::string GetNameById(uint32_t actor_id) {
		char name[256];

		uint32_t chunk_offset = actor_id >> 16;
		uint16_t name_offset = (uint16_t)actor_id;
		uintptr_t fname_pool = globals.Base + 0x71daac0; //ÐÎÒ ÐÀÇÐÀÁÀ ÌÀÍÀË ÂÑÞ ÅÃÎ ÑÅÌÜÞ ÅÁÀÍÍÛÉ ÎÔÑÅÒ ÁËßÒÜ  OFFSET_GNAMES

		uintptr_t name_pool_chunk = memio->ReadData<uintptr_t>(fname_pool + ((chunk_offset + 2) * 8));
		if (name_pool_chunk) {
			uintptr_t entry_offset = name_pool_chunk + (uint32_t)(2 * name_offset);
			if (entry_offset) {

				uint16_t name_entry = memio->ReadData<uint16_t>(entry_offset);

				uint32_t name_length = (name_entry >> 6);

				if (name_length > 256)
				{
					name_length = 255;
				}

				auto name = memio->ReadString(entry_offset + 0x2, name_length);
				return name;


			}
		}
		return std::string("NULL");
	}

	struct FMinimalViewInfo
	{
		Vector3 Location;
		Vector3 Rotation;
		float FOV;
	};
	struct FCameraCacheEntry
	{
		char pad_0x0[0x10];
		FMinimalViewInfo POV;
	};

	class APlayerState
	{
	private:
	public:
		uintptr_t SoldiersOnScreenSortedByNearestDistanceCache;// 0x0488
		int TeamId;// 0x0420 
	};

	class UPlayer {
	private:
	public:
		uintptr_t instance;
		uintptr_t mesh;
		uintptr_t root_component;
		uintptr_t instigator;
		uintptr_t PlayerState;
		std::string name;
		int objectId;
		Vector3 origin;
		float health;
		float healthR;
		float dist;
		float distR;
		int team;
	};



	FCameraCacheEntry cameraCache;
	uintptr_t uWorld;
	uintptr_t gameInstance;
	uintptr_t persistentLevel;
	uintptr_t localPlayerPtr;
	uintptr_t localPlayer;
	uintptr_t playerController;
	uintptr_t pawn;
	uintptr_t cameraManager;
	uintptr_t actorsArray;
	uintptr_t PlayerStateLocalPlayer;
	uintptr_t PlayerState;
	APlayerState LocalTeamId;
	int actorsCount;



	struct matrix4x4
	{
		float m[12];
	};

	uint64_t GetLocalPlayer()
	{
		// Sorted Entity Object
		return memio->ReadData<uintptr_t>(localPlayerPtr);  // LocalPlayer lol /
	}

	uint64_t GetLocalPlayerVisualState()
	{
		// Future Visual State
		return memio->ReadData<uint64_t>(GetLocalPlayer() + 0x198);
	}

	Vector3 GetCoordinate(uint64_t root_component)
	{
		while (true)
		{


			return  memio->ReadData<Vector3>(root_component + 0x11c); // USceneComponent::RelativeLocation

		}
	}
	float GetDistanceToMe(Vector3 Entity)
	{
		return Entity.Distance(GetCoordinate(GetLocalPlayer()));
	}

	D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
	{
		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

	bool WorldToScreenX(Vector3 WorldLocation, FMinimalViewInfo CameraCacheL, Vector3& Screenlocation)
	{

		auto POV = CameraCacheL;
		Vector3 Rotation = POV.Rotation;
		D3DMATRIX tempMatrix = Matrix(Rotation);

		Vector3 vAxisX, vAxisY, vAxisZ;

		vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		Vector3 vDelta = WorldLocation - POV.Location;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f)
			return false;

		float FovAngle = POV.FOV;

		Screenlocation.x = (globals.Width / 2) + vTransformed.x * ((globals.Width / 2) / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.y = (globals.Height / 2) - vTransformed.y * ((globals.Width / 2) / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;



	}



	uint64_t GetWorld() {
		return memio->ReadData<uint64_t>(
			globals.Base + OFF_WORLD);
	}
}
