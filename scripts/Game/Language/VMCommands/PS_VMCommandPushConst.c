class PS_VMCommandPushConst : PS_VMCommand
{
	ref PS_Variable m_vVariable;
	
	override void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString() + " " + m_vVariable.ToString());
		PS_VariableCodeBlock codeBlock = PS_VariableCodeBlock.Cast(m_vVariable);
		if (codeBlock)
			codeBlock.m_cCodeBlock.PrintCommandList(level + 1);
	}
	
	override void Execute(PS_VirtualMachine vm)
	{
		vm.PushStack(m_vVariable);
	}
	
	void PS_VMCommandPushConst(PS_Variable variable)
	{
		m_vVariable = variable;
	}
}