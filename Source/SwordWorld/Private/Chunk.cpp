// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"
#include "Block.h"
#include "Engine/World.h"
#include "Plant.h"
#include "TimerManager.h"


// Sets default values
AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->bUseAsyncCooking = true;
	SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();

	PlantSize = AWorldManager::Inst->BlockSize * 0.3;

	CalculateMapFromScratch();

	SpawnActors();

	CreateVisualMesh();
	IsNoiseGenerated = true;
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::SpawnActors()
{
	AWorldManager::Inst->SpawnEnemiesFromChunk(GetActorLocation());
}

void AChunk::CalculateMapFromScratch()
{
	BlockMap = TMap<FVector, Block*>();

	for(int x = 0; x < AWorldManager::Inst->ChunkWidth; x++)
	{
		for(int y = 0; y < AWorldManager::Inst->ChunkWidth; y++)
		{
			int topDirtZ = -1;
			for(int z = 0; z < AWorldManager::Inst->ChunkHeight; z++)
			{
				BlockType blockType = (BlockType)AWorldManager::Inst->GetTheoreticalByte(FVector(x, y, z) + GetActorLocation() / AWorldManager::Inst->BlockSize);
				Block* block = new Block(this, FVector(x, y, z), blockType);

				if(blockType != BlockType::empty)
				{
					BlockMap.Add(block->index, block);

					if(topDirtZ >= 0 && z - topDirtZ > 1)
					{
						Block* topDirtBlock = GetBlock(x, y, topDirtZ);
						if(topDirtBlock && topDirtBlock->blockType != BlockType::grass)
						{
							topDirtBlock->blockType = BlockType::grass;
							TArray<Block*> blocks = GenerateRandomPlant(FVector(x, y, topDirtZ + 1), z - topDirtZ >= 8);
							for(TArray<Block*>::TIterator iter = blocks.CreateIterator(); iter; ++iter)
							{
								Block* block = *iter;
								if(block->blockType != BlockType::empty && !BlockMap.Contains(block->index))
									BlockMap.Add(block->index, block);
							}
						}
					}

					if(blockType == BlockType::dirt)
						topDirtZ = FMath::Max<int>(topDirtZ, z);
					else if(topDirtZ >= 0)
						topDirtZ = -1;
				}

				if(z == AWorldManager::Inst->ChunkHeight - 1)
				{
					if(topDirtZ >= 0 && z >= topDirtZ)
					{
						Block* topDirtBlock = GetBlock(x, y, topDirtZ);
						if(topDirtBlock && topDirtBlock->blockType != BlockType::grass)
						{
							topDirtBlock->blockType = BlockType::grass;
							TArray<Block*> blocks = GenerateRandomPlant(FVector(x, y, topDirtZ + 1), true);
							for(TArray<Block*>::TIterator iter = blocks.CreateIterator(); iter; ++iter)
							{
								Block* block = *iter;
								if(block->blockType != BlockType::empty)
								{
									if(!BlockMap.Contains(block->index))
										BlockMap.Add(block->index, block);
									else if(block->blockType == BlockType::log_oak || block->blockType == BlockType::log_birch)
										BlockMap[block->index] = block;
								}
							}
						}
					}
				}
			}
		}
	}

	for(TMap<FVector, Block*>::TConstIterator iter = BlockMap.CreateConstIterator(); iter; ++iter)
	{
		FVector index = iter->Key;
		Block* block = iter->Value;
		AWorldManager::Inst->Blocks.Add(LocalIndexToWorld(index), block);
	}
}

void AChunk::SortBlockMap()
{
	BlockMap.KeySort([](FVector A, FVector B){
		if(A.X > B.X)
			return false;
		else if(A.X == B.X)
		{
			if(A.Y > B.Y)
				return false;
			else if(A.Y == B.Y)
			{
				if(A.Z > B.Z)
					return false;
			}
			return true;
		}
		return true;
	});
}

void AChunk::BuildMap(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals, TArray<FVector2D>& uvs, TArray<FColor>& vertexColors, TArray<FProcMeshTangent>& tangents, bool bTransparent)
{
	for(TMap<FVector, Block*>::TConstIterator iter = BlockMap.CreateConstIterator(); iter; ++iter)
	{
		FVector index = iter->Key;
		Block* block = iter->Value;

		if(!bTransparent)
		{
			if(block->blockType == BlockType::empty || block->blockType >= BlockType::glass)
				continue;
		}
		else
		{
			if(block->blockType < BlockType::glass || block->blockType >= BlockType::tall_grass)
				continue;
		}

		if(IsNoiseGenerated)
			block->ClearMeshs();

		// front
		if(IsEmpty(index.X - 1, index.Y, index.Z))
			block->BuildFace(FVector(0, 0, 0), FVector::UpVector, FVector::RightVector, -FVector::ForwardVector, false);
		// right
		if(IsEmpty(index.X, index.Y + 1, index.Z))
			block->BuildFace(FVector(0, 1, 0), FVector::UpVector, FVector::ForwardVector, FVector::RightVector, false);
		// back
		if(IsEmpty(index.X + 1, index.Y, index.Z))
			block->BuildFace(FVector(1, 1, 0), FVector::UpVector, -FVector::RightVector, FVector::ForwardVector, false);
		// left
		if(IsEmpty(index.X, index.Y - 1, index.Z))
			block->BuildFace(FVector(1, 0, 0), FVector::UpVector, -FVector::ForwardVector, -FVector::RightVector, false);
		// bottom
		if(IsEmpty(index.X, index.Y, index.Z - 1))
			block->BuildFace(FVector(1, 0, 0), -FVector::ForwardVector, FVector::RightVector, -FVector::UpVector, false);
		// top
		if(IsEmpty(index.X, index.Y, index.Z + 1))
			block->BuildFace(FVector(1, 0, 1), FVector::RightVector, -FVector::ForwardVector, FVector::UpVector, false);

		int verNum = vertices.Num();
		vertices.Append(block->vertices);
		for(int i = 0; i < block->triangles.Num(); i++)
			triangles.Add(block->triangles[i] + verNum);
		normals.Append(block->normals);
		uvs.Append(block->uvs);
		vertexColors.Append(block->vertexColors);
		tangents.Append(block->tangents);
	}
}

void AChunk::BuildPlants(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals, TArray<FVector2D>& uvs, TArray<FColor>& vertexColors, TArray<FProcMeshTangent>& tangents)
{
	for(TMap<FVector, Block*>::TConstIterator iter = BlockMap.CreateConstIterator(); iter; ++iter)
	{
		FVector index = iter->Key;
		Block* block = iter->Value;

		if(block->blockType < BlockType::tall_grass)
			continue;

		if(IsNoiseGenerated)
			block->ClearMeshs();

		block->BuildFace(FVector(-PlantSize, -PlantSize, 0), FVector(-PlantSize, -PlantSize, PlantSize * 2), FVector(PlantSize, PlantSize, PlantSize * 2), FVector(PlantSize, PlantSize, 0), FVector(AWorldManager::Inst->BlockSize * 0.5f, AWorldManager::Inst->BlockSize * 0.5f, 0), FVector(-1, 1, 0), false);
		block->BuildFace(FVector(PlantSize, -PlantSize, 0), FVector(PlantSize, -PlantSize, PlantSize * 2), FVector(-PlantSize, PlantSize, PlantSize * 2), FVector(-PlantSize, PlantSize, 0), FVector(AWorldManager::Inst->BlockSize * 0.5f, AWorldManager::Inst->BlockSize * 0.5f, 0), FVector(1, 1, 0), false);

		int verNum = vertices.Num();
		vertices.Append(block->vertices);
		for(int i = 0; i < block->triangles.Num(); i++)
			triangles.Add(block->triangles[i] + verNum);
		normals.Append(block->normals);
		uvs.Append(block->uvs);
		vertexColors.Append(block->vertexColors);
		tangents.Append(block->tangents);
	}
}

void AChunk::CreateVisualMesh()
{
	//普通方块
	TArray<FVector> vertices = TArray<FVector>();
	TArray<int32> triangles = TArray<int32>();
	TArray<FVector> normals = TArray<FVector>();
	TArray<FVector2D> uvs = TArray<FVector2D>();
	TArray<FColor> vertexColors = TArray<FColor>();
	TArray<FProcMeshTangent> tangents = TArray<FProcMeshTangent>();

	BuildMap(vertices, triangles, normals, uvs, vertexColors, tangents);
	MeshComponent->CreateMeshSection(0, vertices, triangles, normals, uvs, vertexColors, tangents, true);
	MeshComponent->SetMaterial(0, AWorldManager::Inst->ChunkMaterial);

	//植物
	vertices = TArray<FVector>();
	triangles = TArray<int32>();
	normals = TArray<FVector>();
	uvs = TArray<FVector2D>();
	vertexColors = TArray<FColor>();
	tangents = TArray<FProcMeshTangent>();
	
	BuildPlants(vertices, triangles, normals, uvs, vertexColors, tangents);
	if(vertices.Num() > 0)
	{
		MeshComponent->CreateMeshSection(1, vertices, triangles, normals, uvs, vertexColors, tangents, false);
		MeshComponent->SetMaterial(1, AWorldManager::Inst->PlantsMaterial);
	}

	//透明方块
	vertices = TArray<FVector>();
	triangles = TArray<int32>();
	normals = TArray<FVector>();
	uvs = TArray<FVector2D>();
	vertexColors = TArray<FColor>();
	tangents = TArray<FProcMeshTangent>();

	BuildMap(vertices, triangles, normals, uvs, vertexColors, tangents, true);
	if(vertices.Num() > 0)
	{
		MeshComponent->CreateMeshSection(2, vertices, triangles, normals, uvs, vertexColors, tangents, true);
		MeshComponent->SetMaterial(2, AWorldManager::Inst->ChunkMaterial);
	}
}

TArray<Block*> AChunk::GenerateRandomPlant(FVector index, bool alsoTree)
{
	TArray<Block*> blocks = TArray<Block*>();
	
	float tmpNum = FMath::FRand();
	
	if(tmpNum < 0.2f)
	{
		Block* block = new Block(this, index, FMath::FRand() < 0.8f ? BlockType::tall_grass : (BlockType)FMath::RandRange((int)BlockType::flower_allium, (int)BlockType::flower_tulip_white));
		blocks.Add(block);
		return blocks;
	}
	
	if(alsoTree && tmpNum < 0.21f)
	{
		int trunkHeight = FMath::RandRange(4, 6);
		Block* tmpBlock = nullptr;
		BlockType trunkType = FMath::FRand() < 0.7f ? BlockType::log_oak : BlockType::log_birch;
		for(int i = 0; i < trunkHeight; i++)
		{
			tmpBlock = new Block(this, FVector(index.X, index.Y, index.Z + i), trunkType);
			blocks.Add(tmpBlock);
		}
		int leavesWidth = FMath::RandRange(4, 6);
		int leavesHeight = FMath::RandRange(3, 4);
		for(int x = index.X - leavesWidth / 2; x <= index.X + leavesWidth / 2; x++)
		{
			for(int y = index.Y - leavesWidth / 2; y <= index.Y + leavesWidth / 2; y++)
			{
				for(int z = index.Z + trunkHeight - 1; z < index.Z + trunkHeight + leavesHeight - 1; z++)
				{
					BlockType tmpBlockType = GetTheoreticalBlockType(x, y, z);
					if(tmpBlockType == BlockType::empty || (tmpBlockType >= BlockType::tall_grass && tmpBlockType <= BlockType::flower_tulip_white))
					{
						if((x == index.X - leavesWidth / 2 || x == index.X + leavesWidth / 2 || y == index.Y - leavesWidth / 2 || y == index.Y + leavesWidth / 2) && (z == index.Z + trunkHeight - 1 || z == index.Z + trunkHeight + leavesHeight - 2) && FMath::FRand() < 0.8f)
							continue;
						tmpBlock = new Block(this, FVector(x, y, z), BlockType::leaves);
						blocks.Add(tmpBlock);
					}
				}
			}
		}
		return blocks;
	}
	
	return blocks;
}

FVector AChunk::LocalIndexToWorld(FVector localIndex)
{
	return localIndex + GetActorLocation() / AWorldManager::Inst->BlockSize;
}

FVector AChunk::WorldIndexToLocal(FVector worldIndex)
{
	return worldIndex - GetActorLocation() / AWorldManager::Inst->BlockSize;
}

FVector AChunk::IndexToWorldPos(FVector index)
{
	return index * AWorldManager::Inst->BlockSize + GetActorLocation();
}

FVector AChunk::WorldPosToIndex(FVector worldPos)
{
	worldPos = (worldPos - GetActorLocation()) / AWorldManager::Inst->BlockSize;
	int x = FMath::FloorToInt(worldPos.X);
	int y = FMath::FloorToInt(worldPos.Y);
	int z = FMath::FloorToInt(worldPos.Z);
	return FVector(x, y, z);
}

bool AChunk::IsEmpty(int x, int y, int z)
{
	if(z < 0) return false;

	if(!IsNoiseGenerated)
	{
		if(GetTheoreticalBlockType(x, y, z) != BlockType::empty && GetTheoreticalBlockType(x, y, z) < BlockType::leaves)
			return false;
		return true;
	}
	else
	{
		Block* block = GetBlock(x, y, z);
		if(!block)
		{
			FVector worldPos = IndexToWorldPos(FVector(x, y, z));
			block = AWorldManager::Inst->FindBlock(worldPos);
		}
		if(block && block->blockType != BlockType::empty && GetTheoreticalBlockType(x, y, z) < BlockType::leaves)
			return false;
		return true;
	}
}

BlockType AChunk::GetTheoreticalBlockType(FVector worldPos)
{
	FVector index = WorldPosToIndex(worldPos);
	return GetTheoreticalBlockType(index.X, index.Y, index.Z);
}

BlockType AChunk::GetTheoreticalBlockType(int x, int y, int z)
{
	if(z < 0 || z >= AWorldManager::Inst->ChunkHeight)
		return BlockType::empty;

	if(x < 0 || y < 0 || x >= AWorldManager::Inst->ChunkWidth || y >= AWorldManager::Inst->ChunkWidth)
	{
		FVector worldPos = IndexToWorldPos(FVector(x, y, z));
		AChunk* chunk = AWorldManager::Inst->FindChunk(worldPos);
		if(chunk)
			return chunk->GetTheoreticalBlockType(worldPos);
		return (BlockType)AWorldManager::Inst->GetTheoreticalByte(worldPos / AWorldManager::Inst->BlockSize);
	}

	if(BlockMap.Contains(FVector(x, y, z)))
		return BlockMap[FVector(x, y, z)]->blockType;

	return BlockType::empty;
}

TArray<AChunk*> AChunk::GetBoundChunks()
{
	TArray<AChunk*> boundChunks = TArray<AChunk*>();
	AChunk* tmpChunk;
	tmpChunk = AWorldManager::Inst->FindChunk(GetActorLocation() + FVector(AWorldManager::Inst->ChunkSize, 0, 0));
	if(tmpChunk)
		boundChunks.Add(tmpChunk);
	tmpChunk = AWorldManager::Inst->FindChunk(GetActorLocation() + FVector(-AWorldManager::Inst->ChunkSize, 0, 0));
	if(tmpChunk)
		boundChunks.Add(tmpChunk);
	tmpChunk = AWorldManager::Inst->FindChunk(GetActorLocation() + FVector(0, -AWorldManager::Inst->ChunkSize, 0));
	if(tmpChunk)
		boundChunks.Add(tmpChunk);
	tmpChunk = AWorldManager::Inst->FindChunk(GetActorLocation() + FVector(0, AWorldManager::Inst->ChunkSize, 0));
	if(tmpChunk)
		boundChunks.Add(tmpChunk);
	return boundChunks;
}
Block* AChunk::GetBlock(FVector worldPos)
{
	FVector index = WorldPosToIndex(worldPos);
	return GetBlock(index.X, index.Y, index.Z);
}

Block* AChunk::GetBlock(int x, int y, int z)
{
	if(BlockMap.Contains(FVector(x, y, z)))
		return BlockMap[FVector(x, y, z)];
	return nullptr;
}

void AChunk::DestoryBlock(class Block* block)
{
	BlockMap.Remove(block->index);
	AWorldManager::Inst->Blocks.Remove(LocalIndexToWorld(block->index));
	CreateVisualMesh();
	TArray<AChunk*> chunks = block->GetBoundChunks();
	for(int i = 0; i < chunks.Num(); i++)
		chunks[i]->CreateVisualMesh();
}

void AChunk::GenerateBlock(Block * block)
{
	BlockMap.Add(block->index, block);
	AWorldManager::Inst->Blocks.Add(LocalIndexToWorld(block->index), block);
	CreateVisualMesh();
	TArray<AChunk*> chunks = block->GetBoundChunks();
	for(int i = 0; i < chunks.Num(); i++)
		chunks[i]->CreateVisualMesh();
}
