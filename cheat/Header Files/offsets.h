#pragma once

//UPDATED 15/04/2022

#define OFFSET_ENTITYLIST 	0x19ed718   //cl_entitylist
#define OFFSET_LOCAL_ENT 	0x1d9e418    //local_player
#define OFFSET_ORIGIN 0x014c          //m_vecAbsOrigin
#define OFFSET_BONES 0x0f38            //m_bConstrainBetweenEndpoints
#define OFFSET_NAME_LIST 0xb97e0d0			//NameList
#define OFFSET_NAME	0x589                 //m_iName

#define OFFSET_MATRIX 0x11a210				//view_matrix ViewMatrix
#define OFFSET_RENDER 0x74bad90				//view_render ViewRender
#define OFFSET_HEALTH 0x438					//CPlayer->m_iHealth
#define OFFSET_SHIELD 0x170					//CBaseEntity->m_shieldHealth

#define OFFSET_TEAM	0x0448                 //m_iTeamNum
#define OFFSET_BLEED_OUT_STATE 0x2728     //m_bleedoutState
#define OFFSET_VISIBLE_TIME 0x1b14        //CPlayer!lastVisibleTime
#define OFFSET_ITEM_ID	0x16b8

#define OFFSET_CAMERAPOS			0x1f40 //CPlayer!camera_origin

#define OFFSET_WEAPON				0x1aac //m_latestPrimaryWeapons
#define OFFSET_BULLET_SPEED         0x1f38 //CWeaponX!m_flProjectileSpeed
#define OFFSET_BULLET_SCALE         0x1f40 //CWeaponX!m_flProjectileScale

#define GLOW_DISTANCE 0x3B4
#define GLOW_TYPE 0x02C0						//m_playerLocalGravityLineStartEntity
#define ITEM_GLOW_TYPE 0x02C4				//m_playerLocalGravityLineEndEntity
#define OFFSET_GLOW_ENABLE          0x3c8 //1 = enabled, 2 = disabled
#define OFFSET_GLOW_THROUGH_WALLS   0x3d0 //2 = enabled, 5 = disabled
