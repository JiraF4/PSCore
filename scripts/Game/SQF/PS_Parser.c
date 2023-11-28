class PS_Parser
{
	int m_iPosition = 0;
	PS_Lexer m_lLexer;
	string errorStr;
	ref PS_LanguageNodeCodeBlock m_lRoot;
	
	void PS_Parser(PS_Lexer lexer)
	{
		m_lLexer = lexer;
		Parse(lexer);
	}
	
	void Parse(PS_Lexer lexer)
	{
		m_lRoot = ParseCodeBlock(PS_ETokenType.RIGHTPARENTHISISFIGURE);
		if (errorStr != "") 
			Print(errorStr, LogLevel.ERROR);
	}
	
	// Parse node
	PS_LanguageNodeCodeBlock ParseCodeBlock(PS_ETokenType endTokenType)
	{
		PS_LanguageNodeCodeBlock codeBlock = new PS_LanguageNodeCodeBlock(null);
		while (m_iPosition < m_lLexer.tokens.Count())
		{
			PS_LanguageNode node;
			if (checkPrev({endTokenType}))
			{
				break;
			}
			if (check({PS_ETokenType.IF}))
			{
				node = ParseIf();
			} else if (check({PS_ETokenType.WHILE}))
			{
				node = ParseWhile();
			} else {
				node = ParseExpression(null, {PS_ETokenType.SEMICOLON, endTokenType});
			}
			if (!node) return null;
			codeBlock.InsertNode(node);
		}
		return codeBlock;
	}
	
	PS_LanguageNodeIf ParseIf()
	{
		PS_Token ifToken = match({PS_ETokenType.IF});
		if (!ifToken) return null;
		PS_LanguageNodeCodeBlock conditionCodeBlock = ParseCodeBlock(PS_ETokenType.THEN);
		if (!conditionCodeBlock) return null;
		PS_Token token = match({PS_ETokenType.LEFTPARENTHISISFIGURE});
		if (!token)  return null;
		PS_LanguageNodeCodeBlock trueCodeBlock = ParseCodeBlock(PS_ETokenType.RIGHTPARENTHISISFIGURE);
		if (!trueCodeBlock) return null;
		PS_LanguageNodeCodeBlock falseCodeBlock;
		if (check({PS_ETokenType.ELSE}))
		{
			token = match({PS_ETokenType.ELSE});
			token = match({PS_ETokenType.LEFTPARENTHISISFIGURE});
			if (!token)  return null;
			falseCodeBlock = ParseCodeBlock(PS_ETokenType.RIGHTPARENTHISISFIGURE);
			if (!falseCodeBlock) return null;
		}
		token = match({PS_ETokenType.SEMICOLON});
		if (!token) return null;
		
		PS_LanguageNodeIf nodeIf = new PS_LanguageNodeIf(ifToken);
		nodeIf.SetNodes(conditionCodeBlock, trueCodeBlock, falseCodeBlock);
		return nodeIf;
	}
	
	PS_LanguageNodeWhile ParseWhile()
	{
		PS_Token whileToken = match({PS_ETokenType.WHILE});
		if (!whileToken) return null;
		PS_LanguageNodeCodeBlock conditionCodeBlock = ParseCodeBlock(PS_ETokenType.DO);
		if (!conditionCodeBlock) return null;
		PS_Token token = match({PS_ETokenType.LEFTPARENTHISISFIGURE});
		if (!token)  return null;
		PS_LanguageNodeCodeBlock trueCodeBlock = ParseCodeBlock(PS_ETokenType.RIGHTPARENTHISISFIGURE);
		
		PS_LanguageNodeWhile nodeWhile = new PS_LanguageNodeWhile(whileToken);
		nodeWhile.SetNodes(conditionCodeBlock, trueCodeBlock);
		return nodeWhile;
	}
	
	PS_LanguageNodeExpression ParseParenthisRound(PS_Token startToken)
	{
		return ParseExpression(startToken, {PS_ETokenType.RIGHTPARENTHISISROUND});
	}
	
	PS_LanguageNodeArray ParseArray(PS_Token startToken)
	{
		PS_LanguageNodeArray arrayNode = new PS_LanguageNodeArray(startToken);
		if (!check({PS_ETokenType.RIGHTPARENTHISISROUND})) // Empty array
		{
			while (m_iPosition < m_lLexer.tokens.Count()) {
				PS_LanguageNodeExpression node = ParseExpression(startToken, {PS_ETokenType.RIGHTPARENTHISISSQUARE, PS_ETokenType.COMMA});
				if (!node) return null;
				arrayNode.InsertExpression(node);
				if (checkPrev({PS_ETokenType.RIGHTPARENTHISISSQUARE})) break;
			}
		}
		return arrayNode;
	}
	
	PS_LanguageNodeExpression ParseExpression(PS_Token startToken, array<PS_ETokenType> endTokenTypes)
	{
		PS_LanguageNodeExpression expression = new PS_LanguageNodeExpression(startToken);
		
		array<ref PS_LanguageNode> expressionNodes = new array<ref PS_LanguageNode>;
		
		while (m_iPosition < m_lLexer.tokens.Count()) {
			array<PS_ETokenType> requireTokens = {PS_ETokenType.THEN, PS_ETokenType.RIGHTPARENTHISISFIGURE, PS_ETokenType.VARIABLELOCAL, PS_ETokenType.GLOBALVARIABLE, PS_ETokenType.NUMBER, PS_ETokenType.STRING, PS_ETokenType.COMMAND, PS_ETokenType.LEFTPARENTHISISSQUARE, PS_ETokenType.LEFTPARENTHISISFIGURE, PS_ETokenType.LEFTPARENTHISISROUND};
			requireTokens.InsertAll(endTokenTypes);
			array<PS_ETokenType> nextTokenTypes = {PS_ETokenType.COMMAND, PS_ETokenType.SEMICOLON};
			nextTokenTypes.InsertAll(endTokenTypes);
			PS_Token token = match(requireTokens);
			if (!token) return null;
			// End of scope
			if (endTokenTypes.Contains(token.type))
				break;
			switch (token.type)
			{
				case PS_ETokenType.THEN:
					break;
				case PS_ETokenType.LEFTPARENTHISISSQUARE:
					expressionNodes.Insert(ParseArray(token));
					break;
				case PS_ETokenType.NUMBER:
					expressionNodes.Insert(new PS_LanguageNodeNumber(token));
					if (!require(nextTokenTypes)) 
						return null;
					break;
				case PS_ETokenType.STRING:
					expressionNodes.Insert(new PS_LanguageNodeString(token));
					if (!require(nextTokenTypes)) 
						return null;
					break;
				case PS_ETokenType.COMMAND:
					expressionNodes.Insert(new PS_LanguageNodeCommand(token));
					array<PS_ETokenType> cmdNextTokens = {PS_ETokenType.VARIABLELOCAL, PS_ETokenType.COMMAND, PS_ETokenType.SEMICOLON, PS_ETokenType.NUMBER, PS_ETokenType.STRING, PS_ETokenType.LEFTPARENTHISISROUND, PS_ETokenType.LEFTPARENTHISISSQUARE};
					cmdNextTokens.InsertAll(endTokenTypes);
					if (!require(cmdNextTokens)) 
						return null;
					break;
				case PS_ETokenType.GLOBALVARIABLE:
					expressionNodes.Insert(new PS_LanguageNodeGlobalVariable(token));
					if (!require(nextTokenTypes)) 
						return null;
					break;
				case PS_ETokenType.VARIABLELOCAL:
					expressionNodes.Insert(new PS_LanguageNodeVariable(token));
					if (!require(nextTokenTypes)) 
						return null;
					break;
				case PS_ETokenType.LEFTPARENTHISISROUND:
					expressionNodes.Insert(ParseParenthisRound(token));
					break;
			}
		}
		
		
		// Concatenate commands
		int order = 0;
		bool nodeAdded = true;
		while (nodeAdded)
		{
			nodeAdded = false;
			for (int i; i < expressionNodes.Count(); i++)
			{
				PS_LanguageNodeCommand commandNode = PS_LanguageNodeCommand.Cast(expressionNodes[i]);
				if (commandNode) 
				{
					if (commandNode && commandNode.GetOrder() == order)
					{
						if (i < expressionNodes.Count() - 1) 
						{
							if (commandNode.m_cCommand.m_bHasRightOperand) {
								commandNode.m_nRightOperand = expressionNodes[i+1];
								expressionNodes.Remove(i+1);
							}
						}
						if (i > 0) 
						{
							if (commandNode.m_cCommand.m_bHasLeftOperand) {
								commandNode.m_nLeftOperand = expressionNodes[i-1];
								expressionNodes.Remove(i-1);
								i--;
							}
						}
					}
					if (commandNode.GetOrder() >= order)
					{
						nodeAdded = true;
					}
				}
			}
			order++;
		}
		
		if (expressionNodes.Count() > 0)
			expression.m_eNode = expressionNodes[0];
		return expression;
	}
	
	// Util
	PS_Token match(array<PS_ETokenType> expected)
	{
		if (m_iPosition < m_lLexer.tokens.Count())
		{
			PS_Token currentToken = m_lLexer.tokens.Get(m_iPosition);
			if (expected.Contains(currentToken.type))
			{
				m_iPosition++;
				return currentToken;
			} else {
				MakeMatchError(currentToken, expected);
			}
		}
		return null;
	}
	
	PS_Token require(array<PS_ETokenType> expected)
	{
		if (m_iPosition >= m_lLexer.tokens.Count())
		{
			MakeMatchError(null, expected);
		}
		PS_Token currentToken = m_lLexer.tokens.Get(m_iPosition);
		if (expected.Contains(currentToken.type))
		{
			return currentToken;
		} else {
			MakeMatchError(currentToken, expected);
		}
		return null;
	}
	
	bool checkPrev(array<PS_ETokenType> expected)
	{
		if (m_iPosition >= m_lLexer.tokens.Count())
		{
			MakeMatchError(null, expected);
		}
		if (m_iPosition == 0) return false;
		if (m_lLexer.tokens.Count() <= 1) return false;
		PS_Token currentToken = m_lLexer.tokens.Get(m_iPosition - 1);
		return expected.Contains(currentToken.type);
	}
	
	bool check(array<PS_ETokenType> expected)
	{
		if (m_iPosition >= m_lLexer.tokens.Count()) return false;
		PS_Token currentToken = m_lLexer.tokens.Get(m_iPosition);
		return expected.Contains(currentToken.type);
	}
	
	void MakeMatchError(PS_Token currentToken, array<PS_ETokenType> expected)
	{
		int position;
		if (currentToken) position = currentToken.position;
		else position = m_lLexer.code.Length();
		string codePart = m_lLexer.PeekBackCode(position, 60) + "#...";
		errorStr = "\n" + codePart + "\n";
		errorStr += "On pos " + position + " require: (";
		int i = 0;
		foreach (PS_ETokenType type: expected)
		{
			if (i > 0) errorStr += ", ";
			errorStr += PS_LanguageManager.GetInstance().GetTokenTypeName(type);
			i++;
		}
		errorStr += ") - Encountered: ";
		if (currentToken) errorStr += currentToken.GetTokenName();
		else errorStr += " End of script ";
		errorStr += "\n";
	}
}