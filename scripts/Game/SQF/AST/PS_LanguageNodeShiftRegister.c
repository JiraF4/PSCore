class PS_LanguageNodeShiftRegister: PS_LanguageNode
{
	ref PS_LanguageNode m_eNode;
	
	override void PrintNode(int level)
	{
		super.PrintNode(level);
		if (m_eNode)
			m_eNode.PrintNode(level);
	}
}