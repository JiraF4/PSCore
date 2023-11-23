class PS_CommandAssing : PS_Command // "="
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableHolder holderLeft = PS_VariableHolder.Cast(left);
		if (holderLeft) 
			holderLeft.m_vVariable = right;
		return left;
	}
}