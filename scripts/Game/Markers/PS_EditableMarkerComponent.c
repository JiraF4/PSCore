[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class PS_EditableMarkerComponentClass: SCR_EditableDescriptorComponentClass
{
	[Attribute("#AR-Tasks_Objective", desc: "Name of objective type eg: Attack objective. Used among in, among other things, in notifications", category: "Visualization")]
	protected  LocalizedString m_sObjectiveTypeName;
	
	/*!
	Get objective type name. The name is the same for each prefab objective type
	\return Objective type name
	*/
	LocalizedString GetObjectiveTypeName()
	{
		return m_sObjectiveTypeName;
	}
	
};

class PS_EditableMarkerComponent: SCR_EditableSystemComponent
{	
	override void SetTransform(vector transform[4], bool changedByUser = false)
	{	
		super.SetTransform(transform, changedByUser);
	}
	
	string GetMarkerDescription()
	{
		PS_ManualMarker manualMarker = PS_ManualMarker.Cast(m_Owner);
		return manualMarker.m_sDescription;
	}
	void SetMarkerDescription(string str)
	{
		PS_ManualMarker manualMarker = PS_ManualMarker.Cast(m_Owner);
		manualMarker.m_sDescription = str;
	}
	
	override bool Serialize(out SCR_EditableEntityComponent outTarget = null, out int outTargetIndex = -1, out EEditableEntitySaveFlag outSaveFlags = 0)
	{
		return super.Serialize(outTarget, outTargetIndex, outSaveFlags);
	}
	override void Deserialize(SCR_EditableEntityComponent target, int targetValue)
	{
		super.Deserialize(target, targetValue);
	}
	override ScriptInvoker GetOnUIRefresh()
	{
		return Event_OnUIRefresh;
	}
	override bool RplSave(ScriptBitWriter writer)
	{
		if (!super.RplSave(writer))
			return false;
		
		return true;
	}
	override bool RplLoad(ScriptBitReader reader)
	{
		if (!super.RplLoad(reader))
			return false;
		
		return true;
	}
	override SCR_EditableEntityComponent EOnEditorPlace(out SCR_EditableEntityComponent parent, SCR_EditableEntityComponent recipient, EEditorPlacingFlags flags, bool isQueue)
	{
		return this;
	}
	void SCR_EditableTaskComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
		
	}
	
	override bool Delete(bool changedByUser = false, bool updateNavmesh = true)
	{
		return super.Delete(updateNavmesh);
	}
};
