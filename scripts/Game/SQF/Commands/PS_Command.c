class PS_Command
{
	int m_mOrder = 0;
	bool m_bHasLeftOperand = false;
	bool m_bHasRightOperand = false;
	bool m_bUnholdLeftOperand = false;
	bool m_bUnholdRightOperand = false;
	
	int GetOrder()
	{
		return m_mOrder;
	}
	
	void PS_Command(int order, bool hasLeftOperand, bool hasRightOperand, bool unholdLeftOperand = true, bool unholdRightOperand = true)
	{
		m_mOrder = order;
		m_bHasLeftOperand = hasLeftOperand;
		m_bHasRightOperand = hasRightOperand;
		m_bUnholdLeftOperand = unholdLeftOperand;
		m_bUnholdRightOperand = unholdRightOperand;
	}
	
	PS_Variable Execute(PS_Variable left, PS_Variable right, PS_VirtualMachine vm)
	{
		if (m_bUnholdLeftOperand)
		{
			PS_VariableHolder holderLeft = PS_VariableHolder.Cast(left);
			if (holderLeft) left = holderLeft.m_vVariable;
		}
		if (m_bUnholdRightOperand)
		{
			PS_VariableHolder holderRight = PS_VariableHolder.Cast(right);
			if (holderRight) right = holderRight.m_vVariable;
		}
		return ExecuteUnholded(left, right, vm);
	};
	PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) 
	{
		return new PS_Variable();
	};
}