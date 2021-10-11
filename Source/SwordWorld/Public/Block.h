// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldManager.h"

class SWORDWORLD_API Block
{
public:
	Block(class AChunk* Owner, FVector index, BlockType blockType);
	
	void BuildFace(FVector corner, FVector up, FVector right, FVector forword, bool reversed);

	void BuildFace(FVector ver0, FVector ver1, FVector ver2, FVector ver3, FVector offset,FVector forword, bool reversed);

	void ClearMeshs();

	TArray<class AChunk*> GetBoundChunks();

	FVector GetWorldPos();

	BlockType blockType;

	FBlockStat GetBlockStat();

	TArray<FVector> vertices;
	TArray<int32> triangles;
	TArray<FVector> normals;
	TArray<FVector2D> uvs;
	TArray<FColor> vertexColors;
	TArray<struct FProcMeshTangent> tangents;

	class AChunk* Owner;

	FVector index;

	bool frontFaceOn = false;
};
