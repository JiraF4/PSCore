class PS_VariableBool : PS_Variable
{
	bool m_mBool;
	
	override string GetValueName(bool fancy = false, bool arrayFirstScope = true)
	{
		if (m_mBool)
			return "true";
		else
			return "false";
	}
	
	override void PrintVariable()
	{
		Print("BOOL VARIABLE: " + m_mBool.ToString());
	}
	
	void PS_VariableBool(bool nBool)
	{
		m_mBool = nBool;
	}
}
