[ComponentEditorProps(category: "GameScripted/Character", description: "Add label for observers", color: "0 0 255 255", icon: HYBRID_COMPONENT_ICON)]
class PS_PolyLineClass: ScriptComponentClass
{
};

[ComponentEditorProps(icon: HYBRID_COMPONENT_ICON)]
class PS_PolyLine : ScriptComponent
{
	[Attribute("{B8793707B56B2F9F}UI/Map/PolyMapMarkerBase.layout", params: "layout")]
	protected ResourceName m_sPolyMarkerLayout;
	
	[Attribute("{15A6B5F9E6E1DB8E}UI/layouts/Textures/WarningZoneBorder.edds", UIWidgets.ResourcePickerThumbnail, desc: "", params: "edds")]
	ResourceName m_mPolygonTextureBorder;
	[Attribute("1 1 1 1", UIWidgets.ColorPicker, desc: "")]
	ref Color m_cPolygonBorderColor;
	[Attribute("0.1", UIWidgets.Slider, desc: "", params: "0.001 4 0.01")]
	float m_fPolygonBorderUVScale;
	[Attribute("15", UIWidgets.Slider, desc: "", params: "1 100 0.1")]
	float m_fPolygonBorderWidth;
	
	protected ref SharedItemRef m_TextureBorderSharedItem;
	ShapeEntity m_ePolylineShapeEntity;
	SCR_MapEntity m_MapEntity;
	ref array<float> m_aPoints;
	
	override void OnPostInit(IEntity owner)
	{
		m_MapEntity = SCR_MapEntity.GetMapInstance();
		ScriptInvokerBase<MapConfigurationInvoker> onMapOpen = m_MapEntity.GetOnMapOpen();
		ScriptInvokerBase<MapConfigurationInvoker> onMapClose = m_MapEntity.GetOnMapClose();
		
		onMapOpen.Insert(CreateMapWidget);
		onMapClose.Insert(DeleteMapWidget);
		
		m_ePolylineShapeEntity = ShapeEntity.Cast(owner);
		
		GetGame().GetCallqueue().CallLater(UpdatePolygon, 0, false);
	}
	
	void UpdatePolygon()
	{
		array<vector> outPoints = new array<vector>();
		//m_ePolylineShapeEntity.GenerateTesselatedShape(outPoints);
		m_ePolylineShapeEntity.GetPointsPositions(outPoints);
		vector origin = GetOwner().GetOrigin();
		for (int i = 0; i < outPoints.Count(); i++)
		{
			outPoints[i] = outPoints[i] + origin;
		}
		for (int i = 0; i < outPoints.Count() - 1; i++)
		{
			if ((Math.AbsFloat(outPoints[i][0] - outPoints[i+1][0]) + Math.AbsFloat(outPoints[i][1] - outPoints[i+1][1])) < 0.1)
			{
				outPoints.RemoveOrdered(i);
				i--;
			}
		}
		while (outPoints.Count() > 1 && (outPoints[0] - outPoints[outPoints.Count() - 1]).Length() < 1)
			outPoints.Remove(0);
		m_aPoints = new array<float>();
		SCR_Math2D.Get2DPolygon(outPoints, m_aPoints);
	}
	
	CanvasWidget m_wCanvasWidget;
	protected ref LineDrawCommand m_LinePolygon = new LineDrawCommand();
	protected ref array<ref CanvasWidgetCommand> m_MapDrawCommands = { m_LinePolygon };
	void CreateMapWidget(MapConfiguration mapConfig)
	{
		if (!m_MapEntity)
			m_MapEntity = SCR_MapEntity.GetMapInstance();
		
		// Get map frame
		Widget mapFrame = m_MapEntity.GetMapMenuRoot().FindAnyWidget(SCR_MapConstants.MAP_FRAME_NAME);
		if (!mapFrame) mapFrame = m_MapEntity.GetMapMenuRoot();
		if (!mapFrame) return; // Somethig gone wrong
		
		m_wCanvasWidget = CanvasWidget.Cast(GetGame().GetWorkspace().CreateWidgets(m_sPolyMarkerLayout, mapFrame));
				
		if (m_mPolygonTextureBorder != "")
			m_TextureBorderSharedItem = m_wCanvasWidget.LoadTexture(m_mPolygonTextureBorder);
		else
			m_TextureBorderSharedItem = null;
		
		m_LinePolygon.m_pTexture = m_TextureBorderSharedItem;
		m_LinePolygon.m_UVScale = Vector(1, m_fPolygonBorderUVScale, 0.01);
		m_LinePolygon.m_iColor = m_cPolygonBorderColor.PackToInt();
		m_LinePolygon.m_fWidth = m_fPolygonBorderWidth;
		m_LinePolygon.m_bShouldEnclose = false;
		
		m_wCanvasWidget.SetDrawCommands(m_MapDrawCommands);
		
		SetEventMask(GetOwner(), EntityEvent.POSTFRAME);
	}
	
	void DeleteMapWidget(MapConfiguration mapConfig)
	{
		ClearEventMask(GetOwner(), EntityEvent.POSTFRAME);
	}
	
	override void EOnPostFrame(IEntity owner, float timeSlice)
	{
		m_LinePolygon.m_Vertices = new array<float>();
		float screenXold, screenYold;
		for (int i = 0; i < m_aPoints.Count(); i += 2)
		{
			float screenX, screenY;
			m_MapEntity.WorldToScreen(m_aPoints[i], m_aPoints[i+1], screenX, screenY, true);
			if ((Math.AbsFloat(screenXold - screenX) + Math.AbsFloat(screenYold - screenY)) < 2.1)
			{
				continue;
			}
			screenXold = screenX;
			screenYold = screenY;
			
			m_LinePolygon.m_Vertices.Insert(screenX);
			m_LinePolygon.m_Vertices.Insert(screenY);
		}
	}
}



