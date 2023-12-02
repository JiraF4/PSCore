[ComponentEditorProps(category: "GameScripted/Area Mesh", description: "")]
class PS_TriggerZoneAreaMeshComponentClass: SCR_BaseAreaMeshComponentClass
{
};
class PS_TriggerZoneAreaMeshComponent: SCR_BaseAreaMeshComponent
{
	override float GetRadius()
	{		
		PS_SQFScriptedTriggerEntity triggerEntity = PS_SQFScriptedTriggerEntity.Cast(GetOwner());
		
		
		
		return triggerEntity.GetSphereRadius();
	}
	override void EOnInit(IEntity owner)
	{
		GenerateAreaMesh();
	}
};