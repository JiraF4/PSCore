[BaseContainerProps()]
class PS_PolyZoneEffectScreenBlure : PS_PolyZoneEffect
{
	override void OnActivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		
	}
	
	override void OnDeactivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		
	}
	
	override PS_EffectContainer GetEffectContainer()
	{
		PS_EffectContainer effect = new PS_EffectContainer();
		effect.m_iId = m_iId;
		effect.m_fTime = 10000;
		effect.m_iType = PS_EPolyZoneEffectHUDType.ScreenBlure;
		return effect;
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