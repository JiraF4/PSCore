[ComponentEditorProps(category: "GameScripted/Editor (Editables)", description: "", icon: "WBData/ComponentEditorProps/componentEditor.png")]
class PS_EditableTriggerComponentClass: SCR_EditableDescriptorComponentClass
{
	
};

class PS_EditableTriggerComponent: SCR_EditableSystemComponent
{
	PS_SQFScriptedTriggerEntity m_eSQFTrigger;
	
	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		m_eSQFTrigger = PS_SQFScriptedTriggerEntity.Cast(owner);
	}
	
	float GetRadius()
	{
		return m_eSQFTrigger.GetSphereRadius();
	}
	
	void SetRadius(float radius)
	{
		m_eSQFTrigger.SetRadius(radius);
	}
	
	string GetActivateScript()
	{
		return m_eSQFTrigger.GetActivateScript();
	}
	
	void SetActivateScript(string activateScript)
	{
		m_eSQFTrigger.SetActivateScript(activateScript);
	}
};



