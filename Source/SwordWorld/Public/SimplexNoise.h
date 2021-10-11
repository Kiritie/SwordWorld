// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SWORDWORLD_API SimplexNoise
{
public:
	static float Generate(float x);

	static float Generate(float x, float y);

	static float Generate(float x, float y, float z);

private:
	static int FastFloor(float x);

	static float Grad(int hash, float x);

	static float Grad(int hash, float x, float y);

	static float Grad(int hash, float x, float y, float z);

	static float Grad(int hash, float x, float y, float z, float t);

	static BYTE Perm[];

};
