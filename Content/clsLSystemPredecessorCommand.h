#pragma once
#include "stdafx.h"
#include "clsLSystemCommand.h"

namespace GenerationLib
{
	class clsLSystemPredecessorCommand : public clsLSystemCommand
	{
	public:
		clsLSystemPredecessorCommand( );
		~clsLSystemPredecessorCommand( );
		void Destroy( );
		void AddParameter( float Value );
		void SetParameterAt( int Index, float Value );
		std::vector<float>* GetParameters( );
		float* GetParameterAt( int Index );
		void SetSuccessor( std::vector<clsLSystemSuccessorCommand> a_Successor );
		void AddCommandToSuccessor( clsLSystemSuccessorCommand a_Command );
		std::vector<clsLSystemSuccessorCommand>* GetSuccessor( );
	private:
		std::vector<clsLSystemSuccessorCommand> m_Successor;
		std::vector<float> m_Parameters;
	};
}
