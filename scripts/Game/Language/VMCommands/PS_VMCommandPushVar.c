class PS_VMCommandPushVar : PS_VMCommand
{
	string m_sVariableName;
	
	override void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString() + " " + m_sVariableName);
	}
	
	override void Execute(PS_VirtualMachine vm)
	{
		vm.PushStack(vm.GetVariableHolder(m_sVariableName));
	}
	
	void PS_VMCommandPushVar(string variableName)
	{
		m_sVariableName = variableName;
	}
}