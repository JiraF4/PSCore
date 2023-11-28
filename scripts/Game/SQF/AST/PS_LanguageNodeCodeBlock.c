class PS_LanguageNodeCodeBlock : PS_LanguageNode 
{
	ref array<ref PS_LanguageNode> m_aNodes = new array<ref PS_LanguageNode>();
	
	override void PrintNode(int level)
	{
		super.PrintNode(level);
		level++;
		foreach (PS_LanguageNode node: m_aNodes)
		{
			node.PrintNode(level);
		}
	}
	
	void InsertNode(PS_LanguageNode expression)
	{
		m_aNodes.Insert(expression);
	}
	
	// Entry point of compilation
	PS_VMCodeBlock CompileBlock()
	{
		array<ref PS_VMCommand> nodes = new array<ref PS_VMCommand>;
		foreach (PS_LanguageNode node: m_aNodes)
		{
			node.FillExecuteSteps(nodes);
			nodes.Insert(new PS_VMCommandClearStack());
		}
		return new PS_VMCodeBlock(nodes);
	}
	
	override void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		PS_VMCodeBlock codeBlock = CompileBlock(); // Recursive compile sub blocks
		// Pack compiled to variable
		nodes.Insert(new PS_VMCommandPushConst(new PS_VariableCodeBlock(codeBlock)));
	}
};