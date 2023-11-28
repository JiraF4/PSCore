class PS_VMCodeBlock
{
	ref array<ref PS_VMCommand> m_aCommands;
	
	void PrintCommandList(int level)
	{
		foreach (PS_VMCommand command : m_aCommands)
		{
			command.PrintCommand(level);
		}
	}
	
	void PS_VMCodeBlock(array<ref PS_VMCommand> commandsList)
	{
		m_aCommands = commandsList;
	}
	
	PS_VMCommand Get(int position)
	{
		return m_aCommands.Get(position);
	}
	
	int Count()
	{
		return m_aCommands.Count();
	}
}