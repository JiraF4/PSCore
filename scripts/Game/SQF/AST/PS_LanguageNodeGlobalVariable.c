class PS_LanguageNodeGlobalVariable : PS_LanguageNode 
{
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		nodes.Insert(new PS_VMCommandPushVarGlobal(m_tToken.text));
	}
};

