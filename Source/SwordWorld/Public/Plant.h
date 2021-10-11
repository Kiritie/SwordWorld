// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "WorldManager.h"
#include "Plant.generated.h"

UCLASS()
class SWORDWORLD_API APlant : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BuildMesh(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals, TArray<FVector2D>& uvs, TArray<FColor>& vertexColors, TArray<FProcMeshTangent>& tangents);

	void CreateVisualMesh();

	FBlockStat GetBlockStat();

	BlockType blockType;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UProceduralMeshComponent* MeshComponent;
	
private:
	float PlantSize = 0;
};
