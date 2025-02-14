[BaseContainerProps()]
class PS_TriggerSizeEditorAttribute : SCR_BaseValueListEditorAttribute
{
	override SCR_BaseEditorAttributeVar ReadVariable(Managed item, SCR_AttributesManagerEditorComponent manager)
	{
		PS_EditableTriggerComponent trigger = PS_EditableTriggerComponent.Cast(item);
		if (!trigger)
			return null;

		return SCR_BaseEditorAttributeVar.CreateFloat(trigger.GetRadius());
	}
	override void WriteVariable(Managed item, SCR_BaseEditorAttributeVar var, SCR_AttributesManagerEditorComponent manager, int playerID)
	{
		PS_EditableTriggerComponent trigger = PS_EditableTriggerComponent.Cast(item);
		if (!trigger)
			return;

		trigger.SetRadius(var.GetFloat());
	}
	override int GetEntries(notnull array<ref SCR_BaseEditorAttributeEntry> outEntries)
	{
		return super.GetEntries(outEntries);
	}
}
