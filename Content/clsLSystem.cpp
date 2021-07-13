#pragma once
#include "stdafx.h"
#include "clsLSystem.h"
#include "clsPoint.h"

#include "LSystemCommands\clsLSystemPredecessorCommand.h"
#include "LSystemCommands\clsLSystemSuccessorCommand.h"
#include "LSystemCommands\clsMathOperationAdd.h"
#include "LSystemCommands\clsMathOperationDivide.h"
#include "LSystemCommands\clsMathOperationMultiply.h"
#include "LSystemCommands\clsMathOperationSubtract.h"
#include "LSystemCommands\clsMathOperationValue.h"

#include "../OpenGLLib/clsObject.h"

namespace GenerationLib
{
	/*
	Characters:
	F  - Move forward one time
	+  - Yaw left
	-  - Yaw right
	\\ - Roll left
	/  - Roll right
	^  - Pitch up
	&  - Pitch down
	[  - Start new branch
	]  - End current branch
	!  - Set width
	*/

	clsLSystem::clsLSystem( )
	{
		m_Turtle = new OpenGLLib::clsObject( );
	}

	clsLSystem::~clsLSystem( )
	{
		delete m_Turtle;
	}

	void clsLSystem::Destroy( )
	{
		for (size_t i = 0; i < m_Rules.size( ); ++i)
		{
			m_Rules[ i ].Destroy( );
		}
	}

	void clsLSystem::SetupLSystem( )
	{
		ConvertAxiomToObjects( m_Axiom );

		m_Rules.reserve( m_UnparsedRules.size( ) );
		for (auto it = m_UnparsedRules.begin( ); it != m_UnparsedRules.end( ); ++it)
		{
			ConvertRuleToObject( it->first.first, it->first.second, it->second );
		}
		m_UnparsedRules.clear( );

		m_Result.reserve( 2048 * 4 ); // Temp

		for (int i = 0; i < m_AmountOfIterations; ++i)
		{
			CalculateIteration( );
		}

		TranslateStringToTurtle( );
	}

	void clsLSystem::ConvertRuleToObject( char a_RuleName, std::string a_Parameters, std::string a_Successor )
	{
		m_Rules.push_back( clsLSystemPredecessorCommand( ) );
		clsLSystemPredecessorCommand* l_CurrPredecessorCommand = &m_Rules[ m_Rules.size( ) - 1 ];
		std::vector<clsMathOperation*> l_MathOperationsParameter;
		std::map<char, int> l_ParameterMapFromPredecessor;
		float l_DefaultValue = 1.0f;
		for (size_t i = 0; i < a_Parameters.length( ); ++i)
		{
			l_CurrPredecessorCommand->AddParameter( l_DefaultValue );
			l_ParameterMapFromPredecessor[ a_Parameters[ i ] ] = (int) i;
		}

		l_CurrPredecessorCommand->SetName( a_RuleName );
		l_CurrPredecessorCommand->SetCommandType( enumLSystemCommands::eRule );
		std::vector<std::string> l_SplittedSuccessor = Globals::Split( a_Successor, " " );
		for (size_t i = 0; i < l_SplittedSuccessor.size( ); ++i)
		{
			std::string l_CurrCommand = l_SplittedSuccessor[ i ];
			char l_CommandName = l_CurrCommand[ 0 ];
			switch (l_CommandName)
			{
				case 'F':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eForward );
					break;
				case '+':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eYawLeft );
					break;
				case '-':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eYawRight );
					break;
				case '\\':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eRollLeft );
					break;
				case '/':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eRollRight );
					break;
				case '^':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::ePitchUp );
					break;
				case '&':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::ePitchDown );
					break;
				case '[':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eBranchStart );
					break;
				case ']':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eBranchEnd );
					break;
				case '!':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eWidth );
					break;
				case '@':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eRoutine );
					break;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'G':
					ConvertSuccessorCommandToObject( l_CurrPredecessorCommand, &l_MathOperationsParameter, &l_ParameterMapFromPredecessor, l_CurrCommand, enumLSystemCommands::eRule );
				default:
					break;
			}
		}
	}

	void clsLSystem::ConvertSuccessorCommandToObject( clsLSystemPredecessorCommand* a_CurrPredecessorCommand, std::vector<clsMathOperation*>* a_MathOperationsParameter, std::map<char, int>* a_ParameterMapFromPredecessor, std::string a_Command, enumLSystemCommands::enumLSystemCommands a_CommandType )
	{
		clsLSystemSuccessorCommand l_CurrSuccessorCommand;
		float l_DefaultValue = 1.0f;
		char l_Name = a_Command[ 0 ];
		l_CurrSuccessorCommand.SetName( l_Name );
		l_CurrSuccessorCommand.SetCommandType( a_CommandType );
		for (size_t i = 0; i < a_Command.length( ); ++i)
		{
			char l_CurrChar = a_Command[ i ];
			if (i + 1 < a_Command.length( ))
			{
				char l_NextChar = a_Command[ i + 1 ];
				if (l_NextChar == '(')
				{
					int l_CurrPriority = 0;
					std::string l_PriorityString;
					std::string l_TrimmedCommandString;
					std::vector<std::string> l_PriorityStrings;
					std::vector<std::string> l_TrimmedCommandStrings;
					for (size_t j = i + 2; j < a_Command.length( ) - 1; ++j) // only go through parameters
					{
						char l_CurrChar2 = a_Command[ j ];
						char l_PrevChar = a_Command[ j - 1 ];

						if (l_CurrChar2 != ',')
						{
							ParseCharToPriority( l_CurrChar2, l_PrevChar, &l_CurrPriority, &l_PriorityString );
						}

						// if next variable is not negative OR current character is not parantheses OR current character is not comma
						if (( ( l_PrevChar != '+' && l_PrevChar != '-' && l_PrevChar != '*' && l_PrevChar != '/' ) ||
							( l_CurrChar2 != '+' && l_CurrChar2 != '-' ) ) &&
							 l_CurrChar2 != '(' && l_CurrChar2 != ')'  &&
							 l_CurrChar2 != ',')
						{
							l_TrimmedCommandString += l_CurrChar2;
						}

						if (l_CurrChar2 == ',' || j == a_Command.length( ) - 2)
						{
							l_TrimmedCommandStrings.push_back( l_TrimmedCommandString );
							l_TrimmedCommandString.clear( );

							l_PriorityStrings.push_back( l_PriorityString );
							l_PriorityString.clear( );

							l_CurrSuccessorCommand.AddSuccessorParameter( 1.0f ); //placeholder value to determine param size during CalculateIteration
						}

						if (j == a_Command.length( ) - 2)
						{
							i = j;
						}
					}
					for (size_t j = 0; j < l_PriorityStrings.size( ); ++j)
					{
						if (l_PriorityStrings.size( ) == l_TrimmedCommandStrings.size( ))
						{
							if (l_PriorityStrings[ j ].length( ) == l_TrimmedCommandStrings[ j ].length( ))
							{
								OrderPriorityString( a_CurrPredecessorCommand, a_MathOperationsParameter, a_ParameterMapFromPredecessor, &l_CurrSuccessorCommand, &l_TrimmedCommandStrings[ j ], &l_PriorityStrings[ j ] );
							}
							else
							{
								assert( "Priority and trimmed string are not same size" == 0 );
							}
						}
						else
						{
							assert( "Amount of priority and trimmed strings are not equal" == 0 );
						}
					}
				}
				else if (l_NextChar == 'V')
				{
					// set routine in successor
				}
			}
		}
		a_CurrPredecessorCommand->AddCommandToSuccessor( l_CurrSuccessorCommand );
	}

	void clsLSystem::ParseCharToPriority( char a_Char, char a_PreviousChar, int *r_CurrParenthesesPriority, std::string *r_PriorityString )
	{
		int l_Priority = 0;
		if (a_Char == '(')
		{
			*r_CurrParenthesesPriority += 2;
		}
		else if (a_Char == ')')
		{
			*r_CurrParenthesesPriority -= 2;
		}
		else if (a_Char == '*' || a_Char == '/')
		{
			l_Priority = 1 + *r_CurrParenthesesPriority;
			std::string l_PrioToStr = std::to_string( l_Priority );
			char const *l_Char = l_PrioToStr.c_str( );
			*r_PriorityString += *l_Char;
		}
		else if (a_Char == '+' || a_Char == '-')
		{
			if (a_PreviousChar != '+' && a_PreviousChar != '-' && a_PreviousChar != '*' && a_PreviousChar != '/' && r_PriorityString->length( ) > 0)
			{
				l_Priority = 0 + *r_CurrParenthesesPriority;
				std::string l_PrioToStr = std::to_string( l_Priority );
				char const *l_Char = l_PrioToStr.c_str( );
				*r_PriorityString += *l_Char;
			}
			else
			{
				if (a_Char == '+')
				{
					*r_PriorityString += 'p';
				}
				else if (a_Char == '-')
				{
					*r_PriorityString += 'n';
				}
			}
		}
		else
		{
			if (r_PriorityString->length( ) == 0)
			{
				*r_PriorityString += 'p';
			}
			else if (r_PriorityString->at( r_PriorityString->length( ) - 1 ) != 'n' && r_PriorityString->at( r_PriorityString->length( ) - 1 ) != 'p')
			{
				*r_PriorityString += 'p';
			}
		}
		a_PreviousChar = a_Char;
	}

	void clsLSystem::OrderPriorityString( clsLSystemPredecessorCommand* a_CurrPredecessorCommand, std::vector<clsMathOperation*>* a_MathOperationsParameter, std::map<char, int>* a_ParameterMapFromPredecessor, clsLSystemSuccessorCommand* a_CurrSuccessorCommand, std::string *r_CommandString, std::string *r_PriorityString )
	{
		int l_HighestDepth = 0;
		int l_CurrDepth;
		clsMathOperation* l_TempMathOperation;
		// determine highest depth
		for (size_t i = 0; i < r_PriorityString->length( ); ++i)
		{
			if (isdigit( r_PriorityString->at( i ) ))
			{
				l_CurrDepth = atoi( &r_PriorityString->at( i ) );
				if (l_CurrDepth > l_HighestDepth)
				{
					l_HighestDepth = l_CurrDepth;
				}
			}
		}

		int l_CommandIndex = -1;
		OrderRecursive( a_CurrPredecessorCommand, a_MathOperationsParameter, a_ParameterMapFromPredecessor, *r_CommandString, *r_PriorityString, l_HighestDepth, l_CommandIndex, l_TempMathOperation, nullptr );
		a_CurrSuccessorCommand->SetAndSortCurrentMathOperationsParameter( a_MathOperationsParameter );
	}

	void clsLSystem::OrderRecursive( clsLSystemPredecessorCommand* a_CurrPredecessorCommand, std::vector<clsMathOperation*>* a_MathOperationsParameter, std::map<char, int>* a_ParameterMapFromPredecessor, std::string a_CommandString, std::string a_PriorityString, int a_DepthLevel, int a_CommandIndex, clsMathOperation* &a_MathOperation, clsMathOperation* a_PrevMathOperation )
	{
		int l_OperatorLevel;
		int l_OldCommandIndex = a_CommandIndex;
		char l_Operator;
		bool l_IsCorrectOperatorFound = false;
		std::string l_LeftPrioStr = "";
		std::string l_RightPrioStr = "";

		for (int j = 0; j <= a_DepthLevel; ++j)
		{
			a_CommandIndex = l_OldCommandIndex;
			for (size_t i = 0; i < a_PriorityString.length( ); ++i)
			{
				if (a_PriorityString[ i ] == '(' || a_PriorityString[ i ] == ')')
				{
					assert( "Parentheses are not allowed in priority string" == 0 );
				}

				++a_CommandIndex;
				if (isdigit( a_PriorityString.at( i ) ))
				{
					l_OperatorLevel = atoi( &a_PriorityString.at( i ) );
					if (l_OperatorLevel == j)
					{
						if (a_CommandString[ a_CommandIndex ] == '(' || a_CommandString[ a_CommandIndex ] == ')')
						{
							assert( "Parentheses are not allowed in command string" == 0 );
						}

						l_IsCorrectOperatorFound = true;
						l_Operator = a_CommandString[ a_CommandIndex ];

						// go left
						l_LeftPrioStr.reserve( i );
						for (int k = (int) i - 1; k >= 0; --k)
						{
							if (a_PriorityString[ k ] == '(' || a_PriorityString[ k ] == ')')
							{
								assert( "Parentheses are not allowed in priority string" == 0 );
							}

							if (a_PriorityString.at( k ) != '(' && a_PriorityString.at( k ) != ')')
							{
								l_LeftPrioStr.insert( l_LeftPrioStr.begin( ), a_PriorityString.at( k ) );
							}
						}

						// go right
						l_RightPrioStr.reserve( a_PriorityString.length( ) - 1 - i );
						for (size_t k = i + 1; k < a_PriorityString.length( ); ++k)
						{
							if (a_PriorityString[ k ] == '(' || a_PriorityString[ k ] == ')')
							{
								assert( "Parentheses are not allowed in priority string" == 0 );
							}

							if (a_PriorityString.at( k ) != '(' && a_PriorityString.at( k ) != ')')
							{
								l_RightPrioStr += a_PriorityString.at( k );
							}
						}

						if (l_Operator == '+')
						{
							a_MathOperation = new clsMathOperationAdd( );
						}
						else if (l_Operator == '-')
						{
							a_MathOperation = new clsMathOperationSubtract( );
						}
						else if (l_Operator == '*')
						{
							a_MathOperation = new clsMathOperationMultiply( );
						}
						else if (l_Operator == '/')
						{
							a_MathOperation = new clsMathOperationDivide( );
						}
						else
						{
							assert( "No math operation found in recursion" == 0 );
						}

						if (a_PrevMathOperation != nullptr)
						{
							a_MathOperation->m_Previous = a_PrevMathOperation;
							a_MathOperation->m_RecursiveDepth = a_PrevMathOperation->m_RecursiveDepth + 1;
						}
						else
						{
							a_MathOperation->m_RecursiveDepth = 0;
						}
						a_PrevMathOperation = a_MathOperation;

						if (l_LeftPrioStr.length( ) > 1)
						{
							OrderRecursive( a_CurrPredecessorCommand, a_MathOperationsParameter, a_ParameterMapFromPredecessor, a_CommandString, l_LeftPrioStr, a_DepthLevel, l_OldCommandIndex, a_MathOperation->m_Left, a_PrevMathOperation );
						}
						else if (l_LeftPrioStr.length( ) == 1)
						{
							a_MathOperation->m_Left = new clsMathOperationValue( );

							if (a_PrevMathOperation != nullptr)
							{
								a_MathOperation->m_Left->m_Previous = a_MathOperation;
								a_MathOperation->m_Left->m_RecursiveDepth = a_MathOperation->m_RecursiveDepth + 1;
							}
							else
							{
								a_MathOperation->m_Left->m_RecursiveDepth = 0;
							}

							SetMathOperationPointerValue( a_CurrPredecessorCommand, a_ParameterMapFromPredecessor, a_CommandString[ a_CommandIndex - 1 ], l_LeftPrioStr[ 0 ], a_MathOperation->m_Left );
							a_MathOperationsParameter->push_back( a_MathOperation->m_Left );
						}

						if (l_RightPrioStr.length( ) > 1)
						{
							OrderRecursive( a_CurrPredecessorCommand, a_MathOperationsParameter, a_ParameterMapFromPredecessor, a_CommandString, l_RightPrioStr, a_DepthLevel, a_CommandIndex, a_MathOperation->m_Right, a_PrevMathOperation );
						}
						else if (l_RightPrioStr.length( ) == 1)
						{
							a_MathOperation->m_Right = new clsMathOperationValue( );

							if (a_PrevMathOperation != nullptr)
							{
								a_MathOperation->m_Right->m_Previous = a_MathOperation;
								a_MathOperation->m_Right->m_RecursiveDepth = a_MathOperation->m_RecursiveDepth + 1;
							}
							else
							{
								a_MathOperation->m_Right->m_RecursiveDepth = 0;
							}

							SetMathOperationPointerValue( a_CurrPredecessorCommand, a_ParameterMapFromPredecessor, a_CommandString[ a_CommandIndex + 1 ], l_RightPrioStr[ 0 ], a_MathOperation->m_Right );
							a_MathOperationsParameter->push_back( a_MathOperation->m_Right );
						}

						a_MathOperationsParameter->push_back( a_MathOperation );
					}
				}
				else if (!isdigit( a_PriorityString.at( i ) ) && a_PriorityString.length( ) == 1)
				{
					a_MathOperation = new clsMathOperationValue( );
					SetMathOperationPointerValue( a_CurrPredecessorCommand, a_ParameterMapFromPredecessor, a_CommandString[ 0 ], a_PriorityString[ 0 ], a_MathOperation );
					a_MathOperationsParameter->push_back( a_MathOperation );
				}
				if (l_IsCorrectOperatorFound)
				{
					break;
				}
			}
			if (l_IsCorrectOperatorFound)
			{
				break;
			}
		}
	}

	void clsLSystem::SetPreviousMathOperation( clsMathOperation** a_CurrMathOperation, clsMathOperation** a_PrevMathOperation )
	{
		if (*a_PrevMathOperation != nullptr)
		{
			( *a_CurrMathOperation )->m_Previous = *a_PrevMathOperation;
			( *a_CurrMathOperation )->m_RecursiveDepth = ( *a_PrevMathOperation )->m_RecursiveDepth + 1;
		}
		else
		{
			( *a_CurrMathOperation )->m_RecursiveDepth = 0;
		}
		a_PrevMathOperation = a_CurrMathOperation;
	}

	void clsLSystem::SetMathOperationPointerValue( clsLSystemPredecessorCommand* a_CurrPredecessorCommand, std::map<char, int>* a_ParameterMapFromPredecessor, char a_Name, char a_PosNegSign, clsMathOperation* a_MathOperation )
	{
		for (auto it = a_ParameterMapFromPredecessor->begin( ); it != a_ParameterMapFromPredecessor->end( ); ++it)
		{
			if (it->first == a_Name)
			{
				if (a_PosNegSign == 'n')
				{
					a_MathOperation->m_PValue = a_CurrPredecessorCommand->GetParameterAt( it->second );
					*a_MathOperation->m_PValue *= -1.0f;
				}
				else if (a_PosNegSign == 'p')
				{
					a_MathOperation->m_PValue = a_CurrPredecessorCommand->GetParameterAt( it->second );
				}
				else
				{
					assert( "Sign was neither negative or positive" == 0 );
				}
			}
		}

		for (auto it2 = m_Defines.begin( ); it2 != m_Defines.end( ); ++it2)
		{
			if (it2->first == a_Name)
			{
				if (a_PosNegSign == 'n')
				{
					a_MathOperation->m_PValue = &it2->second;
					*a_MathOperation->m_PValue *= -1.0f;
				}
				else if (a_PosNegSign == 'p')
				{
					a_MathOperation->m_PValue = &it2->second;
				}
				else
				{
					assert( "Sign was neither negative or positive" == 0 );
				}
			}
		}
	}

	void clsLSystem::ConvertAxiomToObjects( std::string a_Axiom )
	{
		std::vector<std::string> l_SplittedAxiom = Globals::Split( a_Axiom, " " );
		for (size_t i = 0; i < l_SplittedAxiom.size( ); ++i)
		{
			std::string l_CurrCommand = l_SplittedAxiom[ i ];
			char l_CommandName = l_CurrCommand[ 0 ];
			switch (l_CommandName)
			{
				case 'F':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eForward );
					break;
				case '+':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eYawLeft );
					break;
				case '-':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eYawRight );
					break;
				case '\\':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eRollLeft );
					break;
				case '/':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eRollRight );
					break;
				case '^':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::ePitchUp );
					break;
				case '&':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::ePitchDown );
					break;
				case '[':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eBranchStart );
					break;
				case ']':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eBranchEnd );
					break;
				case '!':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eWidth );
					break;
				case '@':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eRoutine );
					break;
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'G':
					ConvertAxiomCommandToObject( l_CurrCommand, enumLSystemCommands::eRule );
					break;
				default:
					break;
			}
		}
	}

	void clsLSystem::ConvertAxiomCommandToObject( std::string a_Command, enumLSystemCommands::enumLSystemCommands a_CommandType )
	{
		clsLSystemSuccessorCommand l_CurrAxiomCommand;
		char l_Name = a_Command[ 0 ];
		l_CurrAxiomCommand.SetName( l_Name );
		l_CurrAxiomCommand.SetCommandType( a_CommandType );
		for (size_t i = 0; i < a_Command.length( ); ++i)
		{
			char l_CurrChar = a_Command[ i ];
			if (i + 1 < a_Command.length( ))
			{
				char l_NextChar = a_Command[ i + 1 ];
				if (l_NextChar == '(')
				{
					std::string l_Digits;
					for (size_t j = i + 2; j < a_Command.length( ); ++j)
					{
						char l_CurrChar2 = a_Command[ j ];
						if (l_CurrChar2 != ',' && l_CurrChar2 != ')')
						{
							l_Digits += l_CurrChar2;
						}
						else if (l_CurrChar2 == ',' || l_CurrChar2 == ')')
						{
							float l_FloatValue = std::stof( l_Digits );
							l_Digits = "";
							l_CurrAxiomCommand.AddAxiomParameter( l_FloatValue );
							if (a_CommandType == enumLSystemCommands::eRule)
							{
								m_ParameterMapFromAxiom[ l_Name ].push_back( l_FloatValue );
							}
						}
					}
				}
				else if (l_NextChar == 'V')
				{
					// set routine in successor
				}
			}
		}
		m_Result.push_back( l_CurrAxiomCommand );
	}

	void clsLSystem::CalculateIteration( )
	{
		std::vector<clsLSystemSuccessorCommand> l_TempResult;
		l_TempResult.reserve( (size_t) 2048 ); // Temp
		l_TempResult = m_Result;
		for (size_t i = m_Result.size( ); i-- > 0;)
		{
			for (size_t j = 0; j < m_Rules.size( ); ++j)
			{
				if (m_Result[ i ].GetName( ) == m_Rules[ j ].GetName( ))
				{
					std::vector<float> l_AxiomParameters = m_Result[ i ].GetAxiomParameters( );

					for (size_t k = 0; k < l_AxiomParameters.size( ); ++k)
					{
						m_Rules[ j ].SetParameterAt( (int) k, l_AxiomParameters[ k ] );
					}

					std::vector<clsLSystemSuccessorCommand>* l_Successor = m_Rules[ j ].GetSuccessor( );

					for (size_t k = 0; k < l_Successor->size( ); ++k)
					{
						std::vector<std::vector<clsMathOperation*>> l_MathOperationsParameters = l_Successor->at( k ).GetMathOperationsParameters( );
						for (size_t l = 0; l < l_MathOperationsParameters.size( ); ++l)
						{
							std::vector<clsMathOperation*> l_MathOperationsParameter = l_MathOperationsParameters[ l ];
							for (size_t m = 0; m < l_MathOperationsParameter.size( ); ++m)
							{
								l_MathOperationsParameter[ m ]->Calculate( );
							}

							float l_ParameterValue;
							size_t l_SuccessorParameterSize = l_Successor->at( k ).GetSuccessorParameters( ).size( );
							if (l_MathOperationsParameter.size( ) > 0 && l_SuccessorParameterSize > 0)
							{
								l_ParameterValue = *l_MathOperationsParameter[ l_MathOperationsParameter.size( ) - 1 ]->m_PValue;
								l_Successor->at( k ).SetSuccessorParameterAt( (int) l, l_ParameterValue );
							}
						}
						l_Successor->at( k ).SetAxiomParameters( l_Successor->at( k ).GetSuccessorParameters( ) );
					}
					std::vector<clsLSystemSuccessorCommand> l_SuccessorObjects;
					l_SuccessorObjects.reserve( l_Successor->size( ) );
					for (size_t a = 0; a < l_Successor->size( ); ++a)
					{
						l_SuccessorObjects.push_back( l_Successor->at( a ) );
					}
					l_TempResult.erase( l_TempResult.begin( ) + i );
					l_TempResult.insert( l_TempResult.begin( ) + i, l_SuccessorObjects.begin( ), l_SuccessorObjects.end( ) );
					break;
				}
			}
		}
		m_Result = l_TempResult;
	}

	void clsLSystem::TranslateStringToTurtle( )
	{
		m_Turtle->SetRotation( -90, 0, 0 ); // Face turtle ( 0, 1, 0 ) (up) direction
		clsPoint l_TreePoint;
		l_TreePoint.Init( m_Turtle->GetTranslation( ), m_Turtle->GetRotation( ) );
		l_TreePoint.m_Width = m_Result[ 0 ].GetAxiomParameters( )[ 0 ];
		m_TreePoints.push_back( l_TreePoint ); // Add start position

		glm::vec3 l_Scale = glm::vec3( 50, 50, 50 ); // scale to multiply the distances with
		std::vector<clsPoint> l_VisitedBranchingPoints;
		glm::quat l_PrevBranchingPointRot;
		glm::vec3 l_PrevBranchingPointPos;
		float l_Width = 0.0f;
		float l_DefaultValue = 1.0f;
		float l_Parameter;

		for (size_t i = 0; i < m_Result.size( ); ++i)
		{
			enumLSystemCommands::enumLSystemCommands l_CurrCommandType = m_Result[ i ].GetCommandType( );
			std::vector<float> l_Parameters = m_Result[ i ].GetAxiomParameters( );

			l_Parameter = l_DefaultValue;
			if (l_Parameters.size( ) > 0)
			{
				l_Parameter = l_Parameters[ 0 ];
			}

			switch (l_CurrCommandType)
			{
				case enumLSystemCommands::eForward:
					MoveForward( l_TreePoint, l_Scale * l_Parameter, l_Width );
					break;
				case enumLSystemCommands::eYawLeft:
					SetYaw( l_Parameters, -1 );
					break;
				case enumLSystemCommands::eYawRight:
					SetYaw( l_Parameters, 1 );
					break;
				case enumLSystemCommands::eRollLeft:
					SetRoll( l_Parameters, -1 );
					break;
				case enumLSystemCommands::eRollRight:
					SetRoll( l_Parameters, 1 );
					break;
				case enumLSystemCommands::ePitchUp:
					SetPitch( l_Parameters, -1 );
					break;
				case enumLSystemCommands::ePitchDown:
					SetPitch( l_Parameters, 1 );
					break;
				case enumLSystemCommands::eBranchStart:
					SetBranchingPoint( l_VisitedBranchingPoints );
					break;
				case enumLSystemCommands::eBranchEnd:
					SetReturnPoint( l_VisitedBranchingPoints, l_TreePoint, l_PrevBranchingPointRot, l_PrevBranchingPointPos );
					break;
				case enumLSystemCommands::eWidth:
					SetWidth( l_Parameters, l_Width );
					break;
				case enumLSystemCommands::eRoutine:
					SetTurtleRightVectorToHorizontal( );
					break;
				default:
					break;
			}
		}
	}

	void clsLSystem::MoveForward( clsPoint& r_Point, glm::vec3 a_Distance, float a_Width )
	{
		m_Turtle->Translate( m_Turtle->GetForward( ) * a_Distance );

		r_Point = clsPoint( );
		r_Point.Init( m_Turtle->GetTranslation( ), m_Turtle->GetRotation( ) );
		r_Point.m_Width = a_Width;
		m_TreePoints.push_back( r_Point );
	}

	void clsLSystem::SetYaw( std::vector<float> a_Parameters, int a_PosNegValue )
	{
		if (a_Parameters.size( ) == 0)
		{
			m_Turtle->Rotate( 0, m_Angle * a_PosNegValue, 0 );
		}
		else
		{
			m_Turtle->Rotate( 0, a_Parameters[ 0 ] * a_PosNegValue, 0 );
		}
	}

	void clsLSystem::SetPitch( std::vector<float> a_Parameters, int a_PosNegValue )
	{
		if (a_Parameters.size( ) == 0)
		{
			m_Turtle->Rotate( m_Angle * a_PosNegValue, 0, 0 );
		}
		else
		{
			m_Turtle->Rotate( a_Parameters[ 0 ] * a_PosNegValue, 0, 0 );
		}
	}

	void clsLSystem::SetRoll( std::vector<float> a_Parameters, int a_PosNegValue )
	{
		if (a_Parameters.size( ) == 0)
		{
			m_Turtle->Rotate( 0, 0, m_Angle * a_PosNegValue );
		}
		else
		{
			m_Turtle->Rotate( 0, 0, a_Parameters[ 0 ] * a_PosNegValue );
		}
	}

	void clsLSystem::SetBranchingPoint( std::vector<clsPoint> &r_VisitedBranchingPoints )
	{
		m_TreePoints[ m_TreePoints.size( ) - 1 ].m_IsBranchingPoint = true;
		r_VisitedBranchingPoints.push_back( m_TreePoints[ m_TreePoints.size( ) - 1 ] );
	}

	void clsLSystem::SetReturnPoint( std::vector<clsPoint>& r_VisitedBranchingPoints, clsPoint& r_Point, glm::quat a_PrevBranchingPointRot, glm::vec3 a_PrevBranchingPointPos )
	{
		m_TreePoints[ m_TreePoints.size( ) - 1 ].m_IsEndOfBranch = true;

		a_PrevBranchingPointRot = r_VisitedBranchingPoints[ r_VisitedBranchingPoints.size( ) - 1 ].m_Rotation;
		m_Turtle->SetRotation( a_PrevBranchingPointRot );
		a_PrevBranchingPointPos = r_VisitedBranchingPoints[ r_VisitedBranchingPoints.size( ) - 1 ].m_Position;
		m_Turtle->SetTranslation( a_PrevBranchingPointPos );

		r_Point = clsPoint( r_VisitedBranchingPoints[ r_VisitedBranchingPoints.size( ) - 1 ] );
		r_Point.m_IsReturnPoint = true;
		m_TreePoints.push_back( r_Point );

		r_VisitedBranchingPoints.erase( r_VisitedBranchingPoints.begin( ) + r_VisitedBranchingPoints.size( ) - 1 );
	}

	void clsLSystem::SetWidth( std::vector<float> a_Parameters, float &r_Width )
	{
		if (a_Parameters.size( ) > 0)
		{
			r_Width = a_Parameters[ 0 ];
		}
	}

	std::vector<clsPoint> clsLSystem::GetJoints( )
	{
		return m_TreePoints;
	}

	void clsLSystem::SetAngle( float a_Angle )
	{
		m_Angle = a_Angle;
	}

	void clsLSystem::SetAmountOfIterations( int a_AmountOfIterations )
	{
		m_AmountOfIterations = a_AmountOfIterations;
	}

	void clsLSystem::SetAxiom( std::string a_Axiom )
	{
		m_Axiom = a_Axiom;
	}

	void clsLSystem::SetDefine( char a_Name, float a_Value )
	{
		m_Defines[ a_Name ] = a_Value;
	}

	void clsLSystem::SetRule( char a_Name, std::string a_Successor, std::string a_Parameters )
	{
		std::pair<char, std::string> l_NameAndParams;
		l_NameAndParams.first = a_Name;
		l_NameAndParams.second = a_Parameters;
		m_UnparsedRules[ l_NameAndParams ] = a_Successor;
	}

	void clsLSystem::SetTurtleRightVectorToHorizontal( )
	{
		float l_AngleToHorizontalRight = glm::degrees( 0.0f - m_Turtle->GetRight( ).y );

		if (m_Turtle->GetUp( ).y < 0.0f)
		{
			l_AngleToHorizontalRight += 180.0f;
		}

		m_Turtle->Rotate( 0.0f, 0.0f, l_AngleToHorizontalRight );
	}
}
