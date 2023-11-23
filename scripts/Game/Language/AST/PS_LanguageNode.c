class PS_LanguageNode 
{
	ref PS_Token m_tToken;
	
	void PrintNode(int level)
	{
		string tokenName = "NULL";
		if (m_tToken) tokenName = m_tToken.text;
		Print(level.ToString(level+1) + " " + this.ToString() + " - " + tokenName);
	}
	
	void PrintNodeSingle()
	{
		Print(this.ToString());
	}
	
	void FillExecuteSteps(out array<ref PS_VMCommand> nodes)
	{
		
	}
	
	void PS_LanguageNode(PS_Token token)
	{
		m_tToken = token;
	}
};