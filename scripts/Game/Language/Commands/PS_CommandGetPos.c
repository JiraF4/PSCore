class PS_CommandGetPos : PS_Command // "getPos"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableEntity varEntity = PS_VariableEntity.Cast(right); 
		
		if (varEntity) {
			vector position = varEntity.m_eEntity.GetOrigin();
			PS_VariableArray arr = new PS_VariableArray();
			arr.Insert(new PS_VariableNumber(position[0]));
			arr.Insert(new PS_VariableNumber(position[1]));
			arr.Insert(new PS_VariableNumber(position[2]));
			return arr;
		}
		
		return new PS_Variable();
	};
}