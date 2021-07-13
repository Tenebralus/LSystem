#pragma once
#include "stdafx.h"
#include "clsLSystemSuccessorCommand.h"
#include "clsMathOperation.h"

namespace GenerationLib
{
	clsLSystemSuccessorCommand::clsLSystemSuccessorCommand( ) : clsLSystemCommand( )
	{

	}

	clsLSystemSuccessorCommand::~clsLSystemSuccessorCommand( )
	{

	}

	void clsLSystemSuccessorCommand::Destroy( )
	{
		for (size_t i = 0; i < m_MathOperationsParameters.size( ); ++i)
		{
			for (size_t j = 0; j < m_MathOperationsParameters[ i ].size( ); ++j)
			{
				m_MathOperationsParameters[ i ][ j ]->Destroy( );
				delete m_MathOperationsParameters[ i ][ j ];
				m_MathOperationsParameters[ i ][ j ] = nullptr;
			}
			m_MathOperationsParameters[ i ].clear( );
		}
		m_MathOperationsParameters.clear( );
	}

	void clsLSystemSuccessorCommand::AddSuccessorParameter( float a_Parameter )
	{
		m_SuccessorParams.push_back( a_Parameter );
	}

	void clsLSystemSuccessorCommand::AddAxiomParameter( float a_Parameter )
	{
		m_AxiomParams.push_back( a_Parameter );
	}

	bool SortMathOperationsByRecursiveDepth( clsMathOperation* a_MathOperation1, clsMathOperation* a_MathOperation2 )
	{
		return ( a_MathOperation1->m_RecursiveDepth > a_MathOperation2->m_RecursiveDepth );
	}

	void clsLSystemSuccessorCommand::SetAndSortCurrentMathOperationsParameter( std::vector<clsMathOperation*>* a_MathOperationsParameter )
	{
		std::sort( a_MathOperationsParameter->begin( ), a_MathOperationsParameter->end( ), SortMathOperationsByRecursiveDepth );
		m_MathOperationsParameters.push_back( *a_MathOperationsParameter );
		a_MathOperationsParameter->clear( );
	}

	std::vector<float> clsLSystemSuccessorCommand::GetSuccessorParameters( )
	{
		return m_SuccessorParams;
	}

	std::vector<float> clsLSystemSuccessorCommand::GetAxiomParameters( )
	{
		return m_AxiomParams;
	}

	std::vector<std::vector<clsMathOperation*>> clsLSystemSuccessorCommand::GetMathOperationsParameters( )
	{
		return m_MathOperationsParameters;
	}

	void clsLSystemSuccessorCommand::SetAxiomParameters( std::vector<float> a_Parameters )
	{
		m_AxiomParams.clear( );
		for (size_t i = 0; i < a_Parameters.size( ); ++i)
		{
			m_AxiomParams.push_back( a_Parameters[ i ] );
		}
	}

	void clsLSystemSuccessorCommand::SetSuccessorParameterAt( int a_Index, float a_Parameter )
	{
		m_SuccessorParams[ a_Index ] = a_Parameter;
	}

	void clsLSystemSuccessorCommand::SetRoutine( char a_Routine )
	{
		m_Routine = a_Routine;
	}

	char clsLSystemSuccessorCommand::GetRoutine( )
	{
		return m_Routine;
	}

	void clsLSystemSuccessorCommand::Calculate( )
	{

	}
}
