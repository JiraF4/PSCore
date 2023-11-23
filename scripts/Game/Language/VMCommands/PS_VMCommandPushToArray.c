class PS_VMCommandPushToArray : PS_VMCommand
{
	ref PS_Variable m_vVariable;
	
	override void Execute(PS_VirtualMachine vm)
	{
		PS_Variable var = vm.PopStack();
		PS_VariableArray arr = PS_VariableArray.Cast(vm.PopStack());
		if (arr)
			arr.Insert(var);
	}
	
	void PS_VMCommandPushConst(PS_Variable variable)
	{
		m_vVariable = variable;
	}
}