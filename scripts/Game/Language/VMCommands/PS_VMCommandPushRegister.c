class PS_VMCommandPushRegister : PS_VMCommand
{
	override void Execute(PS_VirtualMachine vm)
	{
		vm.PushStack(vm.m_vRegister);
	}
}