#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture.h"
#include "BlockStat.generated.h"

UENUM()
enum class BlockFace : uint8
{
	Default,
	Side,
	Top,
	Front_On,
	Front_Off,
	Back,
	Bottom
};

USTRUCT()
struct SWORDWORLD_API FBlockStat
{
	GENERATED_BODY()

public:
	FBlockStat();

	FBlockStat(TMap<BlockFace, FVector2D> BlockUVCorners);

	UPROPERTY(EditAnywhere)
	TMap<BlockFace, FVector2D> BlockUVCorners;

	FVector2D GetUVCorner(BlockFace blockFace);
};

FORCEINLINE FBlockStat::FBlockStat()
{
	
}

FORCEINLINE FBlockStat::FBlockStat(TMap<BlockFace, FVector2D> BlockUVCorners)
{
	this->BlockUVCorners = BlockUVCorners;
}

FORCEINLINE FVector2D FBlockStat::GetUVCorner(BlockFace blockFace)
{
	if(BlockUVCorners.Contains(blockFace))
		return BlockUVCorners[blockFace];
	return FVector2D(-1, -1);
}
