class PS_CommandGetLoadout : PS_Command // "getLoadout"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		PS_VariableEntity varEntity = PS_VariableEntity.Cast(right); 
		
		if (varEntity) {
			SCR_CharacterInventoryStorageComponent inventory = SCR_CharacterInventoryStorageComponent.Cast(varEntity.m_eEntity.FindComponent(SCR_CharacterInventoryStorageComponent));
			CharacterWeaponManagerComponent weaponsManager = CharacterWeaponManagerComponent.Cast(varEntity.m_eEntity.FindComponent(CharacterWeaponManagerComponent));
			if (inventory && weaponsManager)
			{
				PS_VariableArray loadoutArray = new PS_VariableArray();
				GetWeapons(weaponsManager, loadoutArray);
				GetItemsInSlot(inventory, loadoutArray, LoadoutBackpackArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutBootsArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutHeadCoverArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutJacketArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutPantsArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutVestArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutArmoredVestSlotArea);
				GetItemsInSlot(inventory, loadoutArray, LoadoutBinocularsArea);
				return loadoutArray;
			}
		}
		
		return new PS_Variable();
	};
	
	void GetWeapons(CharacterWeaponManagerComponent weaponsManager, PS_VariableArray loadoutArray)
	{
		PS_VariableArray weaponsArray = new PS_VariableArray();
		loadoutArray.Insert(weaponsArray);
		
		array<WeaponSlotComponent> outSlots = new array<WeaponSlotComponent>();
		weaponsManager.GetWeaponsSlots(outSlots);
		
		foreach (WeaponSlotComponent weaponSlot : outSlots)
		{
			PS_VariableArray weaponSlotArray = new PS_VariableArray();
			weaponsArray.Insert(weaponSlotArray);
			weaponSlotArray.Insert(new PS_VariableNumber(weaponSlot.GetWeaponSlotIndex()));
			IEntity weaponEntity = weaponSlot.GetWeaponEntity();
			if (weaponEntity) GetItemsRecursive(weaponSlotArray, weaponEntity);
		}
	}
	
	void GetItemsInSlot(SCR_CharacterInventoryStorageComponent inventory, PS_VariableArray loadoutArray, typename areaType)
	{
		PS_VariableArray slotArray = new PS_VariableArray();
		string areaName = areaType.ToString();
		areaName.Replace("Loadout", "");
		areaName.Replace("Area", "");
		slotArray.Insert(new PS_VariableString(areaName));
		loadoutArray.Insert(slotArray);
		
		InventoryStorageSlot slot = inventory.GetSlotFromArea(areaType);
		if (!slot)
			return;
		
		IEntity entity = slot.GetAttachedEntity();
		if (!entity)
			return;
		
		GetItemsRecursive(slotArray, entity);
	}
	
	void GetItemsRecursive(PS_VariableArray itemsArray, IEntity item)
	{
		PS_VariableArray itemArray = new PS_VariableArray();
		itemArray.Insert(new PS_VariableEntity(item));
		SCR_UniversalInventoryStorageComponent subInventory = SCR_UniversalInventoryStorageComponent.Cast(item.FindComponent(SCR_UniversalInventoryStorageComponent));	
		if (subInventory)
		{
			PS_VariableArray subItemsArray = new PS_VariableArray();
			
			array<IEntity> outItems = new array<IEntity>();
			subInventory.GetAll(outItems);
			foreach (IEntity itemEntity : outItems)
			{
				GetItemsRecursive(subItemsArray, itemEntity);
			}
			
			itemArray.Insert(subItemsArray);
		}
		itemsArray.Insert(itemArray);
	}
}
