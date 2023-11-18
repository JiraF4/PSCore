class PS_MapIconListComponent : SCR_ChangeableComponentBase
{
	[Attribute("#AR-MapMarker_ParentCategory", UIWidgets.Auto, "Menu category name" )]
	protected string m_sCategoryName;
	
	[Attribute("{2EFEA2AF1F38E7F0}UI/Textures/Icons/icons_wrapperUI-64.imageset", UIWidgets.ResourceNamePicker, desc: "Icons imageset", params: "imageset" )]
	protected ResourceName m_sIconImageset;
	
	[Attribute("{DEA2D3B788CDCB4F}UI/layouts/Map/MapIconSelectorEntry.layout", UIWidgets.ResourceNamePicker, desc: "Icon selection entry layout", params: "layout" )]
	protected ResourceName m_sSelectorIconEntry;
	
	[Attribute("scenarios", UIWidgets.Auto, "Category icon quad" )]
	protected string m_sCategoryIconName;
	
	[Attribute("20", UIWidgets.Auto, "Icon selector entries per line" )]
	protected int m_iIconsPerLine;
	
	protected SCR_MapMarkerEntryPlaced m_PlacedMarkerConfig;
	
	protected const string ICON_SELECTOR = "IconSelector";
	protected const ResourceName SELECTOR_LINE = "{CF8EC7A0D310A8D9}UI/layouts/Map/MapColorSelectorLine.layout";
	protected const string ICON_ENTRY = "IconEntry";
	
	protected ref Color BACKGROUND_DEFAULT = new Color(4,4,4,255);
	protected ref Color BACKGROUND_SELECTED = new Color(16,16,16,255);
	
	protected ref map<SCR_ButtonBaseComponent, int> m_mIconIDs = new map<SCR_ButtonBaseComponent, int>();
	protected SCR_ButtonBaseComponent m_SelectedIconButton;
	protected int m_iIconEntryCount;
	protected int m_iIconLines;
	protected int m_iWantedIconEntry;
	protected int m_iSelectedIconID;
	protected Widget m_IconSelector;
	protected SCR_TabViewComponent m_TabComponent;
	
	protected ImageWidget m_SelectedIconImage;
	protected ImageWidget m_SelectedIconImageGlow;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		SCR_MapMarkerManagerComponent markerMgr = SCR_MapMarkerManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_MapMarkerManagerComponent));
		SCR_MapMarkerConfig markerConfig = markerMgr.GetMarkerConfig();
		if (!markerConfig)
			return;
		
		m_PlacedMarkerConfig = SCR_MapMarkerEntryPlaced.Cast(markerConfig.GetMarkerEntryConfigByType(SCR_EMapMarkerType.PLACED_CUSTOM));
		
		
		Widget categoryTab = m_wRoot.FindAnyWidget("MarkerEditTab");
		m_TabComponent = SCR_TabViewComponent.Cast(categoryTab.FindHandler(SCR_TabViewComponent));
		
		array<ref SCR_MarkerIconCategory> categoriesArr = m_PlacedMarkerConfig.GetIconCategories();
		foreach (SCR_MarkerIconCategory category : categoriesArr)
		{
			m_TabComponent.AddTab(string.Empty, category.m_sName, identifier: category.m_sIdentifier);
		}
		
		m_SelectedIconImage = ImageWidget.Cast(m_wRoot.FindAnyWidget("SelectedIconImage"));
		m_SelectedIconImageGlow = ImageWidget.Cast(m_wRoot.FindAnyWidget("SelectedIconImageGlow"));
		
		m_TabComponent.m_OnChanged.Insert(OnTabChanged);
		m_TabComponent.ShowTab(0, true, false);
	}
	
	protected void InitCategoryIcons(SCR_TabViewContent tabContent)
	{
		m_mIconIDs.Clear();
		m_SelectedIconButton = null;
		
		string imageset, imagesetGlow, quad;
		
		m_IconSelector = m_wRoot.FindAnyWidget(ICON_SELECTOR);		
		array<ref SCR_MarkerIconEntry> iconsArr = m_PlacedMarkerConfig.GetIconEntries();
				
		Widget child = m_IconSelector.GetChildren();
		while (child)
		{
			child.RemoveFromHierarchy();
			child = m_IconSelector.GetChildren();
		}
		
		Widget iconSelectorLine = GetGame().GetWorkspace().CreateWidgets(SELECTOR_LINE, m_IconSelector);
		m_iIconEntryCount = 0;
		m_iIconLines = 1;
				
		SCR_ButtonImageComponent firstEntry;
		
		foreach (int i, SCR_MarkerIconEntry iconEntry : iconsArr)
		{
			if (iconEntry.m_sCategoryIdentifier != tabContent.m_sTabIdentifier)
				continue;
			
			m_iIconEntryCount++;
			if (m_iIconEntryCount > m_iIconsPerLine * m_iIconLines)
			{
				iconSelectorLine = GetGame().GetWorkspace().CreateWidgets(SELECTOR_LINE, m_IconSelector);
				m_iIconLines++;
			}

			Widget button = GetGame().GetWorkspace().CreateWidgets(m_sSelectorIconEntry, iconSelectorLine);
			button.SetName(ICON_ENTRY + m_iIconEntryCount.ToString());
			SCR_ButtonImageComponent buttonComp = SCR_ButtonImageComponent.Cast(button.FindHandler(SCR_ButtonImageComponent));
			m_mIconIDs.Insert(buttonComp, i);
			if (!firstEntry)
				firstEntry = buttonComp;
			
			iconEntry.GetIconResource(imageset, imagesetGlow, quad);
			buttonComp.SetImage(imageset, quad);
			buttonComp.m_OnClicked.Insert(OnIconEntryClicked);
			buttonComp.m_OnFocus.Insert(OnIconEntryFocused);
		}
		
		if (m_iWantedIconEntry == -1)
		{
			OnIconEntryClicked(firstEntry);
		}
		else
		{
			SCR_ButtonBaseComponent buttonComp = m_mIconIDs.GetKeyByValue(m_iWantedIconEntry);
			if (buttonComp)
				OnIconEntryClicked(buttonComp);
			else 
				OnIconEntryClicked(firstEntry);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	//! SCR_ButtonImageComponent event
	protected void OnIconEntryFocused(Widget rootW)
	{
		if (GetGame().GetInputManager().IsUsingMouseAndKeyboard())
			return;
		
		SCR_ButtonBaseComponent buttonComp = SCR_ButtonBaseComponent.Cast(rootW.FindHandler(SCR_ButtonBaseComponent));
		if (buttonComp)
			OnIconEntryClicked(buttonComp);
	}
	
	//------------------------------------------------------------------------------------------------
	//! SCR_ButtonImageComponent event
	protected void OnIconEntryClicked(notnull SCR_ButtonBaseComponent component)
	{
		if (m_SelectedIconButton)
		{
			m_SelectedIconButton.SetBackgroundColors(BACKGROUND_DEFAULT);
			m_SelectedIconButton.ColorizeBackground(false);
		}
		
		component.SetBackgroundColors(BACKGROUND_SELECTED);
		component.ColorizeBackground(false);	// this will color the button to hover color for KBM
		m_SelectedIconButton = component;
		m_iSelectedIconID = m_mIconIDs.Get(component);
		
		ResourceName imageset, imagesetGlow;
		string quad;
		m_PlacedMarkerConfig.GetIconEntry(m_iSelectedIconID, imageset, imagesetGlow, quad);
		
		m_OnChanged.Invoke(this, imageset, imagesetGlow, quad);
		
		SetSelectedImage(imageset, imagesetGlow, quad);
	}
	
	void SetSelectedImage(ResourceName imageset, ResourceName imagesetGlow, string imageQuad)
	{
		m_SelectedIconImage.LoadImageFromSet(0, imageset, imageQuad);
		m_SelectedIconImageGlow.LoadImageFromSet(0, imagesetGlow, imageQuad);
	}
	
	protected void OnTabChanged(SCR_TabViewComponent tabView, Widget widget, int index)
	{
		SCR_TabViewContent tab = tabView.GetEntryContent(index);
		InitCategoryIcons(tab);
	}
	
	
}