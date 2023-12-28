[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class PS_PolyZoneHUDControllerClass: ScriptComponentClass
{
};
class PS_PolyZoneHUDController: ScriptComponent
{
	protected PS_PolyZoneHUD m_PolyZoneHUD;
	
	void ShowEffect(int id, PS_EPolyZoneEffectHUDType type, float time)
	{
		Rpc(RPC_ShowEffect, id, type, time);
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RPC_ShowEffect(int id, PS_EPolyZoneEffectHUDType type, float time)
	{
		m_PolyZoneHUD.ShowEffect(id, type, time);
	}
	
	void HideEffect(int id)
	{
		Rpc(RPC_HideEffect, id);
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	void RPC_HideEffect(int id)
	{
		m_PolyZoneHUD.HideEffect(id);
	}
	
	override void OnPostInit(IEntity owner)
	{
		SCR_HUDManagerComponent hudManager = SCR_HUDManagerComponent.Cast(owner.FindComponent(SCR_HUDManagerComponent));
		if (hudManager)
		{
			array<BaseInfoDisplay> infoDisplays = new array<BaseInfoDisplay>;
			int count = hudManager.GetInfoDisplays(infoDisplays);
		
			for(int i = 0; i < count; i++)
       		{
	            if (infoDisplays[i].Type() == PS_PolyZoneHUD)
				{
					m_PolyZoneHUD = PS_PolyZoneHUD.Cast(infoDisplays[i]);
					if (m_PolyZoneHUD)
						break;
				}
       	 	}
		}
	}
}