class PS_LanguageNodeVariable : PS_LanguageNode 
{
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		nodes.Insert(new PS_VMCommandPushVar(m_tToken.text));
	}
};

