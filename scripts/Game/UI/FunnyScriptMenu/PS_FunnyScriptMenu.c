class PS_FunnyScriptMenu : ChimeraMenuBase
{
	MultilineEditBoxWidget m_wMultilineEditBoxCode;
	ButtonWidget m_wRunButton;
	SCR_ButtonBaseComponent m_hRunButton;
		
	override void OnMenuInit() 
	{
		m_wMultilineEditBoxCode = MultilineEditBoxWidget.Cast(GetRootWidget().FindAnyWidget("MultilineEditBoxCode"));
		m_wRunButton = ButtonWidget.Cast(GetRootWidget().FindAnyWidget("RunButton"));
		m_hRunButton = SCR_ButtonBaseComponent.Cast(m_wRunButton.FindHandler(SCR_ButtonBaseComponent));
		m_hRunButton.m_OnClicked.Insert(ButtonClick);
	}
	
	void ButtonClick(SCR_ButtonBaseComponent runButton)
	{
		string code = m_wMultilineEditBoxCode.GetText();
		
		PS_LanguageManager.GetInstance().RunVM(code);
		
		Close();
	}
}