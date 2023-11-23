class PS_LanguageNodeArray : PS_LanguageNode 
{
	ref array<ref PS_LanguageNodeExpression> m_aNodes = new array<ref PS_LanguageNodeExpression>();
	
	override void PrintNode(int level)
	{
		super.PrintNode(level);
		level++;
		foreach (PS_LanguageNode node: m_aNodes)
		{
			node.PrintNode(level);
		}
	}
	
	void InsertExpression(PS_LanguageNodeExpression node)
	{
		m_aNodes.Insert(node);
	}
	
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		nodes.Insert(new PS_VMCommandPushConst(new PS_VariableArray())); // Create empty array
		foreach (PS_LanguageNodeExpression node : m_aNodes)
		{
			node.FillExecuteSteps(nodes);                 // Add expression calculation
			nodes.Insert(new PS_VMCommandPushToArray());  // Push result to array
			nodes.Insert(new PS_VMCommandPushRegister()); // Return array to stack
		}
	}
};