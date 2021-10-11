// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "UObject/ConstructorHelpers.h"
#include "WorldManager.h"
#include "Materials/Material.h"

APlayerHUD::APlayerHUD()
{
	CrosshairTex = ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Game/Textures/UI/TUI_Crosshair")).Object;
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();

	FVector2D centerPos = FVector2D(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D drawPos = FVector2D(centerPos.X - 8, centerPos.Y - 8);
	FCanvasTileItem tileItem = FCanvasTileItem(drawPos, CrosshairTex->Resource, FLinearColor::White);
	tileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(tileItem);
}
