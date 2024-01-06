[BaseContainerProps()]
class PS_PolyZoneEffectScreenBlure : PS_PolyZoneEffect
{
	override void OnActivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		handler.ShowEffect(m_iId, PS_EPolyZoneEffectHUDType.ScreenBlure, 1000000.0);
	}
	
	override void OnDeactivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		handler.HideEffect(m_iId);
	}
	
	override PS_PolyZoneEffect CreateCopyObject()
	{
		return new PS_PolyZoneEffectScreenBlure();
	}
	
	override void CopyFields(PS_PolyZoneEffect effect)
	{
		PS_PolyZoneEffectScreenBlure effectCurrent = PS_PolyZoneEffectScreenBlure.Cast(effect);
	}
}