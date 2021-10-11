// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Global.h"
#include "WorldManager.generated.h"

UCLASS()
class SWORDWORLD_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Initialize();

	BYTE GetTheoreticalByte(FVector Pos);

	BYTE GetTheoreticalByte(FVector Pos, FVector Offset0, FVector Offset1, FVector Offset2);

	float CalculateNoiseValue(FVector Pos, FVector Offset, float Scale);

	class AChunk* FindChunk(FVector Pos, bool Accurately = false);

	class Block* FindBlock(FVector WorldPos);

	class Block* FindBlock(int x, int y, int z);

	FVector GetSurfaceFloorPos(FVector2D Corner, float Height, bool IgnoreLeaves = true);

	UFUNCTION(BlueprintPure, Category = "Action")
	FVector GetSurfaceFloorPos(FVector2D Corner, float RadiusX, float RadiusY, float HalfHeight);

	UFUNCTION(BlueprintImplementableEvent, Category = "Action")
	void SpawnEnemiesFromChunk(FVector ChunkPos);

	static AWorldManager* Inst;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	int ChunkNumX = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	int ChunkNumY = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	int ChunkWidth = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	int ChunkHeight = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	float BlockSize = 80;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int InitialChunkNum = 4;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float ChunksGenerateInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 BlockTexSize = 16;

	UPROPERTY(EditAnywhere, Category = "Setup")
	UMaterialInterface* ChunkMaterialTransparent;

	UPROPERTY(EditAnywhere, Category = "Setup")
	UMaterialInterface* ChunkMaterial;

	UPROPERTY(EditAnywhere, Category = "Setup")
	UMaterialInterface* PlantsMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "States")
	bool IsGenerated;

	TMap<BlockType, FBlockStat> BlockStats;

	FVector2D BlockUVWidth;

	TArray<class AChunk*> Chunks;

	TMap<FVector, class Block*>  Blocks;

	float ChunkSize;

	int Seed = 0;

private:
	void GenerateChunks();

	void SpawnChunk(FVector Pos);

	TArray<FVector> UnGeneratedChunks;

	FTimerHandle ChunksGenerator;
};
