// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldManager.h"
#include "SimplexNoise.h"
#include "Engine/World.h"
#include "Chunk.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacterBase.h"
#include "Block.h"
#include "TimerManager.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Engine/Texture2DDynamic.h"
#include "Kismet/KismetMaterialLibrary.h"

AWorldManager* AWorldManager::Inst = nullptr;

// Sets default values
AWorldManager::AWorldManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldManager::Initialize()
{
	Inst = this;
	Seed = FMath::Rand();
	Chunks = TArray<AChunk*>();
	Blocks = TMap<FVector, Block*>();
	UnGeneratedChunks = TArray<FVector>();

	ChunkSize = ChunkWidth * BlockSize;
	BlockUVWidth = FVector2D(0.0625f, 0.0625f);

	TMap<BlockFace, FVector2D> blockUVCorners = TMap<BlockFace, FVector2D>();

	blockUVCorners.Add(BlockFace::Default, FVector2D(0.375f, 0.9375f));
	BlockStats.Add(BlockType::dirt, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Side, FVector2D(0, 0.875f));
	blockUVCorners.Add(BlockFace::Top, FVector2D(0, 0.8125f));
	blockUVCorners.Add(BlockFace::Bottom, FVector2D(0, 0.9375f));
	BlockStats.Add(BlockType::grass, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Side, FVector2D(0.5f, 0.875f));
	blockUVCorners.Add(BlockFace::Top, FVector2D(0.5f, 0.8125f));
	blockUVCorners.Add(BlockFace::Bottom, FVector2D(0.5f, 0.9375f));
	BlockStats.Add(BlockType::grass_snowed, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.0625f, 0.9375f));
	BlockStats.Add(BlockType::sand, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.3125f, 0.9375f));
	BlockStats.Add(BlockType::stone, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Side, FVector2D(0.125f, 0.875f));
	blockUVCorners.Add(BlockFace::Top, FVector2D(0.125f, 0.8125f));
	blockUVCorners.Add(BlockFace::Bottom, FVector2D(0.125f, 0.9375f));
	BlockStats.Add(BlockType::stone_slab, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.625f, 0.9375f));
	BlockStats.Add(BlockType::stonebrick, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.75f, 0.9375f));
	BlockStats.Add(BlockType::wool_colored_white, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.1875f, 0.9375f));
	BlockStats.Add(BlockType::brick, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Side, FVector2D(0.6875f, 0.875f));
	blockUVCorners.Add(BlockFace::Top, FVector2D(0.6875f, 0.8125f));
	blockUVCorners.Add(BlockFace::Bottom, FVector2D(0.6875f, 0.9375f));
	BlockStats.Add(BlockType::log_birch, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.9375f, 0.9375f));
	BlockStats.Add(BlockType::snow, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.8125f, 0.9375f));
	BlockStats.Add(BlockType::crafting_table, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.5625f, 0.9375f));
	BlockStats.Add(BlockType::glass, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Side, FVector2D(0.25f, 0.875f));
	blockUVCorners.Add(BlockFace::Top, FVector2D(0.25f, 0.8125f));
	blockUVCorners.Add(BlockFace::Bottom, FVector2D(0.25f, 0.9375f));
	BlockStats.Add(BlockType::log_oak, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.875f, 0.9375f));
	BlockStats.Add(BlockType::leaves, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.4375f, 0.9375f));
	BlockStats.Add(BlockType::planks_oak, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0, 0.75f));
	BlockStats.Add(BlockType::tall_grass, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.0625f, 0.75f));
	BlockStats.Add(BlockType::flower_allium, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.125f, 0.75f));
	BlockStats.Add(BlockType::flower_blue_orchid, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.1875f, 0.75f));
	BlockStats.Add(BlockType::flower_dandelion, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.25f, 0.75f));
	BlockStats.Add(BlockType::flower_houstonia, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.1875f, 0.75f));
	BlockStats.Add(BlockType::flower_oxeye_daisy, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.3125f, 0.75f));
	BlockStats.Add(BlockType::flower_paeonia, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.375f, 0.75f));
	BlockStats.Add(BlockType::flower_pot, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.4375f, 0.75f));
	BlockStats.Add(BlockType::flower_rose, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.5f, 0.75f));
	BlockStats.Add(BlockType::flower_tulip_orange, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.5625f, 0.75f));
	BlockStats.Add(BlockType::flower_tulip_pink, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.625f, 0.75f));
	BlockStats.Add(BlockType::flower_tulip_red, FBlockStat(blockUVCorners));

	blockUVCorners = TMap<BlockFace, FVector2D>();
	blockUVCorners.Add(BlockFace::Default, FVector2D(0.6875f, 0.75f));
	BlockStats.Add(BlockType::flower_tulip_white, FBlockStat(blockUVCorners));

	for (int x = -FMath::FloorToInt(ChunkNumX * 0.5f); x < FMath::CeilToInt(ChunkNumX * 0.5f); x++)
	{
		for (int y = -FMath::FloorToInt(ChunkNumY * 0.5f); y < FMath::CeilToInt(ChunkNumY * 0.5f); y++)
		{
			UnGeneratedChunks.Add(FVector(x * ChunkSize, y * ChunkSize, 0));
		}
	}

	UnGeneratedChunks.Sort([](const FVector& A, const FVector& B) {
		FVector dirA, dirB;
		float lengthA, lengthB;
		A.ToDirectionAndLength(dirA, lengthA);
		B.ToDirectionAndLength(dirB, lengthB);
		if (lengthA == lengthB)
			return A.X < B.X;
		return lengthA < lengthB;
		});
}

// Called when the game starts or when spawned
void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	Initialize();
	
	for(int i = 0; i < InitialChunkNum; i++)
		GenerateChunks();

	GetWorldTimerManager().SetTimer(ChunksGenerator, this, &AWorldManager::GenerateChunks, ChunksGenerateInterval, true, ChunksGenerateInterval);
}

// Called every frame
void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

BYTE AWorldManager::GetTheoreticalByte(FVector Pos)
{
	FRandomStream randomStream = FRandomStream(Seed);

	FVector grain0Offset = FVector(randomStream.FRand() * 10000, randomStream.FRand() * 10000, randomStream.FRand() * 10000);
	FVector grain1Offset = FVector(randomStream.FRand() * 10000, randomStream.FRand() * 10000, randomStream.FRand() * 10000);
	FVector grain2Offset = FVector(randomStream.FRand() * 10000, randomStream.FRand() * 10000, randomStream.FRand() * 10000);

	return GetTheoreticalByte(Pos, grain0Offset, grain1Offset, grain2Offset);
}

BYTE AWorldManager::GetTheoreticalByte(FVector Pos, FVector Offset0, FVector Offset1, FVector Offset2)
{
	BYTE brick = (BYTE)BlockType::dirt;

	//山脉
	float mountainValue = CalculateNoiseValue(Pos, Offset2, 0.005f);
	//丛林
	float clusterValue = CalculateNoiseValue(Pos, Offset0, 0.01f);
	//大对象
	float blobValue = CalculateNoiseValue(Pos, Offset1, 0.04f);

	if(mountainValue == 0 && blobValue < 0.2f)
		brick = (BYTE)BlockType::dirt;
	else if(clusterValue > 0.9f)
		brick = (BYTE)BlockType::dirt;
	else if(clusterValue > 0.8f)
		brick = (BYTE)BlockType::stone;

	mountainValue = FMath::Sqrt(mountainValue);

	mountainValue *= ChunkHeight;
	mountainValue += clusterValue * 10;
	mountainValue += blobValue * 10;

	if(mountainValue >= Pos.Z)
		return brick;
	return 0;
}

float AWorldManager::CalculateNoiseValue(FVector Pos, FVector Offset, float Scale)
{
	float noiseX = FMath::Abs((Pos.X + Offset.X) * Scale);
	float noiseY = FMath::Abs((Pos.Y + Offset.Y) * Scale);
	float noiseZ = FMath::Abs((Pos.Z + Offset.Z) * Scale);

	return FMath::Max(0.0f, SimplexNoise::Generate(noiseX, noiseY, noiseZ));
}

AChunk* AWorldManager::FindChunk(FVector Pos, bool Accurately)
{
	if(!Accurately)
	{
		for(int i = 0; i < Chunks.Num(); i++)
		{
			FVector tmpPos = Chunks[i]->GetActorLocation();
			if(Pos.X >= tmpPos.X && Pos.X < tmpPos.X + ChunkSize && Pos.Y >= tmpPos.Y && Pos.Y < tmpPos.Y + ChunkSize)
				return Chunks[i];
		}
	}
	else
	{
		Block* block = FindBlock(Pos);
		if(block)
			return block->Owner;
	}
	return nullptr;
}

Block* AWorldManager::FindBlock(FVector Pos)
{
	Pos /= BlockSize;
	int x = FMath::FloorToInt(Pos.X);
	int y = FMath::FloorToInt(Pos.Y);
	int z = FMath::FloorToInt(Pos.Z);
	return FindBlock(x, y, z);
}

Block* AWorldManager::FindBlock(int x, int y, int z)
{
	if(Blocks.Contains(FVector(x, y, z)))
		return Blocks[FVector(x, y, z)];
	return nullptr;
}

FVector AWorldManager::GetSurfaceFloorPos(FVector2D Corner, float Height, bool IgnoreLeaves)
{
	int x = FMath::FloorToInt(Corner.X / BlockSize);
	int y = FMath::FloorToInt(Corner.Y / BlockSize);
	for(int z = ChunkHeight + 10; z >= 0; z--)
	{
		Block* block = FindBlock(x, y, z);
		if(block && block->blockType < BlockType::tall_grass)
		{
			if(IgnoreLeaves && block->blockType == BlockType::leaves)
				continue;
			if(block->blockType != BlockType::log_oak && block->blockType != BlockType::log_birch && !FindBlock(x, y, z + FMath::CeilToInt(Height / BlockSize) + 1))
				return block->GetWorldPos();
			else
				return GetSurfaceFloorPos(Corner, false);
		}
	}
	return FVector(Corner.X, Corner.Y, 0);
}

FVector AWorldManager::GetSurfaceFloorPos(FVector2D Corner, float RadiusX, float RadiusY, float HalfHeight)
{
	float maxPosZ = 0;

	FVector2D boundsMin = FVector2D(Corner.X - RadiusX, Corner.Y - RadiusY);
	boundsMin.X = FMath::FloorToInt(boundsMin.X / BlockSize) * BlockSize;
	boundsMin.Y = FMath::FloorToInt(boundsMin.Y / BlockSize) * BlockSize;
	
	FVector2D boundsMax = FVector2D(Corner.X + RadiusX, Corner.Y + RadiusY);
	boundsMax.X = FMath::CeilToInt(boundsMax.X / BlockSize) * BlockSize;
	boundsMax.Y = FMath::CeilToInt(boundsMax.Y / BlockSize) * BlockSize;

	for(float x = boundsMin.X; x < boundsMax.X; x += BlockSize)
	{
		for(float y = boundsMin.Y; y < boundsMax.Y; y += BlockSize)
		{
			maxPosZ = FMath::Max(GetSurfaceFloorPos(FVector2D(x, y), HalfHeight * 2).Z, maxPosZ);
		}
	}
	
	return FVector(Corner.X, Corner.Y, maxPosZ + BlockSize);
}

void AWorldManager::GenerateChunks()
{
	if(UnGeneratedChunks.Num() == 0)
	{
		IsGenerated = true;
		GetWorldTimerManager().ClearTimer(ChunksGenerator);
		return;
	}
	SpawnChunk(UnGeneratedChunks[0]);
	UnGeneratedChunks.RemoveAt(0);
}

void AWorldManager::SpawnChunk(FVector Pos)
{
	AChunk* chunk = GetWorld()->SpawnActor<AChunk>(Pos, FRotator::ZeroRotator);
	SpawnEnemiesFromChunk(Pos);
	Chunks.Add(chunk);
}
