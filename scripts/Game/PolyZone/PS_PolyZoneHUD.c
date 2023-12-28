class PS_PolyZoneHUD : SCR_InfoDisplay
{
	OverlayWidget m_wVignette;
	VerticalLayoutWidget m_wEffectsVerticalLayout;
	
	ref map<PS_EPolyZoneEffectHUDType, ResourceName> m_mEffectLayouts = new map<PS_EPolyZoneEffectHUDType, ResourceName>();
	ref map<int, PS_PolyZoneEffectHUD> m_mEffects = new map<PS_EPolyZoneEffectHUDType, PS_PolyZoneEffectHUD>();
	
	bool m_bShowVignette = false;
	
	override event void OnStartDraw(IEntity owner)
	{
		super.OnStartDraw(owner);
		
		m_wVignette = OverlayWidget.Cast(m_wRoot.FindAnyWidget("Vignette"));
		m_wEffectsVerticalLayout = VerticalLayoutWidget.Cast(m_wContent.FindAnyWidget("EffectsVerticalLayout"));
		
		// TODO: config
		m_mEffectLayouts.Insert(PS_EPolyZoneEffectHUDType.RestrictedZone, "{934EEEE4F36CE31E}UI/layouts/HUD/PolyZoneEffects/PolyZoneRestrictedZoneEffect.layout");
	}
	
	override protected void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);
		foreach (int id, PS_PolyZoneEffectHUD polyZoneEffectHUD : m_mEffects)
		{
			polyZoneEffectHUD.Update(timeSlice);
		}
		
		if (m_bShowVignette) m_wVignette.SetOpacity(Math.Clamp(m_wVignette.GetOpacity() + timeSlice * 5.0, 0, 1));
		else m_wVignette.SetOpacity(Math.Clamp(m_wVignette.GetOpacity() - timeSlice * 5.0, 0, 1));
	}
	
	void ShowEffect(int id, PS_EPolyZoneEffectHUDType type, float time)
	{
		if (m_mEffects.Contains(id))
			HideEffect(id);
		
		ResourceName effectLayout = m_mEffectLayouts.Get(type);
		Widget effectWidget = GetGame().GetWorkspace().CreateWidgets(effectLayout, m_wEffectsVerticalLayout);
		PS_PolyZoneEffectHUD polyZoneEffectHUD = PS_PolyZoneEffectHUD.Cast(effectWidget.FindHandler(PS_PolyZoneEffectHUD));
		polyZoneEffectHUD.SetTime(time);
		if (!m_bShowVignette) m_bShowVignette = polyZoneEffectHUD.ShowVignette();
		m_mEffects.Insert(id, polyZoneEffectHUD);
	}
	
	void HideEffect(int id)
	{
		m_mEffects.Get(id).GetRootWidget().RemoveFromHierarchy();
		m_mEffects.Remove(id);
		m_bShowVignette = false;
		foreach (int idT, PS_PolyZoneEffectHUD polyZoneEffectHUD : m_mEffects)
		{
			m_bShowVignette = polyZoneEffectHUD.ShowVignette();
			if (m_bShowVignette) break;
		}
	}
}