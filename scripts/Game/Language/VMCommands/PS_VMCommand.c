class PS_VMCommand
{
	void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString());
	}
	
	void Execute(PS_VirtualMachine vm)
	{
		
	}
}