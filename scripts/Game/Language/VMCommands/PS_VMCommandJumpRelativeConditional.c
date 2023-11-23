// Position still increase after jump
class PS_VMCommandJumpRelativeConditional : PS_VMCommand
{
	int m_iDistance;
	
	override void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString() + " " + m_iDistance.ToString());
	}
	
	override void Execute(PS_VirtualMachine vm)
	{
		PS_VariableBool varBool = PS_VariableBool.Cast(vm.PopStack());
		if (varBool.m_mBool)
			vm.JumpRelative(m_iDistance);
	}
	
	void PS_VMCommandJumpRelativeConditional(int distance)
	{
		m_iDistance = distance;
	}
}