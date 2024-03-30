[BaseContainerProps()]
class PS_PolyZoneEffect
{
	static int m_iLastId;
	int m_iId;
	
	void OnFrame(PS_PolyZoneEffectHandler handler, IEntity ent, float timeSlice)
	{
		
	}
	
	void OnActivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		
	}
	
	void OnDeactivate(PS_PolyZoneEffectHandler handler, IEntity ent)
	{
		
	}
	
	PS_PolyZoneEffect CreateCopyObject()
	{
		return new PS_PolyZoneEffect();
	}
	
	void CopyFields(PS_PolyZoneEffect effect)
	{
		
	}
	
	PS_EffectContainer GetEffectContainer()
	{
		PS_EffectContainer effect = new PS_EffectContainer();
		return effect;
	}
	
	PS_PolyZoneEffect Copy()
	{
		PS_PolyZoneEffect copy = CreateCopyObject();
		CopyFields(copy);
		return copy;	
	}
	
	void PS_PolyZoneEffect()
	{
		m_iLastId++;
		m_iId = m_iLastId;
	}
}
class PS_EffectContainer
{
	int m_iId;
	PS_EPolyZoneEffectHUDType m_iType;
	float m_fTime;
	string m_sString;
}