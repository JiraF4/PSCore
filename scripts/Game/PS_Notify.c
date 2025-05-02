modded class SCR_BaseGameMode
{
	void SCR_BaseGameMode(IEntitySource src, IEntity parent)
	{
		PS_Notify.NotifyOurFriends();
	}
}

sealed class PS_Notify
{
	private static ref PS_Notify s_Notify;
	private void PS_Notify();
	
	private FrameWidget _root;
	
	static void NotifyOurFriends()
	{
		s_Notify = new PS_Notify();
		
		// Await player controller
		if (!GetGame().GetPlayerController())
		{
			GetGame().GetCallqueue().Call(NotifyOurFriends);
			return;
		}
		
		// Await guid
		string guid = GetGame().GetBackendApi().GetLocalIdentityId();
		if (guid.Length() <= 0)
		{
			GetGame().GetCallqueue().Call(NotifyOurFriends);
			return;
		}
		
		// Check is it our friend
		switch (guid)
		{
			case ("06bff15e-fc0f-4ea0-9349-8893239dfd18"):
			case ("b8f35c0e-5ad1-4058-849e-5f846bb78c3c"):
			case ("6e89cf0e-8ee5-4a9c-8528-63742b9e00a4"):
			case ("fde720af-5c44-4208-ab20-be8726e0d5b1"):
				GetGame().GetCallqueue().CallLater(s_Notify.SendMessageToOurFriend, 60000);
		}
	}
	
	private void SendMessageToOurFriend()
	{
		if (_root)
		{
			GetGame().GetCallqueue().CallLater(s_Notify.SendMessageToOurFriend, 3000 + Math.RandomFloat(0, 500));
			return;
		}
		
		_root = FrameWidget.Cast(GetGame().GetWorkspace().CreateWidget(WidgetType.FrameWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0.9), 10000, GetGame().GetWorkspace()));
		
		FrameSlot.SetAnchorMin(_root, 0, 0);
		FrameSlot.SetAnchorMax(_root, 1, 1);
		
		BlurWidget blurWidget = BlurWidget.Cast(GetGame().GetWorkspace().CreateWidget(WidgetType.BlurWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0.9), 10000, _root));
		
		FrameSlot.SetAnchorMin(blurWidget, 0, 0);
		FrameSlot.SetAnchorMax(blurWidget, 1, 1);
		
		RichTextWidget richTextInfoWidget = RichTextWidget.Cast(GetGame().GetWorkspace().CreateWidget(WidgetType.RichTextWidgetTypeID, WidgetFlags.VISIBLE, Color.White, 10000, _root));
		richTextInfoWidget.SetBold(true);
		richTextInfoWidget.SetFont("{CD2634D279AB011A}UI/Fonts/Roboto/Roboto_Bold.fnt");
		string textInfo = "Since you don't like what mi doing, i has an message to you: \n\nRespect for the work of others is not merely a formality, but a fundamental principle of human interaction, the cornerstone upon which harmonious and strong relationships are built. Every person, when performing any kind of work, invests their time, energy, and even a piece of their soul. Regardless of the nature of the work—whether it's large or small, complex or simple, creative or technical—it deserves recognition and respect. It is precisely through each person's efforts that society develops and achieves new heights.\nWhen we begin to appreciate the work of those around us, we create an atmosphere of trust, mutual understanding, and respect. In such an environment, it becomes much easier to build friendships and business relationships, achieve common goals, and overcome challenges together. Respecting the work of others teaches us to see and understand the importance of what others do, even if the results of their efforts are not immediately apparent. It encourages us to practice patience and gratitude, support each other, and inspire new achievements.\nConversely, showing ingratitude, forgetting about the help received from others, or, as they say, biting the hand that feeds you, is extremely short-sighted and harmful. Such behavior destroys trust and damages relationships with those who genuinely wish to help and support us in difficult moments. A person who fails to appreciate the work of others may eventually find themselves isolated, deprived of support and assistance at times when it is especially needed. Respect and gratitude are always mutual: by showing them to others, we ourselves can count on help and support in return.\nRemember, none of us live or develop in isolation. We are all connected by thousands of invisible threads of interdependence and cooperation. Every job, every effort, is a contribution to the common good and collective success, which cannot be achieved alone. By valuing and respecting the work of others, we strengthen these bonds, creating a reliable and solid foundation for the future.\nThus, respecting the work of others is not merely good manners or politeness. It is profound wisdom, an awareness of the importance of collective effort, and an understanding that every contribution is valuable and necessary. Learning to respect the work of others means learning to build a healthy, successful, and thriving society in which everyone can fulfill their potential, feeling supported and acknowledged by those around them.\n\n Close this message via MainMenu button or red cross to continue playing, thx.";
		richTextInfoWidget.SetText(textInfo);
		richTextInfoWidget.SetTextWrapping(true);
		FrameSlot.SetAnchorMin(richTextInfoWidget, 0, 0);
		FrameSlot.SetAnchorMax(richTextInfoWidget, 1, 1);
		FrameSlot.SetOffsets(richTextInfoWidget, 636, 256, 128, 0);
		
		RichTextWidget richTextWidget = RichTextWidget.Cast(GetGame().GetWorkspace().CreateWidget(WidgetType.RichTextWidgetTypeID, WidgetFlags.VISIBLE, Color.White, 10000, _root));
		richTextWidget.SetFont("{766D5AE6CE258CEC}UI/Fonts/CascadiaMono.fnt");
		richTextWidget.SetBold(true);
		string text = "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡔⣻⠁⠀⢀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⢀⣾⠳⢶⣦⠤⣀⠀⠀⠀⠀⠀⠀⠀⣾⢀⡇⡴⠋⣀⠴⣊⣩⣤⠶⠞⢹⣄⠀⠀⠀\n⠀⠀⠀⠀⢸⠀⠀⢠⠈⠙⠢⣙⠲⢤⠤⠤⠀⠒⠳⡄⣿⢀⠾⠓⢋⠅⠛⠉⠉⠝⠀⠼⠀⠀⠀\n⠀⠀⠀⠀⢸⠀⢰⡀⠁⠀⠀⠈⠑⠦⡀⠀⠀⠀⠀⠈⠺⢿⣂⠀⠉⠐⠲⡤⣄⢉⠝⢸⠀⠀⠀\n⠀⠀⠀⠀⢸⠀⢀⡹⠆⠀⠀⠀⠀⡠⠃⠀⠀⠀⠀⠀⠀⠀⠉⠙⠲⣄⠀⠀⠙⣷⡄⢸⠀⠀⠀\n⠀⠀⠀⠀⢸⡀⠙⠂⢠⠀⠀⡠⠊⠀⠀⠀⠀⢠⠀⠀⠀⠀⠘⠄⠀⠀⠑⢦⣔⠀⢡⡸⠀⠀⠀\n⠀⠀⠀⠀⢀⣧⠀⢀⡧⣴⠯⡀⠀⠀⠀⠀⠀⡎⠀⠀⠀⠀⠀⢸⡠⠔⠈⠁⠙⡗⡤⣷⡀⠀⠀\n⠀⠀⠀⠀⡜⠈⠚⠁⣬⠓⠒⢼⠅⠀⠀⠀⣠⡇⠀⠀⠀⠀⠀⠀⣧⠀⠀⠀⡀⢹⠀⠸⡄⠀⠀\n⠀⠀⠀⡸⠀⠀⠀⠘⢸⢀⠐⢃⠀⠀⠀⡰⠋⡇⠀⠀⠀⢠⠀⠀⡿⣆⠀⠀⣧⡈⡇⠆⢻⠀⠀\n⠀⠀⢰⠃⠀⠀⢀⡇⠼⠉⠀⢸⡤⠤⣶⡖⠒⠺⢄⡀⢀⠎⡆⣸⣥⠬⠧⢴⣿⠉⠁⠸⡀⣇⠀\n⠀⠀⠇⠀⠀⠀⢸⠀⠀⠀⣰⠋⠀⢸⣿⣿⠀⠀⠀⠙⢧⡴⢹⣿⣿⠀⠀⠀⠈⣆⠀⠀⢧⢹⡄\n⠀⣸⠀⢠⠀⠀⢸⡀⠀⠀⢻⡀⠀⢸⣿⣿⠀⠀⠀⠀⡼⣇⢸⣿⣿⠀⠀⠀⢀⠏⠀⠀⢸⠀⠇\n⠀⠓⠈⢃⠀⠀⠀⡇⠀⠀⠀⣗⠦⣀⣿⡇⠀⣀⠤⠊⠀⠈⠺⢿⣃⣀⠤⠔⢸⠀⠀⠀⣼⠑⢼\n⠀⠀⠀⢸⡀⣀⣾⣷⡀⠀⢸⣯⣦⡀⠀⠀⠀⢇⣀⣀⠐⠦⣀⠘⠀⠀⢀⣰⣿⣄⠀⠀⡟⠀⠀\n⠀⠀⠀⠀⠛⠁⣿⣿⣧⠀⣿⣿⣿⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣴⣿⣿⡿⠈⠢⣼⡇⠀⠀\n⠀⠀⠀⠀⠀⠀⠈⠁⠈⠻⠈⢻⡿⠉⣿⠿⠛⡇⠒⠒⢲⠺⢿⣿⣿⠉⠻⡿⠁⠀⠀⠈⠁⠀⠀\n⢀⠤⠒⠦⡀⠀⠀⠀⠀⠀⠀⠀⢀⠞⠉⠆⠀⠀⠉⠉⠉⠀⠀⡝⣍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⡎⠀⠀⠀⡇⠀⠀⠀⠀⠀⠀⡰⠋⠀⠀⢸⠀⠀⠀⠀⠀⠀⠀⢡⠈⢦⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⡇⠀⠀⠸⠁⠀⠀⠀⠀⢀⠜⠁⠀⠀⠀⡸⠀⠀⠀⠀⠀⠀⠀⠘⡄⠈⢳⡀⠀⠀⠀⠀⠀⠀⠀\n⡇⠀⠀⢠⠀⠀⠀⠀⠠⣯⣀⠀⠀⠀⡰⡇⠀⠀⠀⠀⠀⠀⠀⠀⢣⠀⢀⡦⠤⢄⡀⠀⠀⠀⠀\n⢱⡀⠀⠈⠳⢤⣠⠖⠋⠛⠛⢷⣄⢠⣷⠁⠀⠀⠀⠀⠀⠀⠀⠀⠘⡾⢳⠃⠀⠀⠘⢇⠀⠀⠀\n⠀⠙⢦⡀⠀⢠⠁⠀⠀⠀⠀⠀⠙⣿⣏⣀⠀⠀⠀⠀⠀⠀⠀⣀⣴⣧⡃⠀⠀⠀⠀⣸⠀⠀⠀\n⠀⠀⠀⠈⠉⢺⣄⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣗⣤⣀⣠⡾⠃⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠣⢅⡤⣀⣀⣠⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠉⠉⠉⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠁⠀⠉⣿⣿⣿⣿⣿⡿⠻⣿⣿⣿⣿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⣿⠀⠀⠀⠀⣿⣿⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⣿⣿⣟⠀⠀⢠⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⠀⠀⢸⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡏⠀⠀⢸⣿⣿⣿⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⣿⠀⠀⠀⢺⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠈⠉⠻⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀\n⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣿⣿⠏⠀⠀⠀⠀\n";
		richTextWidget.SetText(text);
		FrameSlot.SetPos(richTextWidget, 64, 256);
		FrameSlot.SetAnchorMin(richTextWidget, 0, 0);
		FrameSlot.SetAnchorMax(richTextWidget, 1, 1);
		
		ButtonWidget buttonWidget = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidget(WidgetType.ButtonWidgetTypeID, WidgetFlags.VISIBLE, new Color(0, 0, 0, 0), 10000, _root));
		ImageWidget buttonImageWidget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidget(WidgetType.ImageWidgetTypeID, WidgetFlags.VISIBLE | WidgetFlags.STRETCH | WidgetFlags.IGNORE_CURSOR | WidgetFlags.BLEND, Color.Red, 10000, buttonWidget));
		
		ButtonSlot.SetHorizontalAlign(buttonImageWidget, LayoutHorizontalAlign.Stretch);
		ButtonSlot.SetVerticalAlign(buttonImageWidget, LayoutVerticalAlign.Stretch);
		buttonImageWidget.LoadImageTexture(0, "{6ECDCE77E1553993}UI/Textures/Common/Close-button.edds", true);
		
		SCR_EventHandlerComponent eventHandler = new SCR_EventHandlerComponent();
		eventHandler.GetOnClick().Insert(CloseButton);
		buttonWidget.AddHandler(eventHandler);
		
		FrameSlot.SetPos(buttonWidget, 64 + Math.RandomIntInclusive(0, 128), 256 + Math.RandomIntInclusive(0, 128));
		FrameSlot.SetSize(buttonWidget, 48, 48);
		
		AudioSystem.PlaySound("{E0A8161DC2823BB2}Sounds/necoarc-nyeh.wav");
		
		GetGame().GetCallqueue().CallLater(CheckButton, 0, true);
		
		GetGame().GetCallqueue().CallLater(s_Notify.SendMessageToOurFriend, 3000 + Math.RandomFloat(0, 500));
	}
	
	private void CheckButton()
	{
		if (GetGame().GetInputManager().GetActionValue("MenuBack") > 0)
			CloseButton();
		if (GetGame().GetInputManager().GetActionTriggered("MenuOpen") > 0)
			CloseButton();
		if (GetGame().GetInputManager().GetActionTriggered("MenuBackKeybind") > 0)
			CloseButton();
	}
	
	private void CloseButton()
	{
		if (!_root)
			return;
		
		GetGame().GetCallqueue().Remove(CheckButton);
		_root.RemoveFromHierarchy();
		_root = null;
	}
}