[BaseContainerProps(), BaseContainerCustomTitleField("m_sDisplayName")]
class PS_MarkerEditorAttribute: SCR_BaseEditorAttribute
{
	[Attribute(SCR_Enum.GetDefault(ETaskTextType.NONE), UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(ETaskTextType))]
	protected ETaskTextType m_TextType;
	
	[Attribute(uiwidget: UIWidgets.LocaleEditBox)]
	protected LocalizedString m_sLocationNamePlaceholder;
	
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return null;
		return SCR_BaseEditorAttributeVar.CreateString(marker.GetMarkerDescription()); // ВОТ ЗДЕСЬ ЭТА ХУЙНЯ
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return;
		
		marker.SetMarkerDescription(var.GetString())
	}
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		return 0;
	}
};