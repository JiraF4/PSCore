class PS_VariableNumber : PS_Variable
{
	float m_mNumber;
	
	override string GetValueName(bool arrayFirstScope = true)
	{
		return m_mNumber.ToString();
	}
	
	override void PrintVariable()
	{
		Print("NUMBER VARIABLE: " + m_mNumber.ToString());
	}
	
	void PS_VariableNumber(float nNumber)
	{
		m_mNumber = nNumber;
	}
}