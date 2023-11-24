class PS_VMCommandPushVarGlobal : PS_VMCommand
{
	string m_sVariableName;
	
	override void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString() + " " + m_sVariableName);
	}
	
	override void Execute(PS_VirtualMachine vm)
	{
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		vm.PushStack(languageManager.GetVariableHolder(m_sVariableName));
	}
	
	void PS_VMCommandPushVarGlobal(string variableName)
	{
		m_sVariableName = variableName;
	}
}