class PS_CommandAdd : PS_Command // "+"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableNumber numberLeft = PS_VariableNumber.Cast(left);
		PS_VariableNumber numberRight = PS_VariableNumber.Cast(right);
		PS_VariableString strLeft = PS_VariableString.Cast(left);
		PS_VariableString strRight = PS_VariableString.Cast(right);
		PS_VariableArray arrLeft = PS_VariableArray.Cast(left);
		PS_VariableArray arrRight = PS_VariableArray.Cast(right);
		
		if (numberLeft && numberRight)
		{
			return new PS_VariableNumber(numberLeft.m_mNumber + numberRight.m_mNumber);
		}
		
		if (strLeft && numberRight)
		{
			return new PS_VariableString(strLeft.m_mString + numberRight.m_mNumber.ToString());
		}
		if (numberLeft && strRight)
		{
			return new PS_VariableString(numberLeft.m_mNumber.ToString() + strRight.m_mString);
		}
		
		if (arrLeft && !arrRight)
		{
			arrLeft.m_aArray.Insert(right);
			return arrLeft;
		}
		
		if (arrLeft && arrRight)
		{
			for ( int i = 0; i < arrRight.m_aArray.Count(); i++ )
			{
				arrLeft.m_aArray.Insert(arrRight.m_aArray.Get(i));
			}
			return arrLeft;
		}
		
		return new PS_Variable();
	};
}