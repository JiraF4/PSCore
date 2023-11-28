class PS_VMCommandRunCodeBlockConditional : PS_VMCommand
{
	override void Execute(PS_VirtualMachine vm)
	{
		// Push blocks and bool first
		PS_VariableCodeBlock varCodeBlockFalse = PS_VariableCodeBlock.Cast(vm.PopStack());
		PS_VariableCodeBlock varCodeBlockTrue = PS_VariableCodeBlock.Cast(vm.PopStack());
		PS_VariableBool varBool = PS_VariableBool.Cast(vm.PopStack());
		if (varBool && varBool.m_mBool && varCodeBlockTrue)
			vm.RunCodeBlock(varCodeBlockTrue.m_cCodeBlock);
		else if (varCodeBlockFalse)
			vm.RunCodeBlock(varCodeBlockFalse.m_cCodeBlock);
	}
}