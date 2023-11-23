class PS_LanguageNodeExpression: PS_LanguageNode
{
	ref PS_LanguageNode m_eNode;
	
	override void PrintNode(int level)
	{
		if (m_eNode)
		{
			super.PrintNode(level);
			m_eNode.PrintNode(level);
		}
	}
	
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		if (m_eNode)
			m_eNode.FillExecuteSteps(nodes);
	}
}