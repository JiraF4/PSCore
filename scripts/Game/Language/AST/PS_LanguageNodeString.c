class PS_LanguageNodeString : PS_LanguageNode 
{
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		nodes.Insert(new PS_VMCommandPushConst(new PS_VariableString(m_tToken.text.Substring(1, m_tToken.text.Length() - 2))));
	}
};