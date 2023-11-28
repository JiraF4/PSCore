class PS_VirtualMachine
{
	// All local variables
	protected ref map<string, ref PS_VariableHolder> m_mVariables = new map<string, ref PS_VariableHolder>();
	
	ref PS_VMCodeBlock m_cRootCodeBlock;
	protected ref array<ref PS_VMCodeBlock> m_aCallstack = new array<ref PS_VMCodeBlock>();
	protected ref array<int> m_aPointers = new array<int>();
	ref PS_LanguageNodeCodeBlock m_nRootNode;
	
	ref array<ref PS_Variable> m_aVarStack = new array<ref PS_Variable>();
	ref PS_Variable m_vRegister;
	
	int m_iSleepTime = 0;
	
	int m_iBackwardJumpCounter = 0;
	int m_iMaxBackwardJump = 10000;
	
	int m_iMaxCallstack = 100;
	
	bool killed = false;
	
	// If script unscheduled you can't crete new VM through script
	bool m_bScheduled = false;
	
	void KillVM()
	{
		// VM been removed on nextframe
		killed = true;
	}
	
	void Sleep(float timeSecods)
	{
		m_iSleepTime = GetGame().GetWorld().GetWorldTime() + 1000 * timeSecods;
	}
	
	void ClearStact()
	{
		m_aVarStack.Clear();
	}
	
	void PushStack(PS_Variable variable)
	{
		m_vRegister = variable;
		m_aVarStack.Insert(variable);
	}
	
	PS_Variable PopStack()
	{
		if (m_aVarStack.IsEmpty()) return new PS_Variable();
		m_vRegister = m_aVarStack.Get(m_aVarStack.Count() - 1);
		m_aVarStack.Remove(m_aVarStack.Count() - 1);
		return m_vRegister;
	}
	
	void JumpRelative(int distance)
	{
		if (distance < 0) { // Prevent infinity loops
			m_iBackwardJumpCounter++;
			if (m_iBackwardJumpCounter > m_iMaxBackwardJump)
			{
				Print("Max jumps count reached. (Infinity loop?)");
				KillVM();
			}
		}
		m_aPointers[m_aPointers.Count() - 1] = m_aPointers[m_aPointers.Count() - 1] + distance;
	}
	
	void SetVariable(string variableName, PS_Variable variable)
	{
		if (!m_mVariables.Contains(variableName)) 
			m_mVariables[variableName] = new PS_VariableHolder(variable);
		else
			m_mVariables[variableName].m_vVariable = variable;
	}
	
	PS_VariableHolder GetVariableHolder(string variableName)
	{
		if (!m_mVariables.Contains(variableName)) SetVariable(variableName, new PS_Variable());
		return m_mVariables[variableName];
	}
	
	void RunCodeBlock(PS_VMCodeBlock codeBlock)
	{
		m_aCallstack.Insert(codeBlock);
		m_aPointers.Insert(0);
		if (m_iMaxCallstack > m_aCallstack.Count()) // Prevent infinity recursion, still possible throug spawn :(
		{
			Print("Call stack overflow. (Infinity recursion?)");
			KillVM();
		}
	}
	
	bool IsSleep()
	{
		return m_iSleepTime > GetGame().GetWorld().GetWorldTime();
	}
	
	// Run in one shoot
	void Restart()
	{
		m_aCallstack.Clear();
		m_aPointers.Clear();
		m_aVarStack.Clear();
		
		m_aCallstack.Insert(m_cRootCodeBlock);
		m_aPointers.Insert(0);
		m_vRegister = new PS_Variable();
	}
	
	PS_Variable RunUnscheduled()
	{
		m_bScheduled = false;
		Restart();
		while (!ExecuteNext())
		{
		}
		return m_vRegister;
	}
	
	void RunScheduled()
	{
		m_bScheduled = true;
		Restart();
	}
	
	bool ExecuteNext()
	{
		if (m_aCallstack.IsEmpty() || killed)
			return true;
		int callStactNum = m_aCallstack.Count() - 1;
		PS_VMCodeBlock codeBlock = m_aCallstack.Get(callStactNum);
		int pointerNum = m_aPointers[callStactNum];
		if (pointerNum >= codeBlock.Count())
		{
			m_aCallstack.Remove(callStactNum);
			m_aPointers.Remove(callStactNum);
		} else {
			PS_VMCommand node = codeBlock.Get(pointerNum);
			node.Execute(this);
			m_aPointers[callStactNum] = m_aPointers[callStactNum] + 1;
		}
		return false;
	}
	
	void PS_VirtualMachine(PS_LanguageNodeCodeBlock rootNode)
	{
		m_nRootNode = rootNode;
		PS_VMCodeBlock codeBlock = rootNode.CompileBlock();
		m_cRootCodeBlock = codeBlock;
		//codeBlock.PrintCommandList(0);
	}
}