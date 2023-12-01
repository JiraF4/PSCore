class PS_VariableEntity : PS_Variable
{
	IEntity m_eEntity;
	
	override string GetValueName(bool arrayFirstScope = true)
	{
		return m_eEntity.ToString();
	}
	
	override void PrintVariable()
	{
		Print("ENTITY VARIABLE: " + m_eEntity.ToString());
	}
	
	void PS_VariableEntity(IEntity entity)
	{
		m_eEntity = entity;
	}
}