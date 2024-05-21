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
	
	override void OnInit(IEntity owner)
	{
		m_polyZone = PS_PolyZone.Cast(owner.GetParent().FindComponent(PS_PolyZone));
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent)
	{
		if (!m_polyZone)
			return true;
		if (!m_polyZone.IsInsidePolygon(ent.GetOrigin()))
			return false;
		
		if (m_bAliveOnly || m_sFactionKey != "" || m_sGroupKey != "")
		{
			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(ent);
			if (m_sGroupKey != "" && !character)
				return false;
			
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
				AIAgent aiAgent = character.PS_GetAIAgent();
				if (aiAgent)
					aiGroup = SCR_AIGroup.Cast(aiAgent.GetParentGroup());
				if (aiGroup)
					aiGroup = aiGroup.GetSlave();
			}
			
			if (m_bAliveOnly)
				damageManager = SCR_DamageManagerComponent.Cast(ent.FindComponent(SCR_DamageManagerComponent));
			
			if (m_bAliveOnly && damageManager.GetState() == EDamageState.DESTROYED)
				return false;
			if (m_sFactionKey != "" && factionAffiliation.GetDefaultAffiliatedFaction().GetFactionKey() != m_sFactionKey)
				return false;
			if (m_sGroupKey != "")
			{
				if (!aiGroup)
					return false;
				if (!aiGroup.GetName().Contains(m_sGroupKey))
					return false;
			}
		}
		
		return true;
	}
	
	override protected void OnActivate(IEntity ent)
	{
		if (!m_polyZoneEffect)
			return;
		
		PS_PolyZoneEffectHandler polyZoneEffectHandler = PS_PolyZoneEffectHandler.Cast(ent.FindComponent(PS_PolyZoneEffectHandler));
		if (!polyZoneEffectHandler)
			return;
		
		if (m_bReversed)
			polyZoneEffectHandler.RemoveEffect(this, m_polyZoneEffect);
		else
			polyZoneEffectHandler.AddEffect(this, m_polyZoneEffect);
	}
	
	override protected void OnDeactivate(IEntity ent)
	{
		if (!m_polyZoneEffect)
			return;
		
		PS_PolyZoneEffectHandler polyZoneEffectHandler = PS_PolyZoneEffectHandler.Cast(ent.FindComponent(PS_PolyZoneEffectHandler));
		if (!polyZoneEffectHandler)
			return;
		
		if (m_bReversed)
			polyZoneEffectHandler.AddEffect(this, m_polyZoneEffect);
		else
			polyZoneEffectHandler.RemoveEffect(this, m_polyZoneEffect);
	}
}