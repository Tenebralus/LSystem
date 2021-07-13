#pragma once
#include "stdafx.h"

namespace GenerationLib
{
	class clsLSystemCommand
	{
	public:
		clsLSystemCommand( );
		~clsLSystemCommand( );
		void SetName( char Name );
		void SetCommandType( enumLSystemCommands::enumLSystemCommands CommandType );
		char GetName( );
		enumLSystemCommands::enumLSystemCommands GetCommandType( );
	protected:

	public:

	protected:
		char m_Name;
		enumLSystemCommands::enumLSystemCommands m_CommandType;
	};
}
