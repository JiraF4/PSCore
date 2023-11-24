class PS_CommandFalse : PS_Command // "false"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		return new PS_VariableBool(false);
	};
}