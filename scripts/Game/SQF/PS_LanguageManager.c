class PS_LanguageManagerClass: SCR_BaseGameModeComponentClass
{
	
};

class PS_LanguageManager : SCR_BaseGameModeComponent
{
	ref map<string, PS_ETokenType> m_mSingleCharTokens = new map<string, PS_ETokenType>();
	ref map<string, PS_ETokenType> m_mMultiCharTokens = new map<string, PS_ETokenType>();
	ref map<PS_ETokenType, string> m_mTokenTypeNames = new map<PS_ETokenType, string>();
	ref map<string, ref PS_Command> m_mCommands = new map<string, ref PS_Command>();
	
	ref array<ref PS_VirtualMachine> m_aVirtualMachines = new array<ref PS_VirtualMachine>();
	int m_vCurrentVirtualMachine = 0;
	
	// All global variables
	protected ref map<string, ref PS_VariableHolder> m_mVariables = new map<string, ref PS_VariableHolder>();
	
	void AddChatAction()
	{
		SCR_ChatPanelManager chatPanelManager = SCR_ChatPanelManager.GetInstance();
		ChatCommandInvoker invoker = chatPanelManager.GetCommandInvoker("sem");
		invoker.Insert(ScriptEditorMenu_Callback);
	}
	
	void ScriptEditorMenu_Callback(SCR_ChatPanel panel, string data)
	{
		GetGame().GetMenuManager().OpenMenu(ChimeraMenuPreset.ScriptEditorMenu);
	}
	
	int m_iLastCodeBlockId = 1;
	int NextCodeBlockId()
	{
		return m_iLastCodeBlockId++;
	}
	
	void SetVariable(string variableName, PS_Variable variable)
	{
		if (!m_mVariables.Contains(variableName)) 
			m_mVariables[variableName] = new PS_VariableHolder(variable);
		else
			m_mVariables[variableName].m_vVariable = variable;
	}
	
	PS_Variable GetVariableHolder(string variableName)
	{
		if (!m_mVariables.Contains(variableName)) SetVariable(variableName, new PS_Variable());
		return m_mVariables[variableName];
	}
	
	void VMLoop()
	{
		float timeBudget = 1;
		float startTime = System.GetTickCount();
		
		bool HasNotSleep = true; // If every machine sleep
		while (m_vCurrentVirtualMachine < m_aVirtualMachines.Count() && HasNotSleep)
		{
			HasNotSleep = false;
			m_vCurrentVirtualMachine++;
			if (m_vCurrentVirtualMachine >= m_aVirtualMachines.Count())
				m_vCurrentVirtualMachine = 0;
			
			while (!m_aVirtualMachines[m_vCurrentVirtualMachine].IsSleep())
			{
				HasNotSleep = true;
				if (m_aVirtualMachines[m_vCurrentVirtualMachine].ExecuteNext())
				{
					m_aVirtualMachines.Remove(m_vCurrentVirtualMachine);
					break;
				} else {
					float time = System.GetTickCount();
					if (time - timeBudget > startTime) break;
				}
			}
			float time = System.GetTickCount();
			if (time - timeBudget > startTime) break;
		}
		
		GetGame().GetCallqueue().CallLater(VMLoop, 0);
	}
	
	PS_VirtualMachine PrepareVM(string code)
	{
		if (code == "") return null;
		PS_Lexer lexer = new PS_Lexer(code);
		if (lexer.errorStr == "")
		{
			//lexer.PrintTokens();
			PS_Parser parser = new PS_Parser(lexer);
			if (parser.m_lRoot) {
				//parser.m_lRoot.PrintNode(0);
				return new PS_VirtualMachine(parser.m_lRoot);
			}
		}
		return null;
	}
	
	bool IsVMRun(PS_VirtualMachine vm)
	{
		return m_aVirtualMachines.Contains(vm);
	}
	
	void RunVM(PS_VirtualMachine vm)
	{
		if (m_aVirtualMachines.Contains(vm)) return;
		vm.RunScheduled();
		m_aVirtualMachines.Insert(vm);
	}
	
	void RunVM(string code)
	{
		PS_VirtualMachine vm = PrepareVM(code);
		if (vm) RunVM(vm);
	}
	
	// more singletons for singletons god, make our spagetie kingdom great
	static PS_LanguageManager GetInstance() 
	{
		BaseGameMode gameMode = GetGame().GetGameMode();
		if (gameMode)
			return PS_LanguageManager.Cast(gameMode.FindComponent(PS_LanguageManager));
		else
			return null;
	}
	
	override void OnPostInit(IEntity owner)
	{
		//string stringTest = "_arr = [[1, 2, 3, 4, 5], [1, 2, 3, 4], [1, 2]];print _arr;";
		
		//RunVM(stringTest);
		
		GetGame().GetCallqueue().CallLater(VMLoop, 0);
		GetGame().GetCallqueue().CallLater(AddMsgAction, 0);
		GetGame().GetCallqueue().CallLater(AddChatAction, 0);
	}
	
	void AddMsgAction()
	{
		SCR_ChatPanelManager chatPanelManager = SCR_ChatPanelManager.GetInstance();
		ChatCommandInvoker invoker = chatPanelManager.GetCommandInvoker("run");
		invoker.Insert(SendBigMsg_runVM);
	}
	
	void SendBigMsg_runVM(SCR_ChatPanel panel, string data)
	{
		RunVM(data);//
	}
	
	/*
	void Execute(string code)
	{
		PS_Lexer lexer = new PS_Lexer(code);
		
		string output = "";
		foreach (PS_Token token: lexer.tokens)
		{
    		output += string.Format("<tr><td>%1</td><td>%2</td><td>%3</td></tr>", token.type, token.text, token.position);
		}
		//PrintFormat("%1", output);
		
		PS_Parser parser = new PS_Parser(lexer);
		if (!parser.m_lRoot)
		{
			Print(parser.errorStr);
			return;
		}
		//parser.m_lRoot.PrintNode(0);
		PS_Variable variable = parser.m_lRoot.Execute();
		//variable.PrintVariable();
	}
	*/
	
	void PS_LanguageManager(IEntityComponentSource src, IEntity ent, IEntity parent)
	{
		registerSingleCharTokens();
		registerMultiCharTokens();
		registerCommands();
		
		m_mTokenTypeNames[PS_ETokenType.VARIABLELOCAL] = "Var";
		m_mTokenTypeNames[PS_ETokenType.GLOBALVARIABLE] = "Global";
		m_mTokenTypeNames[PS_ETokenType.COMMAND] = "Cmd";
		m_mTokenTypeNames[PS_ETokenType.STRING] = "Str";
		m_mTokenTypeNames[PS_ETokenType.NUMBER] = "Number";
		m_mTokenTypeNames[PS_ETokenType.EMPTY] = "NULL";
	}
	
	void registerCommands()
	{
		m_mCommands.Insert("=", new PS_CommandAssing(4, 1, 1, 0));
		m_mCommands.Insert(">", new PS_CommandGreater(4, 1, 1));
		m_mCommands.Insert("<", new PS_CommandLess(4, 1, 1));
		m_mCommands.Insert("+", new PS_CommandAdd(3, 1, 1));
		m_mCommands.Insert("*", new PS_CommandMul(2, 1, 1));
		m_mCommands.Insert("-", new PS_CommandSub(3, 1, 1));
		m_mCommands.Insert("/", new PS_CommandDiv(2, 1, 1));
		m_mCommands.Insert("sqrt", new PS_CommandSqrt(1, 0, 1));
		m_mCommands.Insert("print", new PS_CommandPrint(5, 0, 1));
		m_mCommands.Insert("fancyPrint", new PS_CommandFancyPrint(5, 0, 1));
		m_mCommands.Insert("sleep", new PS_CommandSleep(5, 0, 1));
		m_mCommands.Insert("count", new PS_CommandCount(1, 0, 1));
		m_mCommands.Insert("select", new PS_CommandSelect(4, 1, 1));
		m_mCommands.Insert("cameraPosition", new PS_CommandCameraPosition(1, 0, 0));
		m_mCommands.Insert("getPos", new PS_CommandGetPos(1, 0, 1));
		m_mCommands.Insert("createPrefab", new PS_CommandCreatePrefab(1, 0, 1));
		m_mCommands.Insert("true", new PS_CommandTrue(1, 0, 0));
		m_mCommands.Insert("false", new PS_CommandFalse(1, 0, 0));
		m_mCommands.Insert("setDamage", new PS_CommandSetDamage(4, 1, 1));
		m_mCommands.Insert("ForEach", new PS_CommandForEach(4, 1, 1));
		m_mCommands.Insert("getLoadout", new PS_CommandGetLoadout(2, 0, 1));
	};
	
	void registerMultiCharTokens()
	{
		AddToken("if", PS_ETokenType.IF, false);
		AddToken("then", PS_ETokenType.THEN, false);
		AddToken("else", PS_ETokenType.ELSE, false);
		AddToken("while", PS_ETokenType.WHILE, false);
		AddToken("do", PS_ETokenType.DO, false);
	}
	
	void registerSingleCharTokens()
	{
		AddToken(";", PS_ETokenType.SEMICOLON, true);
		AddToken("(", PS_ETokenType.LEFTPARENTHISISROUND, true);
		AddToken(")", PS_ETokenType.RIGHTPARENTHISISROUND, true);
		AddToken("[", PS_ETokenType.LEFTPARENTHISISSQUARE, true);
		AddToken("]", PS_ETokenType.RIGHTPARENTHISISSQUARE, true);
		AddToken("{", PS_ETokenType.LEFTPARENTHISISFIGURE, true);
		AddToken("}", PS_ETokenType.RIGHTPARENTHISISFIGURE, true);
		AddToken(",", PS_ETokenType.COMMA, true);
	}
	
	void AddToken(string name, PS_ETokenType type, bool singleChar)
	{
		if (singleChar) m_mSingleCharTokens[name] = type;
		else m_mMultiCharTokens[name] = type;
		m_mTokenTypeNames[type] = "\"" + name + "\"";
	}
	
	string GetTokenTypeName(PS_ETokenType type)
	{
		return m_mTokenTypeNames[type];
	}
};