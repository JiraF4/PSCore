// Action scripted through SQF
class PS_SQFScriptedAction : ScriptedUserAction
{	
	[Attribute("")]
	string m_sVisibilityConditionScriptSQF;
	[Attribute("")]
	string m_sPerformActionScriptSQF;
	[Attribute("")]
	string m_sGetActionNameScriptSQF;
	
	protected ref PS_VirtualMachine m_vVisibilityConditionVM;
	protected ref PS_VirtualMachine m_sPerformActionVM;
	protected ref PS_VirtualMachine m_sGetActionNameVM;
	
	override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
	{
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		m_vVisibilityConditionVM = languageManager.PrepareVM(m_sVisibilityConditionScriptSQF);
		m_sPerformActionVM = languageManager.PrepareVM(m_sPerformActionScriptSQF);
		m_sGetActionNameVM = languageManager.PrepareVM(m_sGetActionNameScriptSQF);
	}
	
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
	{
		if (!m_sPerformActionVM) return;
		PS_VariableArray thisArr = new PS_VariableArray();
		thisArr.Insert(PS_VariableEntity(pOwnerEntity));
		thisArr.Insert(PS_VariableEntity(pUserEntity));
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		m_sPerformActionVM.SetVariable("_this", thisArr);
		languageManager.RunVM(m_sPerformActionVM);
	}
	
	override bool GetActionNameScript(out string outName)
	{
		if (!m_sGetActionNameVM) return false;
		PS_Variable var = m_sGetActionNameVM.RunUnscheduled();
		PS_VariableString varStr = PS_VariableString.Cast(var);
		if (varStr) {
			outName = varStr.m_mString;
			return true;
		}
		return false;
	}
	
	override bool CanBePerformedScript(IEntity user)
	{
		if (!m_vVisibilityConditionVM) return false;
		PS_Variable var = m_vVisibilityConditionVM.RunUnscheduled();
		PS_VariableBool varBool = PS_VariableBool.Cast(var);
		if (varBool) return varBool.m_mBool;
		return false;
	}
};