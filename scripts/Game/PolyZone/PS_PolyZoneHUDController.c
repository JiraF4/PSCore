[ComponentEditorProps(category: "GameScripted/Misc", description: "")]
class PS_PolyZoneHUDControllerClass: ScriptComponentClass
{
};
class PS_PolyZoneHUDController: ScriptComponent
{
	protected PS_PolyZoneHUD m_PolyZoneHUD;
			
	void UpdatePlayerHUD(IEntity owner)
	{
		SCR_PlayerController playerController = SCR_PlayerController.Cast(owner);
		
		IEntity character = playerController.GetControlledEntity();
		if (!character)
		{
			Rpc(ShowEffects, new PS_EffectsContainer());
			return;
		}
		
		PS_PolyZoneEffectHandler polyZoneEffectHandler = PS_PolyZoneEffectHandler.Cast(character.FindComponent(PS_PolyZoneEffectHandler));
		if (!polyZoneEffectHandler)
		{
			Rpc(ShowEffects, new PS_EffectsContainer());
			return;
		}
		
		// compress effects
		PS_EffectsContainer effectsContainer = new PS_EffectsContainer();
		foreach (PS_PolyZoneTrigger trigger, PS_PolyZoneEffect effect : polyZoneEffectHandler.m_mapPolyZoneEffects)
		{
			effectsContainer.m_aEffects.Insert(effect.GetEffectContainer());
		}
		// Update on client side
		Rpc(ShowEffects, effectsContainer)
	}
	[RplRpc(RplChannel.Reliable, RplRcver.Owner)]
	protected void ShowEffects(PS_EffectsContainer effectsContainer)
	{
		m_PolyZoneHUD.HideAll();
		foreach (PS_EffectContainer effect : effectsContainer.m_aEffects)
		{
			m_PolyZoneHUD.ShowEffect(effect);
		}
	}
	
	override void OnPostInit(IEntity owner)
	{
		SCR_HUDManagerComponent hudManager = SCR_HUDManagerComponent.Cast(owner.FindComponent(SCR_HUDManagerComponent));
		if (hudManager)
		{
			array<BaseInfoDisplay> infoDisplays = new array<BaseInfoDisplay>;
			int count = hudManager.GetInfoDisplays(infoDisplays);
		
			for(int i = 0; i < count; i++)
			{
				if (infoDisplays[i].Type() == PS_PolyZoneHUD)
				{
					m_PolyZoneHUD = PS_PolyZoneHUD.Cast(infoDisplays[i]);
					if (m_PolyZoneHUD)
						break;
				}
			}
		}
		
		if (Replication.IsServer())
			GetGame().GetCallqueue().CallLater(UpdatePlayerHUD, 100, true, owner);
	}
}
class PS_EffectsContainer
{
	ref array<ref PS_EffectContainer> m_aEffects = {};
	
	static void Encode(SSnapSerializerBase snapshot, ScriptCtx hint, ScriptBitSerializer packet) 
	{
		int effectsCount;
		snapshot.SerializeInt(effectsCount);
		packet.SerializeInt(effectsCount);
		for (int i = 0; i < effectsCount; i++)
		{
			int id;
			PS_EPolyZoneEffectHUDType type;
			float time;
			string str;
			
			snapshot.SerializeInt(id);
			snapshot.SerializeInt(type);
			snapshot.SerializeFloat(time);
			snapshot.SerializeString(str);
			packet.SerializeInt(id);
			packet.SerializeInt(type);
			packet.SerializeFloat(time);
			packet.SerializeString(str);
		}
	}
	static bool Decode(ScriptBitSerializer packet, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		int effectsCount;
		packet.SerializeInt(effectsCount);
		snapshot.SerializeInt(effectsCount);
		for (int i = 0; i < effectsCount; i++)
		{
			int id;
			PS_EPolyZoneEffectHUDType type;
			float time;
			string str;
			
			packet.SerializeInt(id);
			packet.SerializeInt(type);
			packet.SerializeFloat(time);
			packet.SerializeString(str);
			snapshot.SerializeInt(id);
			snapshot.SerializeInt(type);
			snapshot.SerializeFloat(time);
			snapshot.SerializeString(str);
		}
		return true;
	}
	static bool SnapCompare(SSnapSerializerBase lhs, SSnapSerializerBase rhs, ScriptCtx hint) 
	{
		int effectsCount1, effectsCount2;
		lhs.SerializeInt(effectsCount1);
		rhs.SerializeInt(effectsCount2);
		if (effectsCount1 != effectsCount2)
			return false;
		for (int i = 0; i < effectsCount1; i++)
		{
			int id1, id2;
			PS_EPolyZoneEffectHUDType type1, type2;
			float time1, time2;
			string str1, str2;
			
			lhs.SerializeInt(id1);
			rhs.SerializeInt(id2);
			if (id1 != id2)
				return false;
			
			lhs.SerializeInt(type1);
			rhs.SerializeInt(type2);
			if (type1 != type2)
				return false;
			
			lhs.SerializeFloat(time1);
			rhs.SerializeFloat(time2);
			if (time1 != time2)
				return false;
			
			lhs.SerializeString(str1);
			rhs.SerializeString(str2);
			if (str1 != str2)
				return false;
		}
		return true;
	}
	static bool PropCompare(PS_EffectsContainer prop, SSnapSerializerBase snapshot, ScriptCtx hint) 
	{
		int effectsCount;
		snapshot.SerializeInt(effectsCount);
		if (prop.m_aEffects.Count() != effectsCount)
			return false;
		for (int i = 0; i < effectsCount; i++)
		{
			int id;
			PS_EPolyZoneEffectHUDType type;
			float time;
			string str;
			
			snapshot.SerializeInt(id);
			snapshot.SerializeInt(type);
			snapshot.SerializeFloat(time);
			snapshot.SerializeString(str);
			
			if (prop.m_aEffects[i].m_iId != id)
				return false;
			if (prop.m_aEffects[i].m_iType != type)
				return false;
			if (prop.m_aEffects[i].m_fTime != time)
				return false;
			if (prop.m_aEffects[i].m_sString != str)
				return false;
		}
		return true;
	}
	static bool Extract(PS_EffectsContainer prop, ScriptCtx hint, SSnapSerializerBase snapshot) 
	{
		int effectsCount = prop.m_aEffects.Count();
		snapshot.SerializeInt(effectsCount);
		for (int i = 0; i < effectsCount; i++)
		{
			snapshot.SerializeInt(prop.m_aEffects[i].m_iId);
			snapshot.SerializeInt(prop.m_aEffects[i].m_iType);
			snapshot.SerializeFloat(prop.m_aEffects[i].m_fTime);
			snapshot.SerializeString(prop.m_aEffects[i].m_sString);
		}
		return true;
	}
	static bool Inject(SSnapSerializerBase snapshot, ScriptCtx hint, PS_EffectsContainer prop) 
	{	
		int effectsCount;
		snapshot.SerializeInt(effectsCount);
		prop.m_aEffects = {};
		for (int i = 0; i < effectsCount; i++)
		{
			prop.m_aEffects.Insert(new PS_EffectContainer());
			snapshot.SerializeInt(prop.m_aEffects[i].m_iId);
			snapshot.SerializeInt(prop.m_aEffects[i].m_iType);
			snapshot.SerializeFloat(prop.m_aEffects[i].m_fTime);
			snapshot.SerializeString(prop.m_aEffects[i].m_sString);
		}
		return true;
	}
}

