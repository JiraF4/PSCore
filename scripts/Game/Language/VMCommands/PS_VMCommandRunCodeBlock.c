class PS_VMCommandRunCodeBlock : PS_VMCommand
{
	override void Execute(PS_VirtualMachine vm)
	{
		PS_VariableCodeBlock varCodeBlock = PS_VariableCodeBlock.Cast(vm.PopStack());
		if (varCodeBlock) {
			vm.RunCodeBlock(varCodeBlock.m_cCodeBlock);
		}
	}
}