class PS_PolyZoneHUD : SCR_InfoDisplay
{
	OverlayWidget m_wVignette;
	OverlayWidget m_wScreenBlure;
	VerticalLayoutWidget m_wEffectsVerticalLayout;
	
	ref map<PS_EPolyZoneEffectHUDType, ResourceName> m_mEffectLayouts = new map<PS_EPolyZoneEffectHUDType, ResourceName>();
	ref map<int, PS_PolyZoneEffectHUD> m_mEffects = new map<PS_EPolyZoneEffectHUDType, PS_PolyZoneEffectHUD>();
	
	bool m_bShowVignette = false;
	bool m_bShowScreenBlure = false;
	
	override event void OnStartDraw(IEntity owner)
	{
		super.OnStartDraw(owner);
		
		m_wVignette = OverlayWidget.Cast(m_wRoot.FindAnyWidget("Vignette"));
		m_wScreenBlure = OverlayWidget.Cast(m_wRoot.FindAnyWidget("ScreenBlure"));
		m_wEffectsVerticalLayout = VerticalLayoutWidget.Cast(m_wContent.FindAnyWidget("EffectsVerticalLayout"));
		
		// TODO: config
		m_mEffectLayouts.Insert(PS_EPolyZoneEffectHUDType.RestrictedZone, "{934EEEE4F36CE31E}UI/layouts/HUD/PolyZoneEffects/PolyZoneRestrictedZoneEffect.layout");
		m_mEffectLayouts.Insert(PS_EPolyZoneEffectHUDType.FreezeZone, "{934EEEE4F36CE31E}UI/layouts/HUD/PolyZoneEffects/PolyZoneRestrictedZoneEffect.layout");
		m_mEffectLayouts.Insert(PS_EPolyZoneEffectHUDType.ScreenBlure, "{97EFA3A83FBCB3EF}UI/layouts/HUD/PolyZoneEffects/PolyZoneScreenBlureEffect.layout");
		m_mEffectLayouts.Insert(PS_EPolyZoneEffectHUDType.TriggerCapture, "{08732F612F930F6D}UI/layouts/HUD/PolyZoneEffects/PolyZoneTriggerCaptureEffect.layout");
	}
	
	override protected void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);
		foreach (int id, PS_PolyZoneEffectHUD polyZoneEffectHUD : m_mEffects)
		{
			polyZoneEffectHUD.Update(timeSlice);
		}
		
		if (m_bShowScreenBlure) m_wScreenBlure.SetOpacity(Math.Clamp(m_wScreenBlure.GetOpacity() + timeSlice * 5.0, 0, 1));
		else m_wScreenBlure.SetOpacity(Math.Clamp(m_wScreenBlure.GetOpacity() - timeSlice * 5.0, 0, 1));
		if (m_bShowVignette) m_wVignette.SetOpacity(Math.Clamp(m_wVignette.GetOpacity() + timeSlice * 5.0, 0, 1));
		else m_wVignette.SetOpacity(Math.Clamp(m_wVignette.GetOpacity() - timeSlice * 5.0, 0, 1));
	}
	
	void HideAll()
	{
		foreach (int id, PS_PolyZoneEffectHUD effectHUD : m_mEffects)
		{
			HideEffect(id);
		}
	}
	
	void ShowEffect(PS_EffectContainer effect)
	{
		if (m_mEffects.Contains(effect.m_iId))
			HideEffect(effect.m_iId);
		
		ResourceName effectLayout = m_mEffectLayouts.Get(effect.m_iType);
		Widget effectWidget = GetGame().GetWorkspace().CreateWidgets(effectLayout, m_wEffectsVerticalLayout);
		PS_PolyZoneEffectHUD polyZoneEffectHUD = PS_PolyZoneEffectHUD.Cast(effectWidget.FindHandler(PS_PolyZoneEffectHUD));
		polyZoneEffectHUD.SetString(effect.m_sString);
		polyZoneEffectHUD.SetTime(effect.m_fTime);
		if (!m_bShowVignette) m_bShowVignette = polyZoneEffectHUD.ShowVignette();
		if (!m_bShowScreenBlure) m_bShowScreenBlure = polyZoneEffectHUD.ShowScreenBlure();
		m_mEffects.Insert(effect.m_iId, polyZoneEffectHUD);
	}
	
	void HideEffect(int id)
	{
		if (!m_mEffects.Contains(id))
			return;
		
		m_mEffects.Get(id).GetRootWidget().RemoveFromHierarchy();
		m_mEffects.Remove(id);
		m_bShowVignette = false;
		m_bShowScreenBlure = false;
		foreach (int idT, PS_PolyZoneEffectHUD polyZoneEffectHUD : m_mEffects)
		{
			m_bShowVignette = polyZoneEffectHUD.ShowVignette();
			if (m_bShowVignette) break;
		}
		foreach (int idT, PS_PolyZoneEffectHUD polyZoneEffectHUD : m_mEffects)
		{
			m_bShowScreenBlure = polyZoneEffectHUD.ShowScreenBlure();
			if (m_bShowScreenBlure) break;
		}
	}
}