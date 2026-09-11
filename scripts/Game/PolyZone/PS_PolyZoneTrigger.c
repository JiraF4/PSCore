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
	override void OnInit(IEntity owner)
	{
		if (owner)
		{
			IEntity parent = owner.GetParent();
			if (parent)
			{
				m_polyZone = PS_PolyZone.Cast(parent.FindComponent(PS_PolyZone));
				if (!m_bReversed && m_polyZone)
					m_bReversed = m_polyZone.m_bReversed;
			}
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
		bool isReversed = m_bReversed;
		if (!isReversed && m_polyZone)
			isReversed = m_polyZone.m_bReversed;
		return (inside != isReversed);
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
	//! Resolves the group associated with the given character (player group, AI group, or cached initial group)
	SCR_AIGroup GetCharacterGroup(SCR_ChimeraCharacter character)
	{
		if (!character)
			return null;
		
		SCR_AIGroup aiGroup;
		
		// 1. If character is controlled by a human player, resolve group via SCR_GroupsManagerComponent
		PlayerManager playerManager = GetGame().GetPlayerManager();
		int playerId = 0;
		if (playerManager)
			playerId = playerManager.GetPlayerIdFromControlledEntity(character);
		
		if (playerId <= 0)
			playerId = SCR_PossessingManagerComponent.GetPlayerIdFromControlledEntity(character);
		if (playerId <= 0)
			playerId = SCR_PossessingManagerComponent.GetPlayerIdFromMainEntity(character);
		
		if (playerId > 0)
		{
			SCR_GroupsManagerComponent groupsManager = SCR_GroupsManagerComponent.GetInstance();
			if (groupsManager)
				aiGroup = groupsManager.GetPlayerGroup(playerId);
		}
		
		// 2. If AI or player group not found, resolve via AI agent
		if (!aiGroup)
		{
			AIAgent aiAgent = character.PS_GetAIAgent();
			if (!aiAgent)
			{
				ChimeraAIControlComponent aiControl = ChimeraAIControlComponent.Cast(character.FindComponent(ChimeraAIControlComponent));
				if (aiControl)
					aiAgent = aiControl.GetAIAgent();
			}
			if (aiAgent)
				aiGroup = SCR_AIGroup.Cast(aiAgent.GetParentGroup());
		}
		
		// 3. Fallback to cached initial AI group
		if (!aiGroup)
			aiGroup = character.PS_GetInitialAIGroup();
		
		return aiGroup;
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
			SCR_AIGroup aiGroup = GetCharacterGroup(character);
			if (!aiGroup)
				return false;
			
			bool groupMatches = aiGroup.GetName().Contains(m_sGroupKey);
			if (!groupMatches && aiGroup.m_BotsGroup)
				groupMatches = aiGroup.m_BotsGroup.GetName().Contains(m_sGroupKey);
			if (!groupMatches && aiGroup.m_PlayersGroup)
				groupMatches = aiGroup.m_PlayersGroup.GetName().Contains(m_sGroupKey);
			
			if (!groupMatches)
				return false;
		}
		
		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!ShouldEntityHaveEffect(ent))
			return false;
		
		// 1. Character filter evaluation
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
		if (character)
		{
			if (m_bAliveOnly || m_sFactionKey != "" || m_sGroupKey != "")
			{
				if (!MatchesCharacterFilter(character))
					return false;
			}
			
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
					return false;
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
							return false;
					}
					else
					{
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
						return false;
				}
			}
			
			return true;
		}
		
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
			AddZoneEffect(character);
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
					AddZoneEffect(occChar);
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnDeactivate(IEntity ent)
	{
		if (m_bDebug)
		{
			PrintDebug(string.Format("OnDeactivate: ent=%1 exited trigger", ent));
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
				if (occChar)
					RemoveZoneEffect(occChar);
			}
		}
	}
}