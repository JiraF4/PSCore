[ComponentEditorProps(category: "GameScripted/Character", description: "Add label for observers", color: "0 0 255 255", icon: HYBRID_COMPONENT_ICON)]
class PS_PolyMarkerClass: ScriptComponentClass
{
};

[ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
class PS_PolyMarker : ScriptComponent
{
	PolylineShapeEntity m_ePolylineShapeEntity;
	SCR_MapEntity m_MapEntity;
	
	protected ResourceName m_sPolyMarkerPrefab = "{B8793707B56B2F9F}UI/Map/PolyMapMarkerBase.layout";
	protected ref SharedItemRef m_TextureSharedItem;
	protected ref SharedItemRef m_TextureBorderSharedItem;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, desc: "", params: "edds")]
	ResourceName m_mPolygonTexture;
	[Attribute("", UIWidgets.ResourcePickerThumbnail, desc: "", params: "edds")]
	ResourceName m_mPolygonTextureBorder;
	
	override void OnPostInit(IEntity owner)
	{
		m_MapEntity = SCR_MapEntity.GetMapInstance();
		ScriptInvokerBase<MapConfigurationInvoker> onMapOpen = m_MapEntity.GetOnMapOpen();
		ScriptInvokerBase<MapConfigurationInvoker> onMapClose = m_MapEntity.GetOnMapClose();
		
		onMapOpen.Insert(CreateMapWidget);
		onMapClose.Insert(DeleteMapWidget);
		
		m_ePolylineShapeEntity = PolylineShapeEntity.Cast(owner);
	}
	
	CanvasWidget m_wCanvasWidget;
	protected ref PolygonDrawCommand m_DrawPolygon = new PolygonDrawCommand();
	protected ref LineDrawCommand m_LinePolygon = new LineDrawCommand();
	protected ref array<ref CanvasWidgetCommand> m_MapDrawCommands = { m_DrawPolygon, m_LinePolygon };
	void CreateMapWidget(MapConfiguration mapConfig)
	{
		if (!m_MapEntity)
			m_MapEntity = SCR_MapEntity.GetMapInstance();
		
		// Get map frame
		Widget mapFrame = m_MapEntity.GetMapMenuRoot().FindAnyWidget(SCR_MapConstants.MAP_FRAME_NAME);
		if (!mapFrame) mapFrame = m_MapEntity.GetMapMenuRoot();
		if (!mapFrame) return; // Somethig gone wrong
		
		m_wCanvasWidget = CanvasWidget.Cast(GetGame().GetWorkspace().CreateWidgets(m_sPolyMarkerPrefab, mapFrame));
				
		if (!m_TextureSharedItem)
		{
			m_TextureSharedItem = m_wCanvasWidget.LoadTexture(m_mPolygonTexture);
			if (!m_TextureSharedItem.IsValid())
				return;
		}	
		
		if (!m_TextureBorderSharedItem)
		{
			m_TextureBorderSharedItem = m_wCanvasWidget.LoadTexture(m_mPolygonTextureBorder);
			if (!m_TextureBorderSharedItem.IsValid())
				return;
		}
		
		m_DrawPolygon.m_pTexture = m_TextureSharedItem;
		m_DrawPolygon.m_fUVScale = 0.01;
		m_DrawPolygon.m_iColor = Color.WHITE;
		
		m_LinePolygon.m_pTexture = m_TextureBorderSharedItem;
		m_LinePolygon.m_UVScale = Vector(1, 0.1, 0.01);
		m_LinePolygon.m_iColor = Color.FromInt(0xffef961c);
		m_LinePolygon.m_iColor = Color.WHITE;
		m_LinePolygon.m_fWidth = 16.0;
		m_LinePolygon.m_bShouldEnclose = true;
		
		m_wCanvasWidget.SetDrawCommands(m_MapDrawCommands);
		GetGame().GetCallqueue().CallLater(Update, 0, true);
	}
	
	void DeleteMapWidget(MapConfiguration mapConfig)
	{
		GetGame().GetCallqueue().Remove(Update);
	}
	
	void Update()
	{
		
		array<vector> outPoints = new array<vector>();
		m_ePolylineShapeEntity.GetPointsPositions(outPoints);
		
		vector origin = GetOwner().GetOrigin();
		m_DrawPolygon.m_Vertices = new array<float>();
		m_LinePolygon.m_Vertices = new array<float>();
		foreach (vector point : outPoints)
		{
			point += origin;
			float screenX, screenY;
			m_MapEntity.WorldToScreen(point[0], point[2], screenX, screenY, true);
			m_DrawPolygon.m_Vertices.Insert(screenX);
			m_DrawPolygon.m_Vertices.Insert(screenY);
			m_LinePolygon.m_Vertices.Insert(screenX);
			m_LinePolygon.m_Vertices.Insert(screenY);
		}
	}
}



