[BaseContainerProps()]
class PS_MarkerVisibleForEmptyFactionAttribute: SCR_BaseEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return null;
		return SCR_BaseEditorAttributeVar.CreateBool(marker.GetVisibleForEmptyFaction());
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		PS_EditableMarkerComponent marker = PS_EditableMarkerComponent.Cast(item);
		if (!marker)
			return;
		
		marker.SetVisibleForEmptyFaction(var.GetBool())
	}
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		return super.GetEntries(outEntries);
	}
};
