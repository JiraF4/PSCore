class PS_VariableArray : PS_Variable
{
	ref array<ref PS_Variable> m_aArray = new array<ref PS_Variable>;
	
	private static ref array<PS_Variable> m_aVarsInArray; // recursion dirty hack
	override string GetValueName(bool arrayFirstScope = true)
	{
		if (arrayFirstScope) m_aVarsInArray = new array<PS_Variable>();
		m_aVarsInArray.Insert(this); // Exclude recursion
		string str = "[";
		bool firstAdded = false;
		foreach (PS_Variable var : m_aArray) {
			if (firstAdded) str += ", ";
			PS_VariableArray arr = PS_VariableArray.Cast(var);
			if (arr)
			{
				if (!m_aVarsInArray.Contains(arr))
				{
					str += arr.GetValueName(false);
				} else {
					str += "RECURS";
				}
			}
			else str += var.GetValueName(false);
			firstAdded = true;
		}
		str += "]";
		return str;
	}
	
	
	override void PrintVariable()
	{
		Print("ARRAY VARIABLE: " + m_aArray.ToString());
	}
	
	void Insert(PS_Variable var)
	{
		PS_VariableHolder holder = PS_VariableHolder.Cast(var);
		if (holder) m_aArray.Insert(holder.m_vVariable);
		else m_aArray.Insert(var);
	}
	
	void Set(int i, PS_Variable var)
	{
		m_aArray.Set(i, var);
	}
	
	int Count()
	{
		return m_aArray.Count();
	}
	
	PS_Variable Get(int i)
	{
		if (i < 0 || i >= m_aArray.Count()) return new PS_Variable();
		return m_aArray.Get(i);
	}
}