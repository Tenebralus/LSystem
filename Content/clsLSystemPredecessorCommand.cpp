#pragma once
#include "stdafx.h"
#include "clsLSystemPredecessorCommand.h"
#include "clsLSystemSuccessorCommand.h"

namespace GenerationLib
{
	clsLSystemPredecessorCommand::clsLSystemPredecessorCommand( ) : clsLSystemCommand( )
	{

	}

	clsLSystemPredecessorCommand::~clsLSystemPredecessorCommand( )
	{

	}

	void clsLSystemPredecessorCommand::Destroy( )
	{
		for (size_t i = 0; i < m_Successor.size( ); ++i)
		{
			m_Successor[ i ].Destroy( );
		}
	}

	void clsLSystemPredecessorCommand::AddParameter( float a_Value )
	{
		m_Parameters.push_back( a_Value );
	}

	void clsLSystemPredecessorCommand::SetParameterAt( int a_Index, float a_Value )
	{
		m_Parameters[ a_Index ] = a_Value;
	}

	std::vector<float>* clsLSystemPredecessorCommand::GetParameters( )
	{
		return &m_Parameters;
	}

	float* clsLSystemPredecessorCommand::GetParameterAt( int a_Index )
	{
		return &m_Parameters[ a_Index ];
	}

	void clsLSystemPredecessorCommand::SetSuccessor( std::vector<clsLSystemSuccessorCommand> a_Successor )
	{
		m_Successor = a_Successor;
	}

	void clsLSystemPredecessorCommand::AddCommandToSuccessor( clsLSystemSuccessorCommand a_Command )
	{
		m_Successor.push_back( a_Command );
	}

	std::vector<clsLSystemSuccessorCommand>* clsLSystemPredecessorCommand::GetSuccessor( )
	{
		return &m_Successor;
	}
}
