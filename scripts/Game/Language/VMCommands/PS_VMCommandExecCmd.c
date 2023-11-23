class PS_VMCommandExecCmd : PS_VMCommand
{
	ref PS_Command m_cCommand;
	
	override void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString() + " " + m_cCommand.ToString());
	}
	
	override void Execute(PS_VirtualMachine vm)
	{
		// Pop args from stack and push result
		PS_Variable result = m_cCommand.Execute(vm.PopStack(), vm.PopStack(), vm);
		vm.PushStack(result);
	}
	
	void PS_VMCommandExecCmd(PS_Command command)
	{
		m_cCommand = command;
	}
}