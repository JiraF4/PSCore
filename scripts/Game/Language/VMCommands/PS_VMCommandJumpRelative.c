// Position still increase after jump
class PS_VMCommandJumpRelative : PS_VMCommand
{
	int m_iDistance;
	
	override void PrintCommand(int level)
	{
		Print(level.ToString(level+1) + " " + this.ToString() + " " + m_iDistance.ToString());
	}
	
	override void Execute(PS_VirtualMachine vm)
	{
		vm.JumpRelative(m_iDistance);
	}
	
	void PS_VMCommandJumpRelative(int distance)
	{
		m_iDistance = distance;
	}
}


