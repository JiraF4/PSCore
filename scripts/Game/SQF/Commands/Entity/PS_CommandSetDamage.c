class PS_CommandSetDamage : PS_Command // "setDamage"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableEntity varEntity = PS_VariableEntity.Cast(left); 
		PS_VariableNumber varNumber = PS_VariableNumber.Cast(right); 
		
		if (varEntity && varNumber) {
			SCR_CharacterDamageManagerComponent characterDamageManager = SCR_CharacterDamageManagerComponent.Cast(varEntity.m_eEntity.FindComponent(SCR_CharacterDamageManagerComponent));
			if (characterDamageManager)
			{
				HitZone bloodHitzone = characterDamageManager.GetBloodHitZone();
				if (bloodHitzone)
				{
					float bloodScale = 1 - varNumber.m_mNumber;
					float bloodLevel = Math.Clamp(bloodScale, 0, 1);
					bloodHitzone.SetHealthScaled(bloodLevel);
				}
			}
		}
		
		return new PS_Variable();
	};
}