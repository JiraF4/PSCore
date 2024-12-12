// BEWARE REALY STRANGE VANILA THINGS HERE
// like saving EVERYTHING to vector

// Since SCR_EditorAttributeStruct use JsonApiStruct fo serialization it's can't inherite class structure
// And I really need strings, 12 bytes is 12 characters...
// Why vanila can't just pack GM attribues to string? I mean every attribue int, float, vector and other, after all that's what json is made for?
// Well... At least this solution save compatibility with vanila saves.
modded class SCR_EditorAttributeStruct: JsonApiStruct
{
	// Add string to our struct
	protected string s1;
	
	// Copy of original function
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
			entry.s1 = var.GetString(); // Write our string here
		}
		
	}
	// Copy of original function
	override static void DeserializeAttributes(notnull array<ref SCR_EditorAttributeStruct> entries, SCR_EditorAttributeList attributeList = null, Managed item = null)
	{
		SCR_BaseEditorAttribute attribute;
		SCR_BaseEditorAttributeVar var;
		
		foreach (SCR_EditorAttributeStruct entry: entries)
		{
			attribute = attributeList.GetAttribute(entry.id);
			if (!attribute.IsSerializable())
				continue;
			
			var = new SCR_BaseEditorAttributeVar();
			var.SetVector(Vector(entry.v0, entry.v1, entry.v2));
			var.SetString(entry.s1); // Read our string here
			
			attribute.WriteVariable(item, var, null, -1);
		}
	}
	
	void SCR_EditorAttributeStruct()
	{
		RegV("s1"); // Register our string to json api
	}
};

// SCR_BaseEditorAttributeVar also can't be inherited, since Rpc use static calls
// Soo I'm adding string here.
modded class SCR_BaseEditorAttributeVar
{	
	string m_vString; // New string variable
	
	// Vanila like string functions
	static SCR_BaseEditorAttributeVar CreateString(string value)
	{
		SCR_BaseEditorAttributeVar var = new SCR_BaseEditorAttributeVar();
		var.SetString(value);
		return var;
	}
	void SetString(string value)
	{
		m_vString = value;
	}
	string GetString()
	{
		return m_vString;
	}
	
	// Override serealization functions
	// Here some documentation: core:scripts/Core/proto/EnNetwork.c
	override static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet) 
	{
		vector v;
		string s;
		snapshot.SerializeVector(v);
		snapshot.SerializeString(s);
		packet.SerializeVector(v);
		packet.SerializeString(s);
	}
	override static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		vector v;
		string s;
		packet.SerializeVector(v);
		packet.SerializeString(s);
		snapshot.SerializeVector(v);
		snapshot.SerializeString(s);
		return true;
	}
	override static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint) 
	{
		vector v1, v2;
		string s1, s2;
		lhs.SerializeVector(v1);
		lhs.SerializeString(s1);
		rhs.SerializeVector(v1);
		rhs.SerializeString(s2);
		return v1 == v2 && s1 == s2;
	}
	override static bool PropCompare(SCR_BaseEditorAttributeVar prop, SSnapSerializerBase snapshot, ScriptCtx hint) 
	{
		vector v;
		string s;
		snapshot.SerializeVector(v);
		snapshot.SerializeString(s);
		return prop.m_vValue == v && prop.m_vString == s;
	}
	override static bool Extract(SCR_BaseEditorAttributeVar prop, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		snapshot.SerializeBytes(prop.m_vValue, SNAPSHOT_SIZE_VALUE);
		snapshot.SerializeString(prop.m_vString);
		return true;
	}
	override static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, SCR_BaseEditorAttributeVar prop) 
	{
		return Extract(prop, hint, snapshot);
	}
}