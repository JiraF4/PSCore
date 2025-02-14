modded class SCR_ChimeraCharacter 
{
	protected FactionAffiliationComponent PS_m_FactionAffiliationComponent;
	FactionAffiliationComponent PS_GetFactionAffiliationComponent()
	{
		return PS_m_FactionAffiliationComponent;
	}
	protected ChimeraAIControlComponent PS_m_ChimeraAIControlComponent;
	ChimeraAIControlComponent PS_GetChimeraAIControlComponent()
	{
		return PS_m_ChimeraAIControlComponent;
	}
	protected AIAgent PS_m_AIAgent;
	AIAgent PS_GetAIAgent()
	{
		return PS_m_AIAgent;
	}
	
	void SCR_ChimeraCharacter(IEntitySource src, IEntity parent)
	{
		GetGame().GetCallqueue().CallLater(PS_LateInit, 0, false, this);
	}
	
	void PS_LateInit(IEntity owner)
	{
		if (!owner)
			return;
		
		PS_m_FactionAffiliationComponent = FactionAffiliationComponent.Cast(owner.FindComponent(FactionAffiliationComponent));
		PS_m_ChimeraAIControlComponent = ChimeraAIControlComponent.Cast(owner.FindComponent(ChimeraAIControlComponent));
		if (PS_m_ChimeraAIControlComponent)
			PS_m_AIAgent = PS_m_ChimeraAIControlComponent.GetAIAgent();
	}
}