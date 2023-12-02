class PS_CommandCameraPosition : PS_Command // "cameraPosition"
{
	override PS_Variable ExecuteUnholded(PS_Variable left, PS_Variable right, PS_VirtualMachine vm) {
		CameraBase camera = GetGame().GetCameraManager().CurrentCamera();
		vector position;
		if (camera) position = camera.GetOrigin();
		PS_VariableArray arr = new PS_VariableArray();
		arr.Insert(new PS_VariableNumber(position[0]));
		arr.Insert(new PS_VariableNumber(position[1]));
		arr.Insert(new PS_VariableNumber(position[2]));
		return arr;
	};
}

