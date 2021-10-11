// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerControllerBase.h"
#include "Engine/World.h"

bool APlayerControllerBase::GetSightRayHitResult(FHitResult& HitResult, float RayDistance)
{
	int32 viewportSizeX, viewportSizeY;
	GetViewportSize(viewportSizeX, viewportSizeY);
	FVector sightPos;
	FVector rayDirection;
	if(DeprojectScreenPositionToWorld(viewportSizeX * 0.5f, viewportSizeY * 0.5f, sightPos, rayDirection))
	{
		FVector rayStart = PlayerCameraManager->GetCameraLocation();
		FVector rayEnd = rayStart + rayDirection * RayDistance;
		if(GetWorld()->LineTraceSingleByChannel(HitResult, rayStart, rayEnd, ECC_Visibility, FCollisionQueryParams(NAME_None, false, GetPawn())))
			return true;
	}
	return false;
}