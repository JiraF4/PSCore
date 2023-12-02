class PS_CommandForEach : PS_Command // "forEach"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableCodeBlock codeBlockLeft = PS_VariableCodeBlock.Cast(left);
		PS_VariableArray arrayRight = PS_VariableArray.Cast(right);
		
		if (codeBlockLeft && arrayRight)
		{
			vm.SetItterator(new PS_VirtualMachineItterator(arrayRight, codeBlockLeft));
		}
		PS_VirtualMachineItterator itterator = vm.GetItterator();
		if (itterator)
		{
			vm.JumpRelative(-1);
			if (itterator.RunNext(vm))
			{
				vm.SetItterator(null);
			}
		}
		
		/* trash
		int itterator = vm.GetItterator();
		if (itterator > 0)
			arrayRight = 
		if (codeBlockLeft && arrayRight)
		{
			if (itterator < arrayRight.m_aArray.Count())
			{
				vm.JumpRelative(-1); // Move back for next cycle
				if (itterator == 0){
					arrayRight.SetItteratorArray(arrayRight);
				}
				vm.SetItterator(vm.GetItterator() + 1);
				vm.SetVariable("_x", arrayRight.Get(itterator));
				vm.RunCodeBlock(codeBlockLeft.m_cCodeBlock);
			}
			else
			{
				// Reset itterator
				vm.SetItterator(0);
			}
		}
		*/
		
		return new PS_Variable();
	};
}