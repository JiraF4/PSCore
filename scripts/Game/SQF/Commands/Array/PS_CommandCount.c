class PS_CommandCount : PS_Command // "count"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableArray arr = PS_VariableArray.Cast(right);
		
		if (arr)
		{
			return new PS_VariableNumber(arr.Count()); 
		}
		
		return right;
	};
}



