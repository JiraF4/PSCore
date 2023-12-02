class PS_CommandFancyPrint : PS_Command // "fancyPrint"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		string val = right.GetValueName(true);
		Print(val + "");
		return right;
	};
}
