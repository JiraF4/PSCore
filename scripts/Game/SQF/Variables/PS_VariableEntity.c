class PS_VariableEntity : PS_Variable
{
	IEntity m_eEntity;
	
	override string GetValueName(bool fancy = false, bool arrayFirstScope = true)
	{
		if (fancy)
		{
			string value = string.Format("\"%1\"", SCR_ConfigHelper.GetGUID(m_eEntity.GetPrefabData().GetPrefabName()));
			InventoryItemComponent item = InventoryItemComponent.Cast(m_eEntity.FindComponent(InventoryItemComponent));	
			if (item)
			{
				UIInfo info = item.GetUIInfo();
				if (info) {
					string localName = WidgetManager.Translate("%1", info.GetName());
					value += string.Format(" \/\*\"%1\"\*\/", localName);
				}
			}
			SCR_EditableEntityComponent editable = SCR_EditableEntityComponent.Cast(m_eEntity.FindComponent(SCR_EditableEntityComponent));	
			if (editable)
			{
				SCR_UIInfo info = editable.GetInfo();
				if (info) {
					string localName = WidgetManager.Translate("%1", info.GetName());
					value += string.Format(" \/\*\"%1\"\*\/", localName);
				}
			}
			return value;
		}
		return string.Format("%1", m_eEntity);
	}
	
	override void PrintVariable()
	{
		Print("ENTITY VARIABLE: " + m_eEntity.ToString());
	}
	
	void PS_VariableEntity(IEntity entity)
	{
		m_eEntity = entity;
	}
}