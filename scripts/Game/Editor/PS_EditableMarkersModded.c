modded class SCR_EditorAttributeStruct: JsonApiStruct
{
	protected string s1;
	override static void SerializeAttributes(out notnull array<ref SCR_EditorAttributeStruct> outEntries, SCR_EditorAttributeList attributeList = null, Managed item = null)
	{
		outEntries.Clear();
		
		SCR_BaseEditorAttribute attribute;
		SCR_BaseEditorAttributeVar var;
		for (int i = 0, count = attributeList.GetAttributesCount(); i < count; i++)
		{
			attribute = attributeList.GetAttribute(i);
			if (!attribute.IsServer() || !attribute.IsSerializable())
				continue;
			
			var = attribute.ReadVariable(item, null);
			if (!var)
				continue;
			
			SCR_EditorAttributeStruct entry = new SCR_EditorAttributeStruct();
			outEntries.Insert(entry);
			entry.id = i;
			
			entry.v0 = var.GetVector()[0];
			entry.v1 = var.GetVector()[1];
			entry.v2 = var.GetVector()[2];
			
			PS_StringEditorAttributeVar strVar = PS_StringEditorAttributeVar.Cast(var);
			if (!strVar)
				continue;
			
			entry.s1 = strVar.GetString();
		}
	}
	override static void DeserializeAttributes(notnull array<ref SCR_EditorAttributeStruct> entries, SCR_EditorAttributeList attributeList = null, Managed item = null)
	{
		SCR_BaseEditorAttribute attribute;
		PS_StringEditorAttributeVar var;
		
		foreach (SCR_EditorAttributeStruct entry: entries)
		{
			attribute = attributeList.GetAttribute(entry.id);
			if (!attribute.IsSerializable())
				continue;
			
			var = new PS_StringEditorAttributeVar();
			var.SetVector(Vector(entry.v0, entry.v1, entry.v2));
			var.SetString(entry.s1);
			
			attribute.WriteVariable(item, var, null, -1);
		}
	}
	
	void SCR_EditorAttributeStruct()
	{
		RegV("s1");
	}
};

modded class SCR_BaseEditorAttributeVar
{	
	override static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet) 
	{
		snapshot.Serialize(packet, packet.Tell()/8 + 1);
	}
	override static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		return snapshot.Serialize(packet, packet.Tell()/8 + 1);
	}
	override static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint) 
	{
		return lhs.CompareSnapshots(rhs, rhs.Tell());
	}
	override static bool PropCompare(SCR_BaseEditorAttributeVar prop, SSnapSerializerBase snapshot, ScriptCtx hint) 
	{
		PS_StringEditorAttributeVar strVar = PS_StringEditorAttributeVar.Cast(prop);
		if (strVar)
			snapshot.CompareString(strVar.GetString());
		return snapshot.Compare(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
	}
	override static bool Extract(SCR_BaseEditorAttributeVar prop, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		PS_StringEditorAttributeVar strVar = PS_StringEditorAttributeVar.Cast(prop);
		if (strVar)
		{
			snapshot.SerializeString(strVar.m_vString);
			return true;
		}
		snapshot.SerializeBytes(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
		return true;
	}
	override static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, SCR_BaseEditorAttributeVar prop) 
	{
		return Extract(prop, hint, snapshot);
	}
}