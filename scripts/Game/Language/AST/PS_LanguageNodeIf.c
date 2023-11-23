class PS_LanguageNodeIf : PS_LanguageNode 
{
	ref PS_LanguageNodeCodeBlock m_nConditionNode;
	ref PS_LanguageNodeCodeBlock m_nTrueNode;
	ref PS_LanguageNodeCodeBlock m_nFalseNode;
	
	override void PrintNode(int level)
	{
		Print(level.ToString(level+1) + " CONDITION");
		m_nConditionNode.PrintNode(level+1);
		Print(level.ToString(level+1) + " TRUE");
		m_nTrueNode.PrintNode(level+1);
		if (m_nFalseNode)
		{
			Print(level.ToString(level+1) + " FALSE");
			m_nFalseNode.PrintNode(level+1);
		}
	}
	
	void SetNodes(PS_LanguageNodeCodeBlock conditionCodeBlock, PS_LanguageNodeCodeBlock trueCodeBlock, PS_LanguageNodeCodeBlock falseCodeBlock)
	{
		m_nConditionNode = conditionCodeBlock;
		m_nTrueNode = trueCodeBlock;
		m_nFalseNode = falseCodeBlock;
	}
	
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		m_nConditionNode.FillExecuteSteps(nodes);                        // Condition compile and push to stack
		nodes.Insert(new PS_VMCommandRunCodeBlock());                    // run condition block
		nodes.Insert(new PS_VMCommandPushRegister());                    // push run result
		m_nTrueNode.FillExecuteSteps(nodes);                             // True compile and push to stack
		if (m_nFalseNode) m_nFalseNode.FillExecuteSteps(nodes);          // False compile and push to stack
		else nodes.Insert(new PS_VMCommandPushConst(new PS_Variable())); // If there's no false part push null
		nodes.Insert(new PS_VMCommandRunCodeBlockConditional());         // Run conditional
	}
};