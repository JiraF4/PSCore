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
		
		w_hEditBox.SetValue(var.GetString());
	}
	
	override bool OnChange(Widget w, bool finished)
	{
		SCR_BaseEditorAttribute attribute = GetAttribute();
		if (!attribute) return false;

		SCR_BaseEditorAttributeVar var = attribute.GetVariable(true);
		if (!var)
			return false;
		var.SetString(w_hEditBox.GetValue());
		
		super.OnChange(w, finished);
		return false;
	}	
	
	override void HandlerDeattached(Widget w)
	{
			
	}
};