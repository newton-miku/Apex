#pragma once

//UPDATED 11/04/2022

#define OFFSET_ENTITYLIST 0x19ED718   //cl_entitylist
#define OFFSET_LOCAL_ENT 0x1D9E418    //LocalPlayer
#define OFFSET_ORIGIN 0x4          //m_vecAbsOrigin
#define OFFSET_BONES 0xF38            //m_bConstrainBetweenEndpoints
#define OFFSET_NAME	0x589                 //m_iName

#define OFFSET_MATRIX 0x5fb031
#define OFFSET_RENDER 0x1F81058
#define OFFSET_HEALTH 0x438					//CPlayer->m_iHealth
#define OFFSET_SHIELD 0x170					//CBaseEntity->m_shieldHealth

#define OFFSET_TEAM	0x448                 //m_iTeamNum
#define OFFSET_BLEED_OUT_STATE 0x2728     //m_bleedoutState
#define OFFSET_VISIBLE_TIME 0x1b14        //CPlayer!lastVisibleTime
#define OFFSET_ITEM_ID	0x16b8
#define GLOW_TYPE 0x2C4
#define OFFSET_GLOW_ENABLE          0x3c8 //1 = enabled, 2 = disabled
#define OFFSET_GLOW_THROUGH_WALLS   0x3d0 //2 = enabled, 5 = disabled
