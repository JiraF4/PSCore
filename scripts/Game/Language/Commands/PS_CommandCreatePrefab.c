class PS_CommandCreatePrefab : PS_Command // "createPrefab"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableArray argsArray = PS_VariableArray.Cast(right);
		
		if (argsArray)
		{
			PS_VariableString prefabPath = PS_VariableString.Cast(argsArray.Get(0));
			PS_VariableArray posArr = PS_VariableArray.Cast(argsArray.Get(1));
			if (prefabPath && posArr)
			{
				PS_VariableNumber x = PS_VariableNumber.Cast(posArr.Get(0));
				PS_VariableNumber y = PS_VariableNumber.Cast(posArr.Get(1));
				PS_VariableNumber z = PS_VariableNumber.Cast(posArr.Get(2));
				if (x && y && z)
				{
					ResourceName prefabPathRes = prefabPath.m_mString;
					vector origin = Vector(x.m_mNumber, y.m_mNumber, z.m_mNumber);
					
			        Resource resource = Resource.Load(prefabPathRes);
					EntitySpawnParams params = new EntitySpawnParams();
					params.Transform[3] = origin;
					IEntity entity = GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params);
					return new PS_VariableEntity(entity);
				}
			}
		}
		
		return new PS_Variable();
	};
}