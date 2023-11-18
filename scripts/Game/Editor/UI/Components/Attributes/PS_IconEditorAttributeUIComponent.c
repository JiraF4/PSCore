/** @ingroup Editor_UI Editor_UI_Components Editor_UI_Attributes
*/
class PS_IconEditorAttributeUIComponent: SCR_BaseEditorAttributeUIComponent
{
	PS_MapIconListComponent w_hIconList;
	
	override void Init(Widget w, SCR_BaseEditorAttribute attribute)
	{
		Widget iconListWidget = w.FindAnyWidget(m_sUiComponentName);
		if (!iconListWidget) 
			return;
		
		w_hIconList = PS_MapIconListComponent.Cast(iconListWidget.FindHandler(PS_MapIconListComponent));
		if (!w_hIconList) 
			return;
		
		w_hIconList.m_OnChanged.Insert(OnIconSelect);
		
		super.Init(w, attribute);
	}
	
	override void SetVariableToDefaultValue(SCR_BaseEditorAttributeVar var)
	{
		if (var)
			var.SetBool(false);
	}
	
	override void SetFromVar(SCR_BaseEditorAttributeVar var)
	{	
		super.SetFromVar(var);
		
		if (!var)
			return;
		
		SetSelectedImage(var.GetString());
	}
	
	
	void SetSelectedImage(string imageStr)
	{
		array<string> outTokens = new array<string>();
		imageStr.Split("|", outTokens, false);
		w_hIconList.SetSelectedImage(outTokens[0], outTokens[1], outTokens[2]);
	}
	
	bool OnIconSelect(PS_MapIconListComponent iconList, ResourceName imageSet, ResourceName imageSetGlow, string quadName)
	{
		SCR_BaseEditorAttribute attribute = GetAttribute();
		if (!attribute) return false;

		SCR_BaseEditorAttributeVar var = attribute.GetVariable(true);
		if (!var)
			return false;
		var.SetString(imageSet + "|" + imageSetGlow + "|" + quadName);
		
		return false;
	}	
	
	override void HandlerDeattached(Widget w)
	{
		if (w_hIconList)
			w_hIconList.m_OnChanged.Remove(OnIconSelect);	
	}
};