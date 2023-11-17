/** @ingroup Editor_UI Editor_UI_Components Editor_UI_Attributes
*/
class PS_StringEditorAttributeUIComponent: SCR_BaseEditorAttributeUIComponent
{
	
	SCR_EditBoxComponent w_hEditBox;
	
	override void Init(Widget w, SCR_BaseEditorAttribute attribute)
	{
		Widget editboxWidget = w.FindAnyWidget(m_sUiComponentName);
		if (!editboxWidget) 
			return;
		
		w_hEditBox = SCR_EditBoxComponent.Cast(editboxWidget.FindHandler(SCR_EditBoxComponent));
		if (!w_hEditBox) 
			return;
		
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
		
		PS_StringEditorAttributeVar strVar = PS_StringEditorAttributeVar.Cast(var);
		if (!strVar)
			return;
		
		w_hEditBox.SetValue(strVar.GetString());
	}
	
	protected void OnChangeCheckbox(SCR_SelectionWidgetComponent selectionBox, bool value)
	{
		OnChange(selectionBox.GetRootWidget(), value, 0, false);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		SCR_BaseEditorAttribute attribute = GetAttribute();
		if (!attribute) return false;

		PS_StringEditorAttributeVar strVar = PS_StringEditorAttributeVar.Cast(attribute.GetVariable(true));
		if (!strVar)
			return false;
		
		
		strVar.SetString(w_hEditBox.GetValue());
		
		super.OnChange(w, x, y, finished);
		return false;
	}	
	
	override void HandlerDeattached(Widget w)
	{
			
	}
};