[BaseContainerProps()]
class PS_PolyZoneEffectRestricted : PS_PolyZoneEffect
{
	[Attribute("10")]
	float m_fKillTime;
	
	override void OnFrame(PS_PolyZoneEffectHandler handler, IEntity ent, float timeSlice)
	{
		m_fKillTime -= timeSlice;
	}
	
	override void OnActivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		handler.ShowEffect(m_iId, PS_EPolyZoneEffectHUDType.RestrictedZone, m_fKillTime);
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