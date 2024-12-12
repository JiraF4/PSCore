modded class SCR_ChimeraCharacter 
{
	protected FactionAffiliationComponent PS_m_FactionAffiliationComponent;
	FactionAffiliationComponent PS_GetFactionAffiliationComponent()
		return PS_m_FactionAffiliationComponent;
	protected AIAgent PS_m_AIAgent;
	AIAgent PS_GetAIAgent()
		return PS_m_AIAgent;
	
	void SCR_ChimeraCharacter(IEntitySource src, IEntity parent)
	{
		GetGame().GetCallqueue().CallLater(PS_LateInit, 100, false, this);
	}
	
	void PS_LateInit(IEntity owner)
	{
		if (!owner)
			return;
		
		PS_m_FactionAffiliationComponent = FactionAffiliationComponent.Cast(owner.FindComponent(FactionAffiliationComponent));
		ChimeraAIControlComponent aiControlComponent = ChimeraAIControlComponent.Cast(owner.FindComponent(ChimeraAIControlComponent));
		if (aiControlComponent)
			PS_m_AIAgent = aiControlComponent.GetAIAgent();
	}
}