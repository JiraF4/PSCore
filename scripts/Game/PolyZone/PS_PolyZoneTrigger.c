class PS_PolyZoneTriggerClass: SCR_BaseTriggerEntityClass
{
	
}

class PS_PolyZoneTrigger : SCR_BaseTriggerEntity
{
	PS_PolyZone m_polyZone;
	
	[Attribute()]
	ref PS_PolyZoneEffect m_polyZoneEffect;
	
	[Attribute("0")]
	bool m_bReversed;
	
	[Attribute("0")]
	bool m_bAliveOnly;
	
	[Attribute("")]
	FactionKey m_sFactionKey;
	
	[Attribute("")]
	string m_sGroupKey;
	
	[Attribute("0", UIWidgets.CheckBox, "Enable debug log")]
	bool m_bDebug;
	
	protected ref map<IEntity, string> m_mLastFilterState = new map<IEntity, string>();
	
	//------------------------------------------------------------------------------------------------
	protected void PrintDebug(string message)
	{
		if (!m_bDebug)
			return;
		
		string entityName = GetName();
		if (entityName.IsEmpty())
		{
			IEntity parent = GetParent();
			if (parent && !parent.GetName().IsEmpty())
				entityName = string.Format("%1 (parent: %2)", ToString(), parent.GetName());
			else
				entityName = ToString();
		}
		
		Print(string.Format("[PS_PolyZoneTrigger::%1] %2", entityName, message));
	}
	
	//------------------------------------------------------------------------------------------------
	protected void LogFilterState(IEntity ent, string state, string details = "")
	{
		if (!m_bDebug || !ent)
			return;
		
		string oldState = "";
		if (m_mLastFilterState.Contains(ent))
			oldState = m_mLastFilterState.Get(ent);
		
		if (oldState == state)
			return;
		
		if (m_mLastFilterState.Count() > 128)
			m_mLastFilterState.Clear();
		
		m_mLastFilterState.Set(ent, state);
		
		if (details != "")
			details = " [" + details + "]";
		
		if (oldState.IsEmpty())
			PrintDebug(string.Format("Filter: ent=%1 initial state: %2%3", ent, state, details));
		else
			PrintDebug(string.Format("Filter: ent=%1 state changed: %2 -> %3%4", ent, oldState, state, details));
	}
	
	//------------------------------------------------------------------------------------------------
	override void OnInit(IEntity owner)
	{
		SetSphereRadius(999999);
		EnablePeriodicQueries(true);
		SetUpdateRate(0.3);
		
		if (owner)
		{
			IEntity parent = owner.GetParent();
			if (parent)
				m_polyZone = PS_PolyZone.Cast(parent.FindComponent(PS_PolyZone));
		}
		
		if (m_bDebug)
		{
			if (m_polyZone)
			{
				string effectName = "none";
				if (m_polyZoneEffect)
					effectName = m_polyZoneEffect.ClassName();
				PrintDebug(string.Format("OnInit: Initialized with PolyZone '%1'. Parameters: effect=%2, reversed=%3, aliveOnly=%4, factionKey='%5', groupKey='%6'",
					m_polyZone, effectName, m_bReversed, m_bAliveOnly, m_sFactionKey, m_sGroupKey));
			}
			else
			{
				PrintDebug(string.Format("OnInit: WARNING - PS_PolyZone component not found on parent of '%1'!", owner));
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! Evaluates whether the entity should currently suffer the zone penalty
	bool ShouldEntityHaveEffect(IEntity ent)
	{
		if (!m_polyZone || !ent)
			return false;
		
		vector worldPos = ent.GetOrigin();
		IEntity vehicle = CompartmentAccessComponent.GetVehicleIn(ent);
		if (vehicle)
			worldPos = vehicle.GetOrigin();
		else
		{
			IEntity parent = ent.GetParent();
			if (parent && (Vehicle.Cast(parent) || Turret.Cast(parent)))
				worldPos = parent.GetOrigin();
		}
		
		bool inside = m_polyZone.IsInsidePolygon(worldPos);
		return (inside != m_bReversed);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Adds the zone penalty effect to a character
	void AddZoneEffect(SCR_ChimeraCharacter character)
	{
		if (!character || !m_polyZoneEffect)
			return;
		
		PS_PolyZoneEffectHandler handler = PS_PolyZoneEffectHandler.Cast(character.FindComponent(PS_PolyZoneEffectHandler));
		if (!handler)
			return;
		
		if (m_bDebug)
			PrintDebug(string.Format("AddZoneEffect: Adding effect '%1' to character=%2", m_polyZoneEffect.ClassName(), character));
		
		handler.AddEffect(this, m_polyZoneEffect);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Removes the zone penalty effect from a character
	void RemoveZoneEffect(SCR_ChimeraCharacter character)
	{
		if (!character || !m_polyZoneEffect)
			return;
		
		PS_PolyZoneEffectHandler handler = PS_PolyZoneEffectHandler.Cast(character.FindComponent(PS_PolyZoneEffectHandler));
		if (!handler)
			return;
		
		if (m_bDebug)
			PrintDebug(string.Format("RemoveZoneEffect: Removing effect '%1' from character=%2", m_polyZoneEffect.ClassName(), character));
		
		handler.RemoveEffect(this, m_polyZoneEffect);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Validates whether a specific character matches faction, group, and alive filters
	bool MatchesCharacterFilter(SCR_ChimeraCharacter character)
	{
		if (!character)
			return false;
		
		if (m_bAliveOnly)
		{
			SCR_DamageManagerComponent damageManager = character.GetDamageManager();
			if (!damageManager)
				damageManager = SCR_DamageManagerComponent.Cast(character.FindComponent(SCR_DamageManagerComponent));
			if (!damageManager || damageManager.GetState() == EDamageState.DESTROYED)
				return false;
		}
		
		if (m_sFactionKey != "")
		{
			FactionAffiliationComponent factionAffiliation = character.PS_GetFactionAffiliationComponent();
			if (!factionAffiliation)
				factionAffiliation = FactionAffiliationComponent.Cast(character.FindComponent(FactionAffiliationComponent));
			if (!factionAffiliation)
				return false;
			Faction defaultFaction = factionAffiliation.GetDefaultAffiliatedFaction();
			if (!defaultFaction || defaultFaction.GetFactionKey() != m_sFactionKey)
				return false;
		}
		
		if (m_sGroupKey != "")
		{
			SCR_AIGroup aiGroup;
			AIAgent aiAgent = character.PS_GetAIAgent();
			if (aiAgent)
				aiGroup = SCR_AIGroup.Cast(aiAgent.GetParentGroup());
			if (aiGroup && aiGroup.m_BotsGroup)
				aiGroup = aiGroup.m_BotsGroup;
			
			if (!aiGroup)
			{
				PS_PlayableManager playableManager = PS_PlayableManager.GetInstance();
				if (playableManager)
				{
					PS_PlayableComponent playable = PS_PlayableComponent.Cast(character.FindComponent(PS_PlayableComponent));
					if (playable)
						aiGroup = playableManager.GetPlayerGroupByPlayable(playable.GetRplId());
				}
			}
			
			if (!aiGroup)
				return false;
			
			bool groupMatches = aiGroup.GetName().Contains(m_sGroupKey);
			if (!groupMatches && aiGroup.m_BotsGroup)
				groupMatches = aiGroup.m_BotsGroup.GetName().Contains(m_sGroupKey);
			
			if (!groupMatches)
				return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!ent)
			return false;
			
		if (!m_polyZone)
		{
			LogFilterState(ent, "ALLOWED_NO_POLYZONE", "m_polyZone is null");
			return true;
		}
		
		vector checkPos = ent.GetOrigin();
		IEntity vehicleIn = CompartmentAccessComponent.GetVehicleIn(ent);
		if (vehicleIn)
			checkPos = vehicleIn.GetOrigin();
		else
		{
			IEntity parent = ent.GetParent();
			if (parent && (Vehicle.Cast(parent) || Turret.Cast(parent)))
				checkPos = parent.GetOrigin();
		}
		
		if (!m_polyZone.IsInsidePolygon(checkPos))
		{
			LogFilterState(ent, "OUTSIDE_POLYGON", string.Format("pos: %1", checkPos));
			return false;
		}
		
		// 1. Character filter evaluation
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
		if (character)
		{
			if (m_bAliveOnly || m_sFactionKey != "" || m_sGroupKey != "")
			{
				if (!MatchesCharacterFilter(character))
				{
					LogFilterState(ent, "REJECTED_CHARACTER_FILTER");
					return false;
				}
			}
			
			LogFilterState(ent, "PASSED_CHARACTER");
			return true;
		}
		
		// 2. Vehicle / Turret filter evaluation
		SCR_BaseCompartmentManagerComponent compManager = SCR_BaseCompartmentManagerComponent.Cast(ent.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (compManager)
		{
			if (m_bAliveOnly)
			{
				SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.Cast(ent.FindComponent(SCR_DamageManagerComponent));
				if (!damageManager || damageManager.GetState() == EDamageState.DESTROYED)
				{
					LogFilterState(ent, "REJECTED_VEHICLE_DESTROYED");
					return false;
				}
			}
			
			if (m_sFactionKey != "" || m_sGroupKey != "")
			{
				array<IEntity> occupants = {};
				compManager.GetOccupants(occupants);
				if (occupants.IsEmpty())
				{
					if (m_sFactionKey != "")
					{
						FactionAffiliationComponent facAff = FactionAffiliationComponent.Cast(ent.FindComponent(FactionAffiliationComponent));
						if (!facAff || !facAff.GetDefaultAffiliatedFaction() || facAff.GetDefaultAffiliatedFaction().GetFactionKey() != m_sFactionKey)
						{
							LogFilterState(ent, "REJECTED_EMPTY_VEHICLE_FACTION");
							return false;
						}
					}
					else
					{
						LogFilterState(ent, "REJECTED_EMPTY_VEHICLE_GROUP");
						return false;
					}
				}
				else
				{
					bool anyMatches = false;
					foreach (IEntity occ : occupants)
					{
						SCR_ChimeraCharacter occChar = SCR_ChimeraCharacter.Cast(occ);
						if (occChar && MatchesCharacterFilter(occChar))
						{
							anyMatches = true;
							break;
						}
					}
					if (!anyMatches)
					{
						LogFilterState(ent, "REJECTED_VEHICLE_NO_MATCHING_OCCUPANTS");
						return false;
					}
				}
			}
			
			LogFilterState(ent, "PASSED_VEHICLE");
			return true;
		}
		
		LogFilterState(ent, "PASSED");
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnActivate(IEntity ent)
	{
		if (m_bDebug)
			PrintDebug(string.Format("OnActivate: ent=%1 entered trigger", ent));
			
		if (!m_polyZoneEffect)
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnActivate: ent=%1 ignored (m_polyZoneEffect is null)", ent));
			return;
		}
		
		// 1. If infantry character on foot:
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
		if (character)
		{
			if (ShouldEntityHaveEffect(character))
				AddZoneEffect(character);
			else
				RemoveZoneEffect(character);
			return;
		}
		
		// 2. If vehicle or turret: apply to all matching occupants:
		SCR_BaseCompartmentManagerComponent compManager = SCR_BaseCompartmentManagerComponent.Cast(ent.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (compManager)
		{
			array<IEntity> occupants = {};
			compManager.GetOccupants(occupants);
			foreach (IEntity occ : occupants)
			{
				SCR_ChimeraCharacter occChar = SCR_ChimeraCharacter.Cast(occ);
				if (occChar && MatchesCharacterFilter(occChar))
				{
					if (ShouldEntityHaveEffect(occChar))
						AddZoneEffect(occChar);
					else
						RemoveZoneEffect(occChar);
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnDeactivate(IEntity ent)
	{
		if (m_bDebug)
		{
			PrintDebug(string.Format("OnDeactivate: ent=%1 exited trigger", ent));
			if (ent && m_mLastFilterState.Contains(ent))
				m_mLastFilterState.Remove(ent);
		}
		
		if (!m_polyZoneEffect)
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnDeactivate: ent=%1 ignored (m_polyZoneEffect is null)", ent));
			return;
		}
		
		// 1. If infantry character:
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
		if (character)
		{
			if (ShouldEntityHaveEffect(character))
				AddZoneEffect(character);
			else
				RemoveZoneEffect(character);
			return;
		}
		
		// 2. If vehicle or turret: evaluate all occupants:
		SCR_BaseCompartmentManagerComponent compManager = SCR_BaseCompartmentManagerComponent.Cast(ent.FindComponent(SCR_BaseCompartmentManagerComponent));
		if (compManager)
		{
			array<IEntity> occupants = {};
			compManager.GetOccupants(occupants);
			foreach (IEntity occ : occupants)
			{
				SCR_ChimeraCharacter occChar = SCR_ChimeraCharacter.Cast(occ);
				if (occChar && MatchesCharacterFilter(occChar))
				{
					if (ShouldEntityHaveEffect(occChar))
						AddZoneEffect(occChar);
					else
						RemoveZoneEffect(occChar);
				}
			}
		}
	}
}