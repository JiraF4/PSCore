class PS_LanguageNodeCommand: PS_LanguageNode
{
	ref PS_LanguageNode	m_nLeftOperand;
	ref PS_LanguageNode	m_nRightOperand;
	PS_Command m_cCommand;
	
	int GetOrder()
	{
		if (!m_cCommand) return 0;
		return m_cCommand.GetOrder();
	}
	
	override void PrintNode(int level)
	{
		super.PrintNode(level);
		level++;
		if (m_nLeftOperand) m_nLeftOperand.PrintNode(level);
		if (m_nRightOperand) m_nRightOperand.PrintNode(level);
	}
	
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		if (m_nRightOperand) m_nRightOperand.FillExecuteSteps(nodes);
		else nodes.Insert(new PS_VMCommandPushConst(new PS_Variable()));
		if (m_nLeftOperand)	m_nLeftOperand.FillExecuteSteps(nodes);
		else nodes.Insert(new PS_VMCommandPushConst(new PS_Variable()));
		nodes.Insert(new PS_VMCommandExecCmd(m_cCommand));
	}
	
	void PS_LanguageNodeCommand(PS_Token token)
	{
		m_cCommand = PS_LanguageManager.GetInstance().m_mCommands[token.text];
	}
}