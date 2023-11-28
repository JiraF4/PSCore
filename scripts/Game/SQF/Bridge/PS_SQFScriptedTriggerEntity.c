[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class PS_SQFScriptedTriggerEntityClass : SCR_BaseTriggerEntityClass
{

};


class PS_SQFScriptedTriggerEntity : SCR_BaseTriggerEntity
{
	ref PS_VariableArray entitiesList = new PS_VariableArray();
	
	[Attribute("")]
	string m_sEntityProcessScriptSQF;
	
	protected ref PS_VirtualMachine m_vEntityProcessVM;
	
	override void OnInit(IEntity owner)
	{
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		m_vEntityProcessVM = languageManager.PrepareVM(m_sEntityProcessScriptSQF);
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent) { 
		if (!m_vEntityProcessVM) return true;
		
		entitiesList.Insert(new PS_VariableEntity(ent));
		
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		PS_VariableArray arr = new PS_VariableArray();
		arr.Insert(new PS_VariableEntity(ent));
		m_vEntityProcessVM.SetVariable("_this", arr);
		languageManager.RunVM(m_vEntityProcessVM);
		
		
		
		return false;
	};
	
	override protected event void OnActivate(IEntity ent)
	{
		super.OnActivate(ent);
		Print("!!!");
	}
}
