[BaseContainerProps(), BaseContainerCustomTitleField("m_sDisplayName")]
class PS_MarkerEditorAttribute: SCR_BaseEditorAttribute
{
	[Attribute(SCR_Enum.GetDefault(ETaskTextType.NONE), UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ETaskTextType))]
	protected ETaskTextType m_TextType;
	
	[Attribute(uiwidget: UIWidgets.LocaleEditBox)]
	protected LocalizedString m_sLocationNamePlaceholder;
	
	override PS_StringEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return null;
		return PS_StringEditorAttributeVar.CreateString(marker.GetMarkerDescription()); // ВОТ ЗДЕСЬ ЭТА ХУЙНЯ
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return;
		
		PS_StringEditorAttributeVar strVar = PS_StringEditorAttributeVar.Cast(var);
		if (!strVar)
			return;
		
		marker.SetMarkerDescription(strVar.GetString())
	}
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		return 0;
	}
};