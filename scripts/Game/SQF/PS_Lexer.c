class PS_Lexer
{
	string code;
	int position;
	string errorStr;
	ref array<ref PS_Token> tokens = new array<ref PS_Token>();
	
	
	string PeekBackCode(int peekPosition, int length)
	{
		peekPosition = peekPosition - length;
		if (peekPosition > code.Length()) return "";
		if (peekPosition < 1) peekPosition = 0;
		int endPosition = peekPosition + length;
		if (endPosition > code.Length()) endPosition = code.Length();
		string part = code.Substring(peekPosition, endPosition - peekPosition);
		if (peekPosition > 0) part = "..." + part;
		return part;
	}
	
	void PS_Lexer(string nCode)
	{
		code = nCode + " ";
		lexAnalysis();
	}
	
	void PrintTokens()
	{
		string output = "";
		foreach (PS_Token token: tokens)
		{			
			//Print(string.Format("Token: %1, %2, %3", token.position, token.text, token.GetTokenName()))
    		output += string.Format("<tr><td>%1</td><td>%2</td><td>%3</td></tr>", token.position, token.text, token.GetTokenName());
		}
		PrintFormat("%1", output);
	}
	
	protected void lexAnalysis()
	{
		while (nextToken()) {};
		if (errorStr != "") Print(errorStr, LogLevel.ERROR);
	}
	
	protected bool nextToken()
	{
		if (position >= code.Length()) {
            return false;
        }
		
		// check first char
		if (code.IsSpaceAt(position))
		{
			position++;
			return true;
		}
		string snipet = code.Get(position);
		position++;
		
		// Single char
		if (PS_LanguageManager.GetInstance().m_mSingleCharTokens.Contains(snipet))
		{
			PS_Token token = new PS_Token(PS_LanguageManager.GetInstance().m_mSingleCharTokens[snipet], snipet, position);
			tokens.Insert(token);
			return true;
		}
		
		// Variable local
		if (snipet == "_")
		{
			while(IsChar(code.Get(position)) && !PS_LanguageManager.GetInstance().m_mSingleCharTokens.Contains(code.Get(position)))
			{
				snipet += code.Get(position);
				position++;
			}
			PS_Token token = new PS_Token(PS_ETokenType.VARIABLELOCAL, snipet, position);
			tokens.Insert(token);
			
			return true;
		}
		
		// number
		if (code.IsDigitAt(position-1))
		{
			bool dotFound = false;
			while(code.IsDigitAt(position) || (code.Get(position) == "." && !dotFound))
			{
				snipet += code.Get(position);
				if (code.Get(position) == ".")
					dotFound = true;
				position++;
			}
			
			PS_Token token = new PS_Token(PS_ETokenType.NUMBER, snipet, position);
			tokens.Insert(token);
			return true;
		};
		
		// string
		if (snipet == "\"")
		{
			bool dotFound = false;
			int stringStartPosition = position;
			while(true)
			{
				snipet += code.Get(position);
				if (code.Get(position) == "\"")
				{
					position++;
					break;
				}
				position++;
				dotFound = true;
				if (position >= code.Length()) {
					errorStr = "Can't found string end at pos:" + stringStartPosition;
		            return false;
		        }
			}
			
			PS_Token token = new PS_Token(PS_ETokenType.STRING, snipet, position);
			tokens.Insert(token);
			return true;
		};
		
		// Command/Variable
		while(IsChar(code.Get(position)) && !PS_LanguageManager.GetInstance().m_mSingleCharTokens.Contains(code.Get(position)))
		{
			snipet += code.Get(position);
			position++;
		}
		if (PS_LanguageManager.GetInstance().m_mMultiCharTokens.Contains(snipet))
		{
			PS_Token token = new PS_Token(PS_LanguageManager.GetInstance().m_mMultiCharTokens[snipet], snipet, position);
			tokens.Insert(token);
		} else if (PS_LanguageManager.GetInstance().m_mCommands.Contains(snipet))
		{
			PS_Token token = new PS_Token(PS_ETokenType.COMMAND, snipet, position);
			tokens.Insert(token);
		} else {
			PS_Token token = new PS_Token(PS_ETokenType.GLOBALVARIABLE, snipet, position);
			tokens.Insert(token);
		}
		
		return true;
	}
	
	bool IsChar(string char)
	{
		return (char.ToAscii() >= 41 && char.ToAscii() <= 90)
			|| (char.ToAscii() >= 97 && char.ToAscii() <= 122);
	}
}