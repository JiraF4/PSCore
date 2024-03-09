// WLib like changable marcer icons list
// Mainly copy of SCR_MapMarkersUI, but packed into separated prefab.
// Layout: {BE6B4B3F68EA340D}UI/PS_MapIconList.layout
class PS_MapIconListComponent : SCR_ChangeableComponentBase
{
	// Name of currently selected markers category
	[Attribute("#AR-MapMarker_ParentCategory", UIWidgets.Auto, "Menu category name" )]
	protected string m_sCategoryName;
	// Layout witch created for each icon in list
	[Attribute("{DEA2D3B788CDCB4F}UI/layouts/Map/MapIconSelectorEntry.layout", UIWidgets.ResourceNamePicker, desc: "Icon selection entry layout", params: "layout" )]
	protected ResourceName m_sSelectorIconEntry;
	// Maximum icons in one line
	[Attribute("20", UIWidgets.Auto, "Icon selector entries per line" )]
	protected int m_iIconsPerLine;
	
	// Marker configuration registered in markers configuration defaault is: {720E8E61D7692172}Configs/Map/MapMarkerConfig.conf
	// It's configured in gamemode -> marker manager component
	protected SCR_MapMarkerEntryPlaced m_PlacedMarkerConfig;
	
	// Category selection tab widget
	protected SCR_TabViewComponent m_TabComponent; 
	
	// Main container
	protected Widget m_IconSelector; // Widget with contains all icon rows
	protected const string ICON_SELECTOR = "IconSelector"; // Name of the m_IconSelector
	
	// Line container
	protected const ResourceName SELECTOR_LINE = "{CF8EC7A0D310A8D9}UI/layouts/Map/MapColorSelectorLine.layout"; // Path to icons line layout
	protected int m_iIconLines; // Count of icon lines
	
	// Icon button
	protected ref map<SCR_ButtonBaseComponent, int> m_mIconIDs = new map<SCR_ButtonBaseComponent, int>(); // Map of all icon buttons, with ID of icon
	protected int m_iIconEntryCount; // Count of showed Icons
	protected const string ICON_ENTRY = "IconEntry"; // Prefix for each icon button name
	protected ref Color BACKGROUND_DEFAULT = new Color(4,4,4,255); // Color of deselected
	protected ref Color BACKGROUND_SELECTED = new Color(16,16,16,255); // Color of selectted button
	
	// Currently selected
	protected SCR_ButtonBaseComponent m_SelectedIconButton; // Currently select Icon button
	protected int m_iSelectedIconID; // Currently select Icon id from m_PlacedMarkerConfig
	
	// Currently selected image, it may not be from marker list
	protected ImageWidget m_SelectedIconImage;
	protected ImageWidget m_SelectedIconImageGlow;
	
	override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		
		// Get global markers config
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (!gameMode) return;
		SCR_MapMarkerManagerComponent markerMgr = SCR_MapMarkerManagerComponent.Cast(gameMode.FindComponent(SCR_MapMarkerManagerComponent));
		SCR_MapMarkerConfig markerConfig = markerMgr.GetMarkerConfig();
		if (!markerConfig)
			return;
		
		// Get all custom markers
		m_PlacedMarkerConfig = SCR_MapMarkerEntryPlaced.Cast(markerConfig.GetMarkerEntryConfigByType(SCR_EMapMarkerType.PLACED_CUSTOM));
		
		// Category selection tab widget
		Widget categoryTab = m_wRoot.FindAnyWidget("MarkerEditTab");
		m_TabComponent = SCR_TabViewComponent.Cast(categoryTab.FindHandler(SCR_TabViewComponent));
		// Add tab for each available category 
		array<ref SCR_MarkerIconCategory> categoriesArr = m_PlacedMarkerConfig.GetIconCategories();
		foreach (SCR_MarkerIconCategory category : categoriesArr)
		{
			m_TabComponent.AddTab(string.Empty, category.m_sName, identifier: category.m_sIdentifier);
		}
		
		// Currently selected icon widget
		m_SelectedIconImage = ImageWidget.Cast(m_wRoot.FindAnyWidget("SelectedIconImage"));
		m_SelectedIconImageGlow = ImageWidget.Cast(m_wRoot.FindAnyWidget("SelectedIconImageGlow"));
		
		// Get main icon container
		m_IconSelector = m_wRoot.FindAnyWidget(ICON_SELECTOR);	
		
		// Register tab change event and switch to first available
		m_TabComponent.GetOnChanged().Insert(OnTabChanged);
		m_TabComponent.ShowTab(0, true, false);
	}
	
	// Call every time category tab changed
	protected void InitCategoryIcons(SCR_TabViewContent tabContent)
	{
		// Clear old data
		m_mIconIDs.Clear();
		m_SelectedIconButton = null;
		m_iIconEntryCount = 0;
		m_iIconLines = 1;
			
		// Remove every old row
		Widget child = m_IconSelector.GetChildren();
		while (child)
		{
			child.RemoveFromHierarchy();
			child = m_IconSelector.GetChildren();
		}
		
		// Create new line
		Widget iconSelectorLine = GetGame().GetWorkspace().CreateWidgets(SELECTOR_LINE, m_IconSelector);
				
		// Get all registered in config marker icons
		array<ref SCR_MarkerIconEntry> iconsArr = m_PlacedMarkerConfig.GetIconEntries();
		foreach (int i, SCR_MarkerIconEntry iconEntry : iconsArr)
		{
			// If wrong category skip icon
			if (iconEntry.m_sCategoryIdentifier != tabContent.m_sTabIdentifier)
				continue;
			
			// Counting icons and creating new line if need
			m_iIconEntryCount++;
			if (m_iIconEntryCount > m_iIconsPerLine * m_iIconLines)
			{
				iconSelectorLine = GetGame().GetWorkspace().CreateWidgets(SELECTOR_LINE, m_IconSelector);
				m_iIconLines++;
			}
			
			// Create new icon widget
			Widget button = GetGame().GetWorkspace().CreateWidgets(m_sSelectorIconEntry, iconSelectorLine);
			button.SetName(ICON_ENTRY + m_iIconEntryCount.ToString());
			SCR_ButtonImageComponent buttonComp = SCR_ButtonImageComponent.Cast(button.FindHandler(SCR_ButtonImageComponent));
			m_mIconIDs.Insert(buttonComp, i);
			
			// Set button image and register events
			string imageset, imagesetGlow, quad;
			iconEntry.GetIconResource(imageset, imagesetGlow, quad);
			buttonComp.SetImage(imageset, quad);
			buttonComp.m_OnClicked.Insert(OnIconEntryClicked);
			buttonComp.m_OnFocus.Insert(OnIconEntryFocused);
		}
	}
	
	// Set currently selected icon by imagesets
	void SetSelectedImage(ResourceName imageset, ResourceName imagesetGlow, string imageQuad)
	{
		m_SelectedIconImage.LoadImageFromSet(0, imageset, imageQuad);
		m_SelectedIconImageGlow.LoadImageFromSet(0, imagesetGlow, imageQuad);
	}
	
	// Icon button events
	protected void OnIconEntryFocused(Widget rootW)
	{
		// If gamepade boi play game, we can't click, soo apply icon on focus
		if (GetGame().GetInputManager().IsUsingMouseAndKeyboard())
			return;
		
		SCR_ButtonBaseComponent buttonComp = SCR_ButtonBaseComponent.Cast(rootW.FindHandler(SCR_ButtonBaseComponent));
		if (buttonComp)
			OnIconEntryClicked(buttonComp);
	}
	protected void OnIconEntryClicked(notnull SCR_ButtonBaseComponent component)
	{
		// Deselect old button
		if (m_SelectedIconButton)
		{
			m_SelectedIconButton.SetBackgroundColors(BACKGROUND_DEFAULT);
			m_SelectedIconButton.ColorizeBackground(false);
		}
		
		// Change color of selected icon
		component.SetBackgroundColors(BACKGROUND_SELECTED);
		component.ColorizeBackground(false);	// this will color the button to hover color for KBM -- A?
		m_SelectedIconButton = component;
		m_iSelectedIconID = m_mIconIDs.Get(component);
		
		// Get imagesets and quad from config by id
		ResourceName imageset, imagesetGlow, quad;
		m_PlacedMarkerConfig.GetIconEntry(m_iSelectedIconID, imageset, imagesetGlow, quad);
		
		// Set selected icon, and trigger event
		SetSelectedImage(imageset, imagesetGlow, quad);
		m_OnChanged.Invoke(this, imageset, imagesetGlow, quad);
	}
	
	// Tab widget event
	protected void OnTabChanged(SCR_TabViewComponent tabView, Widget widget, int index)
	{
		SCR_TabViewContent tab = tabView.GetEntryContent(index);
		InitCategoryIcons(tab);
	}
	
	
}