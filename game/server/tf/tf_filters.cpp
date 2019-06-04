//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. ========//
//
// Purpose:
//
//=============================================================================//
#include "cbase.h"
#include "filters.h"
#include "team_control_point.h"
#include "tf_gamerules.h"
#include "tf_playerclass_shared.h"
#include "tf_player.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//=============================================================================
//
// Team Fortress Team Filter
//
class CFilterTFTeam : public CBaseFilter
{
	DECLARE_CLASS( CFilterTFTeam, CBaseFilter );

public:

	void InputRoundSpawn( inputdata_t &inputdata );
	void InputRoundActivate( inputdata_t &inputdata );

	inline bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );

private:

	string_t m_iszControlPointName;

	DECLARE_DATADESC();
};

BEGIN_DATADESC( CFilterTFTeam )

DEFINE_KEYFIELD( m_iszControlPointName, FIELD_STRING, "controlpoint" ),

// Inputs.
DEFINE_INPUTFUNC( FIELD_VOID, "RoundSpawn", InputRoundSpawn ),
DEFINE_INPUTFUNC( FIELD_VOID, "RoundActivate", InputRoundActivate ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( filter_activator_tfteam, CFilterTFTeam );

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CFilterTFTeam::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	// is the entity we're asking about on the winning 
	// team during the bonus time? (winners pass all filters)
	if (  TFGameRules() &&
		( TFGameRules()->State_Get() == GR_STATE_TEAM_WIN ) && 
		( TFGameRules()->GetWinningTeam() == pEntity->GetTeamNumber() ) )
	{
		// this should open all doors for the winners
		if ( m_bNegated )
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return ( pEntity->GetTeamNumber() == GetTeamNumber() );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFilterTFTeam::InputRoundSpawn( inputdata_t &input )
{

}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFilterTFTeam::InputRoundActivate( inputdata_t &input )
{
	if ( m_iszControlPointName != NULL_STRING )
	{
		CTeamControlPoint *pControlPoint = dynamic_cast<CTeamControlPoint*>( gEntList.FindEntityByName( NULL, m_iszControlPointName ) );
		if ( pControlPoint )
		{
			ChangeTeam( pControlPoint->GetTeamNumber() );
		}
		else
		{
			Warning( "%s failed to find control point named '%s'\n", GetClassname(), STRING(m_iszControlPointName) );
		}
	}
}

//=============================================================================
//
// Team Fortress Class Filter
//
class CFilterTFClass : public CBaseFilter
{
	DECLARE_CLASS( CFilterTFClass, CBaseFilter );

public:

	void InputRoundSpawn( inputdata_t &inputdata );
	void InputRoundActivate( inputdata_t &inputdata );

	inline bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );

private:

	int m_iszTFClassName;

	DECLARE_DATADESC();
};

BEGIN_DATADESC( CFilterTFClass )

DEFINE_KEYFIELD( m_iszTFClassName, FIELD_INTEGER, "tfclass" ),

// Inputs.
DEFINE_INPUTFUNC( FIELD_VOID, "RoundActivate", InputRoundActivate ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( filter_tf_class, CFilterTFClass );

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CFilterTFClass::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	CTFPlayer *pPlayer = dynamic_cast< CTFPlayer * >(pEntity);
	if (!pPlayer)
		return false;
	return ( pPlayer->IsPlayerClass(m_iszTFClassName) );
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFilterTFClass::InputRoundActivate( inputdata_t &input )
{

}

//=============================================================================
//
// Team Fortress Class Filter
//
class CFilterTFMoney : public CBaseFilter
{
	DECLARE_CLASS( CFilterTFMoney, CBaseFilter );

public:

	void InputRoundSpawn( inputdata_t &inputdata );
	void InputRoundActivate( inputdata_t &inputdata );

	inline bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );

private:

	int m_iszMoneyAmount;
	int m_iszCheckType;

	DECLARE_DATADESC();
};

BEGIN_DATADESC( CFilterTFMoney )

DEFINE_KEYFIELD( m_iszMoneyAmount, FIELD_INTEGER, "moneyamount" ),
DEFINE_KEYFIELD( m_iszCheckType, FIELD_INTEGER, "checktype" ),
// Inputs.
DEFINE_INPUTFUNC( FIELD_VOID, "RoundActivate", InputRoundActivate ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( filter_tf_money, CFilterTFMoney );

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CFilterTFMoney::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	CTFPlayer *pPlayer = dynamic_cast< CTFPlayer * >(pEntity);
	if (!pPlayer)
		return false;
	
	switch (m_iszCheckType) 
	{
		case 0: 
			return ( pPlayer->m_iAccount > m_iszMoneyAmount );
			break;		
		case 1:
			return ( pPlayer->m_iAccount < m_iszMoneyAmount );
			break;	
		case 2:
			return ( pPlayer->m_iAccount == m_iszMoneyAmount );
			break;		
		default:
			return false;
			break;
	}
	return false;
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFilterTFMoney::InputRoundActivate( inputdata_t &input )
{

}

//=============================================================================
//
// Team Fortress Active Weapon Filter
//
class CFilterTFActiveWeapon : public CBaseFilter
{
	DECLARE_CLASS( CFilterTFActiveWeapon, CBaseFilter );

public:

	void InputRoundSpawn( inputdata_t &inputdata );
	void InputRoundActivate( inputdata_t &inputdata );

	inline bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );

private:

	string_t m_iszTFWeaponName;

	DECLARE_DATADESC();
};

BEGIN_DATADESC( CFilterTFActiveWeapon )

DEFINE_KEYFIELD( m_iszTFWeaponName, FIELD_STRING, "weaponname" ),

// Inputs.
DEFINE_INPUTFUNC( FIELD_VOID, "RoundActivate", InputRoundActivate ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( filter_tf_active_weapon, CFilterTFActiveWeapon );

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CFilterTFActiveWeapon::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	CTFPlayer *pPlayer = dynamic_cast< CTFPlayer * >(pEntity);
	if (!pPlayer)
		return false;
	if ( !pPlayer->GetActiveWeapon() )
		return false;
	DevMsg("Active weapon is %s \n Filtered weapon is %s", pPlayer->GetActiveWeapon()->GetClassname(), STRING(m_iszTFWeaponName) );
	return ( pPlayer->GetActiveWeapon()->GetClassname() == STRING(m_iszTFWeaponName) );
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFilterTFActiveWeapon::InputRoundActivate( inputdata_t &input )
{

}


//=============================================================================
//
// Team Fortress Owning Weapon Filter
//
class CFilterTFOwnsWeapon : public CBaseFilter
{
	DECLARE_CLASS( CFilterTFOwnsWeapon, CBaseFilter );

public:

	void InputRoundSpawn( inputdata_t &inputdata );
	void InputRoundActivate( inputdata_t &inputdata );

	inline bool PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity );

private:

	string_t m_iszTFWeaponName;

	DECLARE_DATADESC();
};

BEGIN_DATADESC( CFilterTFOwnsWeapon )

DEFINE_KEYFIELD( m_iszTFWeaponName, FIELD_STRING, "weaponname" ),

// Inputs.
DEFINE_INPUTFUNC( FIELD_VOID, "RoundActivate", InputRoundActivate ),

END_DATADESC()


LINK_ENTITY_TO_CLASS( filter_tf_owns_weapon, CFilterTFOwnsWeapon );

//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
bool CFilterTFOwnsWeapon::PassesFilterImpl( CBaseEntity *pCaller, CBaseEntity *pEntity )
{
	CTFPlayer *pPlayer = dynamic_cast< CTFPlayer * >(pEntity);
	if (!pPlayer)
		return false;
	
	DevMsg("WeaponID is %d \n", AliasToWeaponID( STRING(m_iszTFWeaponName) ) );
	bool bSuccess = pPlayer->OwnsWeaponID(AliasToWeaponID(STRING(m_iszTFWeaponName)) );
	DevMsg("Success is %d\n", bSuccess);
	return bSuccess;
}
//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CFilterTFOwnsWeapon::InputRoundActivate( inputdata_t &input )
{

}