class PS_StringMultilineEditorAttributeUISQFComponent: PS_StringMultilineEditorAttributeUIComponent
{
	RichTextWidget w_hErrorText;
	
	override void Init(Widget w, SCR_BaseEditorAttribute attribute)
	{
		w_hErrorText = RichTextWidget.Cast(w.FindAnyWidget("SQFErrorText"));
		super.Init(w, attribute);
	}
	
	override bool OnChange(Widget w, int x, int y, bool finished)
	{
		bool res = super.OnChange(w, x, y, finished);
		
		string code = w_hEditBox.GetValue();
		if (code == "") return null;
		PS_Lexer lexer = new PS_Lexer(code);
		if (lexer.errorStr != "")
		{
			w_hErrorText.SetText(lexer.errorStr);
			return res;
		}
		PS_Parser parser = new PS_Parser(lexer);
		if (parser.errorStr != "")
		{
			w_hErrorText.SetText(parser.errorStr);
			return res;
		}
		w_hErrorText.SetText("");
		return res;
	}	
};