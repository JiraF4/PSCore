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
	
	override void OnInit(IEntity owner)
	{
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
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!ent)
			return false;
			
		if (!m_polyZone)
		{
			LogFilterState(ent, "ALLOWED_NO_POLYZONE", "m_polyZone is null");
			return true;
		}
		if (!m_polyZone.IsInsidePolygon(ent.GetOrigin()))
		{
			LogFilterState(ent, "OUTSIDE_POLYGON", string.Format("pos: %1", ent.GetOrigin()));
			return false;
		}
		
		if (m_bAliveOnly || m_sFactionKey != "" || m_sGroupKey != "")
		{
			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
			if (m_sGroupKey != "" && !character)
			{
				LogFilterState(ent, "REJECTED_NOT_CHARACTER", string.Format("groupKey '%1' requires character", m_sGroupKey));
				return false;
			}
			
			Vehicle vehicle = Vehicle.Cast(ent);
			SCR_DamageManagerComponent damageManager;
			FactionAffiliationComponent factionAffiliation;
			SCR_AIGroup aiGroup;
			
			if (vehicle)
			{
				damageManager = vehicle.GetDamageManager();
				factionAffiliation = vehicle.GetFactionAffiliation();
			}
			
			if (character)
			{
				damageManager = character.GetDamageManager();
				factionAffiliation = character.PS_GetFactionAffiliationComponent();
				if (!factionAffiliation)
					factionAffiliation = FactionAffiliationComponent.Cast(character.FindComponent(FactionAffiliationComponent));
				AIAgent aiAgent = character.PS_GetAIAgent();
				if (aiAgent)
					aiGroup = SCR_AIGroup.Cast(aiAgent.GetParentGroup());
				if (aiGroup)
					aiGroup = aiGroup.m_BotsGroup;
			}
			
			if (m_bAliveOnly)
			{
				if (!damageManager)
					damageManager = SCR_DamageManagerComponent.Cast(ent.FindComponent(SCR_DamageManagerComponent));
				
				if (!damageManager || damageManager.GetState() == EDamageState.DESTROYED)
				{
					string dmgState = "null";
					if (damageManager)
						dmgState = damageManager.GetState().ToString();
					LogFilterState(ent, "REJECTED_DEAD", string.Format("damageManager=%1, state=%2", damageManager, dmgState));
					return false;
				}
			}
			
			if (m_sFactionKey != "")
			{
				if (!factionAffiliation)
				{
					LogFilterState(ent, "REJECTED_FACTION", string.Format("no faction affiliation, expected '%1'", m_sFactionKey));
					return false;
				}
				Faction defaultFaction = factionAffiliation.GetDefaultAffiliatedFaction();
				if (!defaultFaction || defaultFaction.GetFactionKey() != m_sFactionKey)
				{
					string currentFactionKey = "none";
					if (defaultFaction)
						currentFactionKey = defaultFaction.GetFactionKey();
					LogFilterState(ent, "REJECTED_FACTION", string.Format("expected '%1', got '%2'", m_sFactionKey, currentFactionKey));
					return false;
				}
			}
			
			if (m_sGroupKey != "")
			{
				if (!aiGroup)
				{
					LogFilterState(ent, "REJECTED_GROUP", string.Format("no AI group, expected '%1'", m_sGroupKey));
					return false;
				}
				if (!aiGroup.GetName().Contains(m_sGroupKey))
				{
					LogFilterState(ent, "REJECTED_GROUP", string.Format("group '%1' does not contain '%2'", aiGroup.GetName(), m_sGroupKey));
					return false;
				}
			}
		}
		
		LogFilterState(ent, "PASSED");
		return true;
	}
	
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
		
		PS_PolyZoneEffectHandler polyZoneEffectHandler = PS_PolyZoneEffectHandler.Cast(ent.FindComponent(PS_PolyZoneEffectHandler));
		if (!polyZoneEffectHandler)
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnActivate: ent=%1 ignored (PS_PolyZoneEffectHandler component not found)", ent));
			return;
		}
		
		if (m_bReversed)
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnActivate: Removing effect '%1' from ent=%2 (reversed=true)", m_polyZoneEffect.ClassName(), ent));
			polyZoneEffectHandler.RemoveEffect(this, m_polyZoneEffect);
		}
		else
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnActivate: Adding effect '%1' to ent=%2 (reversed=false)", m_polyZoneEffect.ClassName(), ent));
			polyZoneEffectHandler.AddEffect(this, m_polyZoneEffect);
		}
	}
	
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
		
		PS_PolyZoneEffectHandler polyZoneEffectHandler = PS_PolyZoneEffectHandler.Cast(ent.FindComponent(PS_PolyZoneEffectHandler));
		if (!polyZoneEffectHandler)
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnDeactivate: ent=%1 ignored (PS_PolyZoneEffectHandler component not found)", ent));
			return;
		}
		
		if (m_bReversed)
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnDeactivate: Adding effect '%1' to ent=%2 (reversed=true)", m_polyZoneEffect.ClassName(), ent));
			polyZoneEffectHandler.AddEffect(this, m_polyZoneEffect);
		}
		else
		{
			if (m_bDebug)
				PrintDebug(string.Format("OnDeactivate: Removing effect '%1' from ent=%2 (reversed=false)", m_polyZoneEffect.ClassName(), ent));
			polyZoneEffectHandler.RemoveEffect(this, m_polyZoneEffect);
		}
	}
}