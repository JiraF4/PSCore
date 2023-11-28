class PS_CommandSelect : PS_Command // "select"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableArray arrayleft = PS_VariableArray.Cast(left);
		PS_VariableNumber numberRight = PS_VariableNumber.Cast(right);
		
		if (arrayleft && numberRight)
		{
			return arrayleft.Get(numberRight.m_mNumber);
		}
		
		return new PS_Variable();
	};
}