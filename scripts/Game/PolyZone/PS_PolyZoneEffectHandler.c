[ComponentEditorProps(category: "GameScripted/Character", description: "Add label for observers", color: "0 0 255 255", icon: HYBRID_COMPONENT_ICON)]
class PS_PolyZoneEffectHandlerClass: ScriptComponentClass
{
	
}

[ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
class PS_PolyZoneEffectHandler : ScriptComponent
{
	ref map<PS_PolyZoneTrigger, ref PS_PolyZoneEffect> m_mapPolyZoneEffects = new map<PS_PolyZoneTrigger, ref PS_PolyZoneEffect>();
	
	override void OnPostInit(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.FRAME);
	}
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		foreach (PS_PolyZoneTrigger zone, PS_PolyZoneEffect effect : m_mapPolyZoneEffects)
		{
			effect.OnFrame(this, owner, timeSlice);
		}
	}
	
	void ShowEffect(int id, PS_EPolyZoneEffectHUDType type, float time)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();
		
		int playerId = playerManager.GetPlayerIdFromControlledEntity(GetOwner());
		if (playerId <= 0)
			return;
		
		PlayerController playerController = playerManager.GetPlayerController(playerId);
		if (!playerController)
			return;
			
		PS_PolyZoneHUDController polyZoneHUDController = PS_PolyZoneHUDController.Cast(playerController.FindComponent(PS_PolyZoneHUDController));
		polyZoneHUDController.ShowEffect(id, type, time);
	}
	
	void HideEffect(int id)
	{
		PlayerManager playerManager = GetGame().GetPlayerManager();
		
		int playerId = playerManager.GetPlayerIdFromControlledEntity(GetOwner());
		if (playerId <= 0)
			return;
		
		PlayerController playerController = playerManager.GetPlayerController(playerId);
		if (!playerController)
			return;
			
		PS_PolyZoneHUDController polyZoneHUDController = PS_PolyZoneHUDController.Cast(playerController.FindComponent(PS_PolyZoneHUDController));
		polyZoneHUDController.HideEffect(id);
	}
	
	void AddEffect(PS_PolyZoneTrigger zone, PS_PolyZoneEffect effect)
	{
		if (!Replication.IsServer()) return;
		PS_PolyZoneEffect effectCopy = effect.Copy();
		effectCopy.OnActivate(this, GetOwner());
		m_mapPolyZoneEffects.Insert(zone, effectCopy);
	}
	
	void RemoveEffect(PS_PolyZoneTrigger zone, PS_PolyZoneEffect effect)
	{
		if (!Replication.IsServer()) return;
		if (!m_mapPolyZoneEffects.Contains(zone))
			return;
		m_mapPolyZoneEffects[zone].OnDeactivate(this, GetOwner());
		m_mapPolyZoneEffects.Remove(zone);
	}
}












