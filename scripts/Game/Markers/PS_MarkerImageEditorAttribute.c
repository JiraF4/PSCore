[BaseContainerProps()]
class PS_MarkerImageEditorAttribute: SCR_BaseFloatValueHolderEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return null;
		return SCR_BaseEditorAttributeVar.CreateString(marker.GetMarkerImage());
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return;
		
		marker.SetMarkerImage(var.GetString())
	}
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		return super.GetEntries(outEntries);
	}
};