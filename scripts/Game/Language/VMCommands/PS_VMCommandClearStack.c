class PS_VMCommandClearStack : PS_VMCommand
{
	override void Execute(PS_VirtualMachine vm)
	{
		vm.ClearStact();
	}
}

