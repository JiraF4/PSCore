class PS_StringEditorAttributeVar : SCR_BaseEditorAttributeVar
{
	string m_vString;
	
	void SetString(string value)
	{
		m_vString = value;
	}
	string GetString()
	{
		return m_vString;
	}
	
	//--- Network functions
	override static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet) 
	{
		snapshot.Serialize(packet, SNAPSHOT_SIZE_VALUE);
	}
	override static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		return snapshot.Serialize(packet, SNAPSHOT_SIZE_VALUE);
	}
	override static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint) 
	{
		return lhs.CompareSnapshots(rhs, SNAPSHOT_SIZE_VALUE);
	}
	override static bool PropCompare(SCR_BaseEditorAttributeVar prop, SSnapSerializerBase snapshot, ScriptCtx hint) 
	{
		return snapshot.Compare(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
	}
	override static bool Extract(SCR_BaseEditorAttributeVar prop, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		snapshot.SerializeBytes(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
		return true;
	}
	override static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, SCR_BaseEditorAttributeVar prop) 
	{
		return Extract(prop, hint, snapshot);
	}
	
	//--- Default functions
	static PS_StringEditorAttributeVar CreateString(string value)
	{
		PS_StringEditorAttributeVar var = new PS_StringEditorAttributeVar();
		var.SetString(value);
		return var;
	}
};