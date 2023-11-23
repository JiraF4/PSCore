class PS_LanguageNodeNumber : PS_LanguageNode 
{
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		nodes.Insert(new PS_VMCommandPushConst(new PS_VariableNumber(m_tToken.text.ToFloat())));
	}
};