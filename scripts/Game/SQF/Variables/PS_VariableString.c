class PS_VariableString : PS_Variable
{
	string m_mString;
	
	override string GetValueName(bool fancy = false, bool arrayFirstScope = true)
	{
		if (fancy)
		{
			return string.Format("\"%1\"", m_mString);
		}
		return m_mString;
	}
	
	override void PrintVariable()
	{
		Print("STRING VARIABLE: " + m_mString);
	}
	
	void PS_VariableString(string nStr)
	{
		m_mString = nStr;
	}
}