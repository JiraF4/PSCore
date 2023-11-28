class PS_CommandGreater : PS_Command // ">"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableNumber numberLeft = PS_VariableNumber.Cast(left);
		PS_VariableNumber numberRight = PS_VariableNumber.Cast(right);
		
		if (numberLeft && numberRight)
		{
			return new PS_VariableBool(numberLeft.m_mNumber > numberRight.m_mNumber);
		}
		
		return new PS_Variable();
	};
}