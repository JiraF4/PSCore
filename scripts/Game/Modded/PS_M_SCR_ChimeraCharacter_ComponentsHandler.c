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
	protected SCR_AIGroup PS_m_InitialAIGroup;
	AIAgent PS_GetAIAgent()
	{
		return PS_m_AIAgent;
	}
	SCR_AIGroup PS_GetInitialAIGroup()
	{
		return PS_m_InitialAIGroup;
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
		if (PS_m_AIAgent)
			PS_m_InitialAIGroup = SCR_AIGroup.Cast(PS_m_AIAgent.GetParentGroup());
	}
}