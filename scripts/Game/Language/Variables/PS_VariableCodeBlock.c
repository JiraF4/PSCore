class PS_VariableCodeBlock : PS_Variable
{
	ref PS_VMCodeBlock m_cCodeBlock;
	
	override string GetValueName(bool arrayFirstScope = true)
	{
		return "CodeBlock";
	}
	
	override void PrintVariable()
	{
		Print("CODE BLOCK: " + m_cCodeBlock.ToString());
	}
	
	void PS_VariableCodeBlock(PS_VMCodeBlock codeBlock)
	{
		m_cCodeBlock = codeBlock;
	}
}