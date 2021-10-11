// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"


// Sets default values
APlant::APlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->bUseAsyncCooking = true;
	SetRootComponent(MeshComponent);
}

// Called when the game starts or when spawned
void APlant::BeginPlay()
{
	Super::BeginPlay();

	PlantSize = AWorldManager::Inst->BlockSize * 0.3;

	blockType = FMath::FRand() < 0.5f ? BlockType::tall_grass : (BlockType)FMath::RandRange(19, 24);
	
	CreateVisualMesh();
}

// Called every frame
void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlant::BuildMesh(TArray<FVector>& vertices, TArray<int32>& triangles, TArray<FVector>& normals, TArray<FVector2D>& uvs, TArray<FColor>& vertexColors, TArray<FProcMeshTangent>& tangents)
{
	vertices.Add(FVector(-PlantSize, -PlantSize, 0));
	vertices.Add(FVector(-PlantSize, -PlantSize, PlantSize * 2));
	vertices.Add(FVector(PlantSize, PlantSize, PlantSize * 2));
	vertices.Add(FVector(PlantSize, PlantSize, 0));

	vertices.Add(FVector(PlantSize, -PlantSize, 0));
	vertices.Add(FVector(PlantSize, -PlantSize, PlantSize * 2));
	vertices.Add(FVector(-PlantSize, PlantSize, PlantSize * 2));
	vertices.Add(FVector(-PlantSize, PlantSize, 0));

	FVector2D uvWidth = AWorldManager::Inst->BlockUVWidth;
	FBlockStat blockStat = GetBlockStat();

	FVector2D uvCorner = blockStat.GetUVCorner(BlockFace::Default);

	uvs.Add(FVector2D(uvCorner.X, uvCorner.Y + uvWidth.Y));
	uvs.Add(uvCorner);
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y));
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y + uvWidth.Y));

	uvs.Add(FVector2D(uvCorner.X, uvCorner.Y + uvWidth.Y));
	uvs.Add(uvCorner);
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y));
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y + uvWidth.Y));

	triangles.Add(1);
	triangles.Add(0);
	triangles.Add(2);
	triangles.Add(3);
	triangles.Add(2);
	triangles.Add(0);

	triangles.Add(5);
	triangles.Add(4);
	triangles.Add(6);
	triangles.Add(7);
	triangles.Add(6);
	triangles.Add(4);

	normals.Add(FVector(-1, 1, 0));
	normals.Add(FVector(-1, 1, 0));
	normals.Add(FVector(-1, 1, 0));
	normals.Add(FVector(-1, 1, 0));

	normals.Add(FVector(1, 1, 0));
	normals.Add(FVector(1, 1, 0));
	normals.Add(FVector(1, 1, 0));
	normals.Add(FVector(1, 1, 0));
}

void APlant::CreateVisualMesh()
{
	TArray<FVector> vertices = TArray<FVector>();
	TArray<int32> triangles = TArray<int32>();
	TArray<FVector> normals = TArray<FVector>();
	TArray<FVector2D> uvs = TArray<FVector2D>();
	TArray<FColor> vertexColors = TArray<FColor>();
	TArray<FProcMeshTangent> tangents = TArray<FProcMeshTangent>();

	BuildMesh(vertices, triangles, normals, uvs, vertexColors, tangents);

	MeshComponent->CreateMeshSection(0, vertices, triangles, normals, uvs, vertexColors, tangents, false);
	MeshComponent->SetMaterial(0, AWorldManager::Inst->PlantsMaterial);
}

FBlockStat APlant::GetBlockStat()
{
	return AWorldManager::Inst->BlockStats[blockType];
}