// Custom marker widget component
class PS_ManualMarkerComponent : SCR_ScriptedWidgetComponent
{
	protected SCR_MapEntity m_MapEntity;
	
	// Cache widgets
	protected ImageWidget m_wMarkerIcon;
	protected ImageWidget m_wMarkerIconGlow;
	protected FrameWidget m_wMarkerFrame;
	protected RichTextWidget m_wDescriptionText;
	protected PanelWidget m_wDescriptionPanel;
	protected ScrollLayoutWidget m_wMarkerScrollLayout;
	
	// Internal variables
	protected string m_sDescription;
	protected bool m_bHasGlow;
	
	// Cache every used widget after attaching to widget tree
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_MapEntity = SCR_MapEntity.GetMapInstance();
		m_wMarkerIcon = ImageWidget.Cast(w.FindAnyWidget("MarkerIcon"));
		m_wMarkerIconGlow = ImageWidget.Cast(w.FindAnyWidget("MarkerGlowIcon"));
		m_wMarkerFrame = FrameWidget.Cast(w.FindAnyWidget("MarkerFrame"));
		m_wDescriptionText = RichTextWidget.Cast(w.FindAnyWidget("DescriptionText"));
		m_wMarkerScrollLayout = ScrollLayoutWidget.Cast(w.FindAnyWidget("MarkerScrollLayout"));
		m_wDescriptionPanel = PanelWidget.Cast(w.FindAnyWidget("DescriptionPanel"));
	}
	
	float GetYScale()
	{
		int x, y;
		m_wMarkerIcon.GetImageSize(0, x, y);
		if (y == 0) y = 1;
		float scale = (float) y / (float) x;
		return scale;
	}
	
	// Every info contains in PS_ManualMarker, soo ther is onle setters
	void SetImage(ResourceName m_sImageSet, string quadName)
	{
		m_wMarkerIcon.LoadImageFromSet(0, m_sImageSet, quadName);
	}
	void SetImageGlow(ResourceName m_sImageSet, string quadName)
	{
		if (m_sImageSet != "") m_bHasGlow = true;
		m_wMarkerIconGlow.LoadImageFromSet(0, m_sImageSet, quadName);
	}
	void SetDescription(string description)
	{
		m_sDescription = description;
		
		m_wDescriptionText.SetText(description);
	}
	void SetColor(Color color)
	{
		m_wMarkerIcon.SetColor(color);	
		m_wMarkerIconGlow.SetColor(color);	
	}
	
	// Update marker "Transform", called every frame
	void SetSlot(float posX, float posY, float sizeX, float sizeY, float rotation)
	{
		FrameSlot.SetPos(m_wRoot, posX, posY);
		
		FrameSlot.SetPos(m_wMarkerIcon, -sizeX/2, -sizeY/2);
		FrameSlot.SetSize(m_wMarkerIcon, sizeX, sizeY);
		FrameSlot.SetPos(m_wMarkerIconGlow, -sizeX/2, -sizeY/2);
		FrameSlot.SetSize(m_wMarkerIconGlow, sizeX, sizeY);
		FrameSlot.SetPos(m_wMarkerScrollLayout, -sizeX/2, -sizeY/2);
		FrameSlot.SetSize(m_wMarkerScrollLayout, sizeX, sizeY);
		
		float panelX, panelY;
		m_wDescriptionPanel.GetScreenSize(panelX, panelY);
		float panelXD = GetGame().GetWorkspace().DPIUnscale(panelX);
		float panelYD = GetGame().GetWorkspace().DPIUnscale(panelY);
		FrameSlot.SetPos(m_wDescriptionPanel, -panelXD/2, -panelYD/2);
		m_wMarkerIcon.SetRotation(rotation);
		m_wMarkerIconGlow.SetRotation(rotation);
	}
	
	// Show/Hide description box
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (m_bHasGlow) m_wMarkerIconGlow.SetVisible(true);
		if (m_sDescription != "") m_wDescriptionPanel.SetVisible(true);
		
		return true;
	}
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_wMarkerIconGlow.SetVisible(false);	
		m_wDescriptionPanel.SetVisible(false);
		
		return true;
	}
};