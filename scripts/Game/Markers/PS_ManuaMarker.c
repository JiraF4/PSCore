class PS_ManualMarkerClass : GenericEntityClass
{

}

class PS_ManualMarker : GenericEntity
{
	[Attribute("{52CA8FF5F56C6F31}UI/layouts/Map/ManualMapMarkerBase.layout")]
	protected ResourceName m_sMarkerPrefab;
	[Attribute("{D17288006833490F}UI/Textures/Icons/icons_wrapperUI-32.imageset")]
	protected ResourceName m_sImageSet;
	[Attribute("")]
	protected ResourceName m_sImageSetGlow;
	[Attribute("1 1 1 1", UIWidgets.ColorPicker)]
	ref Color m_MarkerColor;
	[Attribute("empty")]
	protected string m_sQuadName;
	[Attribute("5.0")]
	protected float m_fWorldSize;
	[Attribute("")]
	protected string m_sDescription;
	[Attribute("true")]
	bool m_bUseWorldScale;
	
	string GetImageSet()
	{
		return m_sImageSet;
	}
	void SetImageSet(string imageSet)
	{
		m_sImageSet = imageSet;
	}
	
	string GetImageSetGlow()
	{
		return m_sImageSetGlow;
	}
	void SetImageSetGlow(string imageSetGlow)
	{
		m_sImageSetGlow = imageSetGlow;
	}
	
	Color GetColor()
	{
		return m_MarkerColor;
	}
	void SetColor(Color color)
	{
		m_MarkerColor = color;
	}
	
	string GetQuadName()
	{
		return m_sQuadName;
	}
	void SetQuadName(string quadName)
	{
		m_sQuadName = quadName;
	}
	
	float GetSize()
	{
		return m_fWorldSize;
	}
	void SetSize(float worldSize)
	{
		m_fWorldSize = worldSize;
	}
	
	string GetDescription()
	{
		return m_sDescription;
	}
	void SetDescription(string description)
	{
		m_sDescription = description;
	}
	
	bool GetUseWorldScale()
	{
		return m_bUseWorldScale;
	}
	void SetUseWorldScale(bool useWorldScale)
	{
		m_bUseWorldScale = useWorldScale;
	}
	
	
	
	Widget m_wRoot;
	SCR_MapEntity m_MapEntity;
	PS_ManualMarkerComponent m_hManualMarkerComponent;
	override protected void EOnPostFrame(IEntity owner, float timeSlice)
	{
		if (!m_wRoot)
			return;
		
		// Get screen position
		float wX, wY, screenX, screenY, screenXEnd, screenYEnd;
		vector worldPosition = GetOrigin();
		wX = worldPosition[0];
		wY = worldPosition[2];
		m_MapEntity.WorldToScreen(wX, wY, screenX, screenY, true);
		m_MapEntity.WorldToScreen(wX + m_fWorldSize, wY + m_fWorldSize, screenXEnd, screenYEnd, true);
		
		float screenXD = GetGame().GetWorkspace().DPIUnscale(screenX);
		float screenYD = GetGame().GetWorkspace().DPIUnscale(screenY);
		float sizeXD = GetGame().GetWorkspace().DPIUnscale(screenXEnd - screenX);
		float sizeYD = GetGame().GetWorkspace().DPIUnscale(screenY - screenYEnd); // Y flip
		if (!m_bUseWorldScale)
		{
			sizeXD = m_fWorldSize;
			sizeYD = m_fWorldSize;
		}
		
		m_hManualMarkerComponent.SetSlot(screenXD, screenYD, sizeXD, sizeYD);
	}
	
	//
	override protected void EOnInit(IEntity owner)
	{
		m_MapEntity = SCR_MapEntity.GetMapInstance();
		ScriptInvokerBase<MapConfigurationInvoker> onMapOpen = m_MapEntity.GetOnMapOpen();
		ScriptInvokerBase<MapConfigurationInvoker> onMapClose = m_MapEntity.GetOnMapClose();
		
		onMapOpen.Insert(CreateMapWidget);
		onMapClose.Insert(DeleteMapWidget);
		//GetGame().GetCallqueue().CallLater(CreateMap, 1000);
	}
	
	void CreateMapWidget(MapConfiguration mapConfig)
	{
		Widget mapFrame = m_MapEntity.GetMapMenuRoot().FindAnyWidget(SCR_MapConstants.MAP_FRAME_NAME);
		if (!mapFrame) return;
		m_wRoot = Widget.Cast(GetGame().GetWorkspace().CreateWidgets(m_sMarkerPrefab, mapFrame));
		m_hManualMarkerComponent = PS_ManualMarkerComponent.Cast(m_wRoot.FindHandler(PS_ManualMarkerComponent));
		m_hManualMarkerComponent.SetImage(m_sImageSet, m_sQuadName);
		m_hManualMarkerComponent.SetImageGlow(m_sImageSetGlow, m_sQuadName);
		m_hManualMarkerComponent.SetDescription(m_sDescription);
		m_hManualMarkerComponent.SetColor(m_MarkerColor);
		m_hManualMarkerComponent.OnMouseLeave(null, null, 0, 0);
		SetEventMask(EntityEvent.POSTFRAME);
	}
	
	void DeleteMapWidget(MapConfiguration mapConfig)
	{
		m_wRoot = null;
		m_hManualMarkerComponent = null;
		ClearEventMask(EntityEvent.POSTFRAME);
	}
	
	void PS_ManualMarker(IEntitySource src, IEntity parent)
	{
		SetEventMask(EntityEvent.INIT);
	}
	
	override bool RplSave(ScriptBitWriter writer)
	{
		writer.WriteString(m_sDescription);
		
		return true;
	}
	override bool RplLoad(ScriptBitReader reader)
	{
		reader.ReadString(m_sDescription);
		
		
		return true;
	}
}