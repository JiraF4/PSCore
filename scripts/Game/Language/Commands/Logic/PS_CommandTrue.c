class PS_CommandTrue : PS_Command // "true"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		return new PS_VariableBool(true);
	};
}