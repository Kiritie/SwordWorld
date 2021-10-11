// Fill out your copyright notice in the Description page of Project Settings.

#include "Block.h"
#include "Engine/World.h"
#include "Chunk.h"

Block::Block(AChunk* Owner, FVector index, BlockType blockType)
{
	this->Owner = Owner;
	this->index = index;
	this->blockType = blockType;

	vertices = TArray<FVector>();
	triangles = TArray<int32>();
	normals = TArray<FVector>();
	uvs = TArray<FVector2D>();
	vertexColors = TArray<FColor>();
	tangents = TArray<FProcMeshTangent>();
}

void Block::BuildFace(FVector corner, FVector up, FVector right, FVector forword, bool reversed)
{
	int verNum = vertices.Num();

	vertices.Add((index + corner) * AWorldManager::Inst->BlockSize);
	vertices.Add((index + corner + up) * AWorldManager::Inst->BlockSize);
	vertices.Add((index + corner + up + right) * AWorldManager::Inst->BlockSize);
	vertices.Add((index + corner + right) * AWorldManager::Inst->BlockSize);

	FVector2D uvWidth = AWorldManager::Inst->BlockUVWidth;
	FBlockStat blockStat = GetBlockStat();

	FVector2D uvCorner = blockStat.GetUVCorner(BlockFace::Default);
	if(uvCorner == FVector2D(-1, -1))
	{
		if(forword == FVector(0, -1, 0) || forword == FVector(0, 1, 0))
			uvCorner = blockStat.GetUVCorner(BlockFace::Side);
		else if(forword == FVector(0, 0, 1))
			uvCorner = blockStat.GetUVCorner(BlockFace::Top);
		else if(forword == FVector(-1, 0, 0))
		{
			uvCorner = blockStat.GetUVCorner(frontFaceOn ? BlockFace::Front_On : BlockFace::Front_Off);
			if(uvCorner == FVector2D(-1, -1))
				uvCorner = blockStat.GetUVCorner(BlockFace::Side);
		}
		else if(forword == FVector(1, 0, 0))
		{
			uvCorner = blockStat.GetUVCorner(BlockFace::Back);
			if(uvCorner == FVector2D(-1, -1))
				uvCorner = blockStat.GetUVCorner(BlockFace::Side);
		}
		else if(forword == FVector(0, 0, -1))
		{
			uvCorner = blockStat.GetUVCorner(BlockFace::Bottom);
			if(uvCorner == FVector2D(-1, -1))
				uvCorner = blockStat.GetUVCorner(BlockFace::Top);
		}
	}

	uvs.Add(FVector2D(uvCorner.X, uvCorner.Y + uvWidth.Y));
	uvs.Add(uvCorner);
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y));
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y + uvWidth.Y));

	if(reversed)
	{
		triangles.Add(verNum + 0);
		triangles.Add(verNum + 1);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 3);
		triangles.Add(verNum + 0);
	}
	else
	{
		triangles.Add(verNum + 1);
		triangles.Add(verNum + 0);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 3);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 0);
	}

	normals.Add(forword);
	normals.Add(forword);
	normals.Add(forword);
	normals.Add(forword);
}

void Block::BuildFace(FVector ver0, FVector ver1, FVector ver2, FVector ver3, FVector offset, FVector forword, bool reversed)
{
	int verNum = vertices.Num();

	vertices.Add(ver0 + index * AWorldManager::Inst->BlockSize + offset);
	vertices.Add(ver1 + index * AWorldManager::Inst->BlockSize + offset);
	vertices.Add(ver2 + index * AWorldManager::Inst->BlockSize + offset);
	vertices.Add(ver3 + index * AWorldManager::Inst->BlockSize + offset);

	FVector2D uvWidth = AWorldManager::Inst->BlockUVWidth;
	FBlockStat blockStat = GetBlockStat();

	FVector2D uvCorner = blockStat.GetUVCorner(BlockFace::Default);
	if(uvCorner == FVector2D(-1, -1))
	{
		if(forword == FVector(0, -1, 0) || forword == FVector(0, 1, 0))
			uvCorner = blockStat.GetUVCorner(BlockFace::Side);
		else if(forword == FVector(0, 0, 1))
			uvCorner = blockStat.GetUVCorner(BlockFace::Top);
		else if(forword == FVector(-1, 0, 0))
		{
			uvCorner = blockStat.GetUVCorner(frontFaceOn ? BlockFace::Front_On : BlockFace::Front_Off);
			if(uvCorner == FVector2D(-1, -1))
				uvCorner = blockStat.GetUVCorner(BlockFace::Side);
		}
		else if(forword == FVector(1, 0, 0))
		{
			uvCorner = blockStat.GetUVCorner(BlockFace::Back);
			if(uvCorner == FVector2D(-1, -1))
				uvCorner = blockStat.GetUVCorner(BlockFace::Side);
		}
		else if(forword == FVector(0, 0, -1))
		{
			uvCorner = blockStat.GetUVCorner(BlockFace::Bottom);
			if(uvCorner == FVector2D(-1, -1))
				uvCorner = blockStat.GetUVCorner(BlockFace::Top);
		}
	}

	uvs.Add(FVector2D(uvCorner.X, uvCorner.Y + uvWidth.Y));
	uvs.Add(uvCorner);
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y));
	uvs.Add(FVector2D(uvCorner.X + uvWidth.X, uvCorner.Y + uvWidth.Y));

	if(reversed)
	{
		triangles.Add(verNum + 0);
		triangles.Add(verNum + 1);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 3);
		triangles.Add(verNum + 0);
	}
	else
	{
		triangles.Add(verNum + 1);
		triangles.Add(verNum + 0);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 3);
		triangles.Add(verNum + 2);
		triangles.Add(verNum + 0);
	}

	normals.Add(forword);
	normals.Add(forword);
	normals.Add(forword);
	normals.Add(forword);
}

void Block::ClearMeshs()
{
	vertices.Empty();
	triangles.Empty();
	normals.Empty();
	uvs.Empty();
	vertexColors.Empty();
	tangents.Empty();
}

TArray<AChunk*> Block::GetBoundChunks()
{
	TArray<AChunk*> boundChunks = TArray<AChunk*>();
	TArray<FVector> boundVectors = TArray<FVector>();
	FVector worldPos = GetWorldPos();
	boundVectors.Add(worldPos + FVector(AWorldManager::Inst->BlockSize, 0, 0));
	boundVectors.Add(worldPos + FVector(-AWorldManager::Inst->BlockSize, 0, 0));
	boundVectors.Add(worldPos + FVector(0, -AWorldManager::Inst->BlockSize, 0));
	boundVectors.Add(worldPos + FVector(0, AWorldManager::Inst->BlockSize, 0));
	boundVectors.Add(worldPos + FVector(0, 0, AWorldManager::Inst->BlockSize));
	boundVectors.Add(worldPos + FVector(0, 0, -AWorldManager::Inst->BlockSize));
	AChunk* tmpChunk = nullptr;
	TArray<AChunk*> tmpChunks = Owner->GetBoundChunks();
	for(int i = 0; i < boundVectors.Num(); i++)
	{
		if(Owner->GetBlock(boundVectors[i]))
			continue;
		for(int j = 0; j < tmpChunks.Num(); j++)
		{
			if(tmpChunks[j]->GetBlock(boundVectors[i]))
			{
				tmpChunk = tmpChunks[j];
				break;
			}
		}
		if(!tmpChunk)
			tmpChunk = AWorldManager::Inst->FindChunk(boundVectors[i], true);
		if(tmpChunk && !boundChunks.Contains(tmpChunk))
			boundChunks.Add(tmpChunk);
	}
	return boundChunks;
}

FVector Block::GetWorldPos()
{
	return Owner->IndexToWorldPos(index);
}

FBlockStat Block::GetBlockStat()
{
	return AWorldManager::Inst->BlockStats[blockType];
}