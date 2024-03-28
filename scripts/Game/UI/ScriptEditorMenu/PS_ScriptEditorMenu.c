modded enum ChimeraMenuPreset
{
	ScriptEditorMenu
}

class PS_ScriptEditorMenu : ChimeraMenuBase
{
	protected Widget m_wRoot;
	protected RichTextWidget m_wScriptRichText;
	protected MultilineEditBoxWidget m_wScriptEditBox;
	protected string m_sOldText;
	
	protected ButtonWidget m_wExecuteButton;
	protected SCR_ButtonBaseComponent m_hExecuteButton;
	
	override void OnMenuInit() 
	{
		super.OnMenuInit();
		
		m_wRoot = GetRootWidget();
		m_wScriptRichText = RichTextWidget.Cast(m_wRoot.FindAnyWidget("ScriptRichText"));
		m_wScriptEditBox = MultilineEditBoxWidget.Cast(m_wRoot.FindAnyWidget("ScriptEditBox"));
		m_wExecuteButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("ExecuteButton"));
		
		m_hExecuteButton = SCR_ButtonBaseComponent.Cast(m_wExecuteButton.FindHandler(SCR_ButtonBaseComponent));
		
		m_hExecuteButton.m_OnClicked.Insert(ExecuteCodeButton);
	}
	
	override void OnMenuOpened()
	{
		super.OnMenuOpened();
		
	}
	
	override void OnMenuUpdate(float tDelta)
	{
		super.OnMenuUpdate(tDelta);
		
		string text = m_wScriptEditBox.GetText();
		if (m_sOldText == text)
			return;
		m_sOldText = text;
		
		bool inString = false;
		string lexerText = "";
		int oldPosition = 0;
		PS_Lexer lexer = new PS_Lexer(text);
		foreach (PS_Token token : lexer.tokens)
		{
			int position = token.position;
			string part = text.Substring(oldPosition, position - oldPosition);
			oldPosition = position;
			
			PS_ETokenType tokenType = token.type;
			string color = "128, 128, 128, 255";
			switch (tokenType)
			{
				case PS_ETokenType.COMMA:
				case PS_ETokenType.SEMICOLON:
				case PS_ETokenType.NUMBER:
					color = "213,223,240,255";
					break;
				case PS_ETokenType.WHILE:
				case PS_ETokenType.DO:
				case PS_ETokenType.THEN:
				case PS_ETokenType.ELSE:
				case PS_ETokenType.IF:
					color = "89,166,233,255";
					break;
				case PS_ETokenType.LEFTPARENTHISISROUND:
				case PS_ETokenType.RIGHTPARENTHISISROUND:
				case PS_ETokenType.LEFTPARENTHISISFIGURE:
				case PS_ETokenType.RIGHTPARENTHISISFIGURE:
				case PS_ETokenType.LEFTPARENTHISISSQUARE:
				case PS_ETokenType.RIGHTPARENTHISISSQUARE:
					color = "213,223,240,255";
					break;
				case PS_ETokenType.VARIABLELOCAL:
					color = "213,223,240,255";
					break;
				case PS_ETokenType.COMMAND:
					if (token.text.Length() == 1)
						color = "213,223,240,255";
					else
						color = "243,173,88,255";
					break;
				case PS_ETokenType.GLOBALVARIABLE:
					color = "64,181,172,255";
					break;
				case PS_ETokenType.STRING:
					color = "193,120,221,255";
					break;
			}
			
			string fColor = string.Format("<color rgba='%1'>", color);
			string eColor = "</color>";
			
			lexerText = string.Format("%1%2%3%4", lexerText, fColor, part, eColor);
		}
		
		m_wScriptRichText.SetText(lexerText);
	}
	
	void ExecuteCodeButton(SCR_ButtonBaseComponent button)
	{
		
	}
}