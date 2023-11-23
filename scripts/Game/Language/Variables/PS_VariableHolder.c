class PS_VariableHolder : PS_Variable
{
	ref PS_Variable m_vVariable;
	
	override string GetValueName(bool arrayFirstScope = true)
	{
		return "VariableHolder";
	}
	
	override void PrintVariable()
	{
		m_vVariable.PrintVariable();
	}
	
	void PS_VariableHolder(PS_Variable variable)
	{
		m_vVariable = variable;
	}
}