#pragma once
#include "stdafx.h"
#include "clsLSystemCommand.h"

namespace GenerationLib
{
	clsLSystemCommand::clsLSystemCommand( )
	{

	}

	clsLSystemCommand::~clsLSystemCommand( )
	{

	}

	void clsLSystemCommand::SetName( char Name )
	{
		m_Name = Name;
	}

	char clsLSystemCommand::GetName( )
	{
		return m_Name;
	}

	void clsLSystemCommand::SetCommandType( enumLSystemCommands::enumLSystemCommands a_CommandType )
	{
		m_CommandType = a_CommandType;
	}

	enumLSystemCommands::enumLSystemCommands clsLSystemCommand::GetCommandType( )
	{
		return m_CommandType;
	}
}
