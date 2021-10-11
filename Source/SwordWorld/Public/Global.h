// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockStat.h"

UENUM()
enum class BlockType : uint8
{
	//¿Õ
	empty,
	//ÄàÍÁ
	dirt,
	//²ÝµØ
	grass,
	//»ýÑ©µÄ²ÝµØ
	grass_snowed,
	//É³
	sand,
	//Ê¯Í·
	stone,
	//Ê¯°å
	stone_slab,
	//Ê¯×©
	stonebrick,
	//×©¿é
	brick,
	//Ñ©¿é
	snow,
	//¹¤×÷Ì¨
	crafting_table,
	//ÏðÊ÷Ä¾°å
	planks_oak,
	//°×É«ÑòÃ«
	wool_colored_white,
	//ÏðÄ¾
	log_oak,
	//°×èëÄ¾
	log_birch,
	//ÏðÊ÷Ò¶
	leaves,
	//²£Á§¿é
	glass,
	//¸ß¾¥²Ý
	tall_grass,
	//
	flower_allium,
	//
	flower_blue_orchid,
	//
	flower_dandelion,
	//
	flower_houstonia,
	//
	flower_oxeye_daisy,
	//
	flower_paeonia,
	//
	flower_pot,
	//
	flower_rose,
	//
	flower_tulip_orange,
	//
	flower_tulip_pink,
	//
	flower_tulip_red,
	//
	flower_tulip_white
};

class SWORDWORLD_API Global
{

};
