// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WorldManager.h"
#include "Chunk.generated.h"


UCLASS()
class SWORDWORLD_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnActors();

	void CalculateMapFromScratch();

	void SortBlockMap();

	void BuildMap(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals, TArray<FVector2D>& uvs, TArray<FColor>& vertexColors, TArray<FProcMeshTangent>& tangents, bool bTransparent = false);

	void BuildPlants(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals, TArray<FVector2D>& uvs, TArray<FColor>& vertexColors, TArray<FProcMeshTangent>& tangents);

	void CreateVisualMesh();

	TArray<Block*> GenerateRandomPlant(FVector index, bool alsoTree = false);

	FVector LocalIndexToWorld(FVector localIndex);

	FVector WorldIndexToLocal(FVector worldIndex);

	FVector IndexToWorldPos(FVector index);

	FVector WorldPosToIndex(FVector worldPos);

	bool IsEmpty(int x, int y, int z);

	BlockType GetTheoreticalBlockType(FVector worldPos);

	BlockType GetTheoreticalBlockType(int x, int y, int z);

	TArray<AChunk*> GetBoundChunks();

	class Block* GetBlock(FVector worldPos);

	class Block* GetBlock(int x, int y, int z);

	void DestoryBlock(class Block* block);

	void GenerateBlock(class Block* block);

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UProceduralMeshComponent* MeshComponent;

	TMap<FVector, class Block*> BlockMap;

private:
	bool IsNoiseGenerated = false;

	float PlantSize = 0;
};
