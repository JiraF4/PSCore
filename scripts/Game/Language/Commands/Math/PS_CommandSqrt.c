class PS_CommandSqrt : PS_Command // "sqrt"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableNumber numberRight = PS_VariableNumber.Cast(right);
		
		if (numberRight)
		{
			return new PS_VariableNumber(Math.Sqrt(numberRight.m_mNumber));
		}
		
		return new PS_Variable();
	};
}

