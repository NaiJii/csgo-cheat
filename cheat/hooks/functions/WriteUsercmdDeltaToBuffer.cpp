#include "../hooks.h"

namespace n_hooks {
	namespace n_functions {
		bool __fastcall WriteUsercmdDeltaToBuffer( std::uintptr_t ecx, std::uintptr_t edx, int slot, void* buf, int from, int to, bool isnewcommand ) {
			static auto original_fn = vmt_manager->get_original_fn< bool( __thiscall* )( std::uintptr_t, int, void*, int, int, bool ) >( WriteUsercmdDeltaToBuffer );

			// if all else fails we can just byte patch
			/*
				// address found in CL_SendMove
				static auto MAX_NEW_COMMANDS = n_utilities::find_pattern( "engine.dll", "B8 ? ? ? ? 3B F0 0F 4F F0 89" ) + 0x1;
				
				// protect memory region and store old page protection
				const auto old_protection = n_utilities::protect_full_memory_region( MAX_NEW_COMMANDS, PAGE_EXECUTE_READWRITE );

				// modify MAX_NEW_COMMANDS because its clientside and change it to 17 as sv_maxusrcmdprocessticks is defaulted to 16
				*reinterpret_cast< std::uintptr_t* >( MAX_NEW_COMMANDS ) = 17;

				// revert page protection
				n_utilities::protect_full_memory_region( MAX_NEW_COMMANDS, old_protection );
			*/

			// first command is deltaed against zeros 
			if ( from == -1 ) {
				// variable on the stack for esp
				std::uintptr_t* _esp;
				// grabbing esp - how do i not use inline asm??
				__asm mov _esp, esp;

				// calculating the ebp on the stack
				const auto ebp = *reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) - sizeof( std::uintptr_t* ) );

				// loop counter is stored in the current stake frame in the esi register
				std::uintptr_t* loop_counter = nullptr;

				// finding esi on the stack ( esi is compiler and stack size dependent so we have to loop through hgiher address to find it )
				for ( int i = 0; i < 30; i++ ) {
					// checking if the value stored by esp is equal to the value of to
					if ( _esp[ i ] == to ) {
						// we have found the correct value -> time to store the address of esi in the previous stack frame
						loop_counter = &_esp[ i ];
						break;
					}				
				}

				// sanity check
				if ( !loop_counter )
					return original_fn( ecx, slot, buf, from, to, isnewcommand );

				// getting the relative CHL_Move stack variable
				const auto move_msg = reinterpret_cast< void* >( ebp - 0x58 );

				// getting the relative unknown comparision varialbe ( v31 = lastCommand - newCommands; )
				const auto last_outgoing_command = reinterpret_cast< int* >( ebp - 0x4 ); // unsigned / signed mismatch grr!

				// creating variables on the stack
				const auto last_command = n_interfaces::client_state->m_last_outgoing_command + n_interfaces::client_state->m_chocked_commands + 1;
				const auto new_commands = n_interfaces::client_state->m_chocked_commands + 1;

				// modify command_comparison
				*last_outgoing_command = last_command - new_commands; // just doing what ida does but it is the same as, n_interfaces::client_state->m_last_outgoing_command

				// modify move_msg.new_commands
				*reinterpret_cast< std::uintptr_t* >( reinterpret_cast< std::uintptr_t >( move_msg ) + 0x10 ) = new_commands;

				// modify the loop counter
				*loop_counter = last_command - new_commands - 1;

				// modify the argument "to"
				to = *loop_counter;

				// modify the argument "isnewcommand"
				isnewcommand = to >= *last_outgoing_command + 1;
			}

			return original_fn( ecx, slot, buf, from, to, isnewcommand );
		}
	}
}