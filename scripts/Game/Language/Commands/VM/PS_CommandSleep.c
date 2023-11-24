class PS_CommandSleep : PS_Command // "sleep"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableNumber numberRight = PS_VariableNumber.Cast(right);
		
		if (numberRight)
		{
			vm.Sleep(numberRight.m_mNumber);
		}
		
		return new PS_Variable();
	};
}

