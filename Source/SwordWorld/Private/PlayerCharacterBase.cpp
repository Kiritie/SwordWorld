// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterBase.h"
#include "Engine.h"
#include "Engine/World.h"
#include "PlayerControllerBase.h"
#include "Chunk.h"
#include "WorldManager.h"
#include "Block.h"
#include "Global.h"


// Sets default values
APlayerCharacterBase::APlayerCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	//MoveToBirthPoint(FVector(0, 0, 0));
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Destroy"), IE_Pressed, this, &APlayerCharacterBase::DestroyBlock);

	PlayerInputComponent->BindAction(TEXT("Generate"), IE_Pressed, this, &APlayerCharacterBase::GenerateBlock);
}

void APlayerCharacterBase::MoveToBirthPoint(FVector BirthPoint)
{
	BirthPoint = AWorldManager::Inst->GetSurfaceFloorPos(FVector2D(BirthPoint.X, BirthPoint.Y), GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	SetActorLocation(BirthPoint + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}

void APlayerCharacterBase::DestroyBlock()
{
	FHitResult hitResult;
	if(Cast<APlayerControllerBase>(GetController())->GetSightRayHitResult(hitResult, 500))
	{
		AChunk* chunk = Cast<AChunk>(hitResult.GetActor());
		if(chunk)
		{
			Block* block = chunk->GetBlock(hitResult.ImpactPoint - hitResult.ImpactNormal * AWorldManager::Inst->BlockSize * 0.5f);
			if(block)
			{
				chunk->DestoryBlock(block);
			}
		}
	}
}

void APlayerCharacterBase::GenerateBlock()
{
	FHitResult hitResult;
	if(Cast<APlayerControllerBase>(GetController())->GetSightRayHitResult(hitResult, 500))
	{
		AChunk* chunk = Cast<AChunk>(hitResult.GetActor());
		if(chunk)
		{
			Block* block = chunk->GetBlock(hitResult.ImpactPoint - hitResult.ImpactNormal * AWorldManager::Inst->BlockSize * 0.5f);
			if(block)
			{
				FVector worldPos = block->GetWorldPos() + (hitResult.ImpactNormal + FVector::OneVector * 0.5f) * AWorldManager::Inst->BlockSize;
				if(GetCapsuleComponent()->OverlapComponent(worldPos, FQuat::Identity, FCollisionShape::MakeBox(FVector::OneVector * AWorldManager::Inst->BlockSize * 0.5f)))
					return;
				Block* newBlock = new Block(chunk, block->index + hitResult.ImpactNormal, (BlockType)FMath::RandRange(1, 71));
				chunk->GenerateBlock(newBlock);
			}
		}
	}
}