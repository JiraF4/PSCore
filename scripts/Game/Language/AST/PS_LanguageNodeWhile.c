class PS_LanguageNodeWhile : PS_LanguageNode 
{
	ref PS_LanguageNodeCodeBlock m_nConditionNode;
	ref PS_LanguageNodeCodeBlock m_nTrueNode;
	
	override void PrintNode(int level)
	{
		Print(level.ToString(level+1) + " WHILE");
		m_nConditionNode.PrintNode(level+1);
		Print(level.ToString(level+1) + " DO");
		m_nTrueNode.PrintNode(level+1);
	}
	
	void SetNodes(PS_LanguageNodeCodeBlock conditionCodeBlock, PS_LanguageNodeCodeBlock trueCodeBlock)
	{
		m_nConditionNode = conditionCodeBlock;
		m_nTrueNode = trueCodeBlock;
	}
	
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		nodes.Insert(new PS_VMCommandJumpRelative(2));             // Skip first execution
		m_nTrueNode.FillExecuteSteps(nodes);                       // True compile and push to stack
		nodes.Insert(new PS_VMCommandRunCodeBlock());			   // Run true
		m_nConditionNode.FillExecuteSteps(nodes);                  // Condition compile and push to stack
		nodes.Insert(new PS_VMCommandRunCodeBlock());              // Run condition block
		nodes.Insert(new PS_VMCommandPushRegister());              // Push run result
		nodes.Insert(new PS_VMCommandJumpRelativeConditional(-6)); // If condition run true
	}
};