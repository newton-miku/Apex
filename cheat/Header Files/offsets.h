#pragma once

//UPDATED 2/05/2022

#define OFFSET_KEY_JUMP 0x074baed8		//in_jump
#define OFFSET_KEY_DUCK 0x0b97eb28		//in_duck

#define OFFSET_ENTITYLIST 	0x19ed718   //cl_entitylist
#define OFFSET_LOCAL_ENT 	0x1d9e418    //local_player
#define OFFSET_ORIGIN 0x014c          //m_vecAbsOrigin
#define OFFSET_BONES 0x0f38            //m_bConstrainBetweenEndpoints
#define OFFSET_NAME_LIST 0xb97e0d0			//NameList
#define OFFSET_NAME	0x589                 //m_iName
#define OFFSET_SIG_NAME 0x580			//m_iSignifierName //Useless

#define OFFSET_MATRIX 0x11a210				//view_matrix ViewMatrix
#define OFFSET_RENDER 0x74bad90				//view_render ViewRender
#define OFFSET_HEALTH 0x438					//CPlayer->m_iHealth
#define OFFSET_SHIELD 0x0170				//CBaseEntity->m_shieldHealth
#define OFFSET_SHIELD_MAX 0x0174			//CBaseEntity->m_shieldHealthMax
#define OFFSET_LIFESTATE 0x0798				//m_lifeState, 1 = dead
#define OFFSET_OBSERVER_MODE 0x34c4				//m_iObserverMode Type-int 0-not 1/5-watching 3-choosing player
#define OFFSET_OBSERVER_TARGET 0x34d0				//m_hObserverTarget Type-int //no more networked

#define OFFSET_TEAM	0x0448                 //m_iTeamNum
#define OFFSET_BLEED_OUT_STATE 0x2728     //m_bleedoutState,>0 = knocked
#define OFFSET_VISIBLE_TIME 0x1b14        //CPlayer!lastVisibleTime
#define OFFSET_ITEM_ID	0x16b8

#define OFFSET_CAMERAPOS			0x1f40 //CPlayer!camera_origin
#define OFFSET_THIRDPERSON1          0x01d82830 + 0x6c //thirdperson_mayamode + 0x6c//0-视角跟随人物视角 !0-人物随意转动视角，镜头视角不转动
#define OFFSET_THIRDPERSON2         0x019d2760 + 0x6c //thirdperson_override + 0x6c
#define OFFSET_THIRDPERSON3          0x01d8a950 + 0x6c //thirdperson_screenspace + 0x6c//效果位置，默认为0

#define OFFSET_WEAPON				0x1aac //m_latestPrimaryWeapons
#define OFFSET_BULLET_SPEED         0x1f38 //CWeaponX!m_flProjectileSpeed
#define OFFSET_BULLET_SCALE         0x1f40 //CWeaponX!m_flProjectileScale
#define OFFSET_AMMO_IN_CLIP       0x16e4 //CWeaponX!m_ammoInClip		Type-int
#define OFFSET_IN_RELOADING         0x16fa //CWeaponX!m_bInReload		Type-bool

#define OFFSET_TIMESCALE			0x013ccca0//host_timescale

#define GLOW_DISTANCE 0x3B4					//m_highlightServerFadeEndTimes + 52(0x34)
#define GLOW_TYPE 0x02C0						//m_highlightFunctionBits
#define ITEM_GLOW_TYPE 0x02C4				//m_highlightFunctionBits + 4
#define OFFSET_GLOW_ENABLE          0x3c8 //1 = enabled, 2 = disabled
#define OFFSET_GLOW_THROUGH_WALLS   0x3d0 //2 = enabled, 5 = disabled
