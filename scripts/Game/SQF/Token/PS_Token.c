class PS_Token
{
	PS_ETokenType type;
	string text;
	int position;
	
	string GetTokenName()
	{
		string name = PS_LanguageManager.GetInstance().GetTokenTypeName(type) + " - \"" + text + "\"";
		return name;
	}
	
	void PS_Token(PS_ETokenType nType, string nText, int nPosition)
	{
		type = nType;
		text = nText;
		position = nPosition;
	}
}