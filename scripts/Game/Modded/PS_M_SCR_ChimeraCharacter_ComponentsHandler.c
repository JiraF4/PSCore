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
		GetGame().GetCallqueue().Call(PS_LateInit, this);
	}
	
	void PS_LateInit(IEntity owner)
	{
		PS_m_FactionAffiliationComponent = FactionAffiliationComponent.Cast(owner.FindComponent(FactionAffiliationComponent));
		AIControlComponent aiControlComponent = AIControlComponent.Cast(owner.FindComponent(AIControlComponent));
		PS_m_AIAgent = aiControlComponent.GetAIAgent();
	}
}