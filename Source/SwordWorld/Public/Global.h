// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockStat.h"

UENUM()
enum class BlockType : uint8
{
	//��
	empty,
	//����
	dirt,
	//�ݵ�
	grass,
	//��ѩ�Ĳݵ�
	grass_snowed,
	//ɳ
	sand,
	//ʯͷ
	stone,
	//ʯ��
	stone_slab,
	//ʯש
	stonebrick,
	//ש��
	brick,
	//ѩ��
	snow,
	//����̨
	crafting_table,
	//����ľ��
	planks_oak,
	//��ɫ��ë
	wool_colored_white,
	//��ľ
	log_oak,
	//����ľ
	log_birch,
	//����Ҷ
	leaves,
	//������
	glass,
	//�߾���
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
