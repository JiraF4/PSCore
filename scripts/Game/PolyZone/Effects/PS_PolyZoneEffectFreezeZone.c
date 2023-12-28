[BaseContainerProps()]
class PS_PolyZoneEffectFreezeZone : PS_PolyZoneEffect
{
	[Attribute("10")]
	float m_fKillTime;
	
	bool m_bTriggerd = false;
	bool m_bOutside = false;
	
	override void OnFrame(PS_PolyZoneEffectHandler handler, IEntity ent, float timeSlice)
	{
		m_fKillTime -= timeSlice;
		if (m_fKillTime <= 0 && !m_bTriggerd)
		{
			DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
			if (!damageManager || damageManager.GetState() == EDamageState.DESTROYED)
				return;
			damageManager.SetHealthScaled(0);
		}
	}
	
	override void OnActivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		handler.ShowEffect(m_iId, PS_EPolyZoneEffectHUDType.FreezeZone, m_fKillTime);
	}
	
	override void OnDeactivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		handler.HideEffect(m_iId);
	}
	
	override PS_PolyZoneEffect CreateCopyObject()
	{
		return new PS_PolyZoneEffectRestricted();
	}
	
	override void CopyFields(PS_PolyZoneEffect effect)
	{
		PS_PolyZoneEffectRestricted effectCurrent = PS_PolyZoneEffectRestricted.Cast(effect);
		effectCurrent.m_fKillTime = m_fKillTime;
	}
}