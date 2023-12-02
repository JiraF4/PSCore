[EntityEditorProps(category: "GameScripted/Triggers", description: "")]
class PS_SQFScriptedTriggerEntityClass : SCR_BaseTriggerEntityClass
{
	
};


class PS_SQFScriptedTriggerEntity : SCR_BaseTriggerEntity
{
	ref PS_VariableArray entitiesList = new PS_VariableArray();
	
	[Attribute("")]
	string m_sActivateScriptSQF;
	
	protected ref PS_VirtualMachine m_vActivateVM;
	
	string GetActivateScript()
	{
		return m_sActivateScriptSQF;
	}
	
	void SetActivateScript(string activateScript)
	{
		RPC_SetActivateScript(activateScript);
		Rpc(RPC_SetActivateScript, activateScript);
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RPC_SetActivateScript(string activateScript)
	{
		m_sActivateScriptSQF = activateScript;
		CompileVM();
	}
	
	float GetRadius()
	{
		return GetSphereRadius();
	}
	
	void SetRadius(float radius)
	{
		SetSphereRadius(radius);
		
		PS_TriggerZoneAreaMeshComponent triggerZoneAreaMeshComponent = PS_TriggerZoneAreaMeshComponent.Cast(FindComponent(PS_TriggerZoneAreaMeshComponent));
		if (triggerZoneAreaMeshComponent)
			triggerZoneAreaMeshComponent.GenerateAreaMesh();
	}
	
	override void OnInit(IEntity owner)
	{
		CompileVM();
	}
	
	void CompileVM()
	{
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		m_vActivateVM = languageManager.PrepareVM(m_sActivateScriptSQF);
	}
	
	override bool ScriptedEntityFilterForQuery(IEntity ent) { 
		if (!m_vActivateVM) return false;
		
		entitiesList.Insert(new PS_VariableEntity(ent));
		
		return false;
	};
	
	override event protected void OnQueryFinished(bool bIsEmpty)
	{
		super.OnQueryFinished(bIsEmpty);
		
		if (!m_vActivateVM) return;
		
		PS_LanguageManager languageManager = PS_LanguageManager.GetInstance();
		PS_VariableArray arr = new PS_VariableArray();
		arr.Insert(entitiesList);
		m_vActivateVM.SetVariable("_this", arr);
		m_vActivateVM.RunUnscheduled();
		entitiesList = new PS_VariableArray();
	}
	
	// JIP Replication
	override bool RplSave(ScriptBitWriter writer)
	{
		// Pack every changeable variable
		writer.WriteString(m_sActivateScriptSQF);
		
		return true;
	}
	override bool RplLoad(ScriptBitReader reader)
	{
		// Unpack every changeable variable
		reader.ReadString(m_sActivateScriptSQF);
		
		return true;
	}
}
