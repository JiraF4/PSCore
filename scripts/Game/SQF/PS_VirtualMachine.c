class PS_VirtualMachine
{
	// All local variables
	protected ref map<string, ref PS_VariableHolder> m_mVariables = new map<string, ref PS_VariableHolder>();
	
	// TODO: move all stacks to vm state class
	ref PS_VMCodeBlock m_cRootCodeBlock;
	ref PS_LanguageNodeCodeBlock m_nRootNode;
	
	ref array<ref PS_VirtualMachineState> m_aStates = new array<ref PS_VirtualMachineState>();
	
	ref array<ref PS_Variable> m_aVarStack = new array<ref PS_Variable>();
	ref PS_Variable m_vRegister;
	
	int m_iSleepTime = 0;
	
	int m_iBackwardJumpCounter = 0;
	int m_iMaxBackwardJump = 10000;
	
	int m_iMaxCallstack = 100;
	
	bool killed = false;
	
	// If script unscheduled you can't crete new VM through script
	bool m_bScheduled = false;
	
	PS_VirtualMachineState GetSate()
	{
		return m_aStates[m_aStates.Count() - 1];
	}
	
	PS_VirtualMachineItterator GetItterator()
	{
		return GetSate().m_iItterator;
	}
	
	void SetItterator(PS_VirtualMachineItterator iterrator)
	{
		GetSate().m_iItterator = iterrator;
	}
	
	
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
		GetSate().m_aPointer = GetSate().m_aPointer + distance;
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
		m_aStates.Insert(new PS_VirtualMachineState(codeBlock));
		if (m_iMaxCallstack < m_aStates.Count()) // Prevent infinity recursion, still possible throug spawn :(
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
		m_aStates.Clear();
		m_aStates.Insert(new PS_VirtualMachineState(m_cRootCodeBlock));
		
		m_aVarStack.Clear();
		m_vRegister = new PS_Variable();
		
		m_iBackwardJumpCounter = 0;
		
		killed = false;
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
	
	// TODO: refactor
	bool ExecuteNext()
	{
		if (m_aStates.IsEmpty() || killed)
		{
			//Print("KILL");
			return true;
		}
		int callStactNum = m_aStates.Count() - 1;
		PS_VMCodeBlock codeBlock = m_aStates.Get(callStactNum).m_cCodeBlock;
		int pointerNum = m_aStates.Get(callStactNum).m_aPointer;
		if (pointerNum >= codeBlock.Count())
		{
			m_aStates.Remove(callStactNum);
		} else {
			PS_VMCommand node = codeBlock.Get(pointerNum);
			//node.PrintCommand(-1);
			node.Execute(this);
			m_aStates.Get(callStactNum).m_aPointer = m_aStates.Get(callStactNum).m_aPointer + 1;
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

class PS_VirtualMachineState
{
	ref PS_VirtualMachineItterator m_iItterator;
	int m_aPointer;
	ref PS_VMCodeBlock m_cCodeBlock;
	
	void PS_VirtualMachineState(PS_VMCodeBlock codeBlock)
	{
		m_aPointer = 0;
		m_cCodeBlock = codeBlock;
	}
}

class PS_VirtualMachineItterator
{
	int m_iPosition;
	ref PS_VariableArray m_aArray;
	ref PS_VariableCodeBlock m_cCodeBlock;
	
	void PS_VirtualMachineItterator(PS_VariableArray variableArray, PS_VariableCodeBlock codeBlock)
	{
		m_iPosition = 0;
		m_aArray = variableArray;
		m_cCodeBlock = codeBlock;
	}
	
	bool RunNext(PS_VirtualMachine vm)
	{
		if (m_iPosition >= m_aArray.Count())
			return true; // Exit iterrator loop
		
		vm.SetVariable("_x", m_aArray.Get(m_iPosition));
		vm.RunCodeBlock(m_cCodeBlock.m_cCodeBlock);
		m_iPosition++;
		return false;
	}
}







