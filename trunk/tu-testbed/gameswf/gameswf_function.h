// gameswf_function.h	-- Thatcher Ulrich <tu@tulrich.com> 2003

// This source code has been donated to the Public Domain.  Do
// whatever you want with it.

// ActionScript function.

#ifndef GAMESWF_FUNCTION_H
#define GAMESWF_FUNCTION_H

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_environment.h"
#include "gameswf/gameswf_object.h"

#include "base/container.h"
#include "base/smart_ptr.h"

namespace gameswf
{
	struct as_environment;
	struct as_object;

	struct as_as_function : public ref_counted
	{
		action_buffer*	m_action_buffer;

		// We need to watch m_env
		// testcase:
		// the movie executes the following opcodes
		// _global.x=new Object();
		// loadMovie("another.swf", _root);
		// the result of this:
		// current movie will be deleted and it environment too
		weak_ptr<as_environment>	m_env;
		
		array<with_stack_entry>	m_with_stack;	// initial with-stack on function entry.
		int	m_start_pc;
		int	m_length;
		struct arg_spec
		{
			int	m_register;
			tu_string	m_name;
		};
		array<arg_spec>	m_args;
		bool	m_is_function2;
		uint8	m_local_register_count;
		uint16	m_function2_flags;	// used by function2 to control implicit arg register assignments

		// ActionScript functions have a property namespace!
		// Typically used for class constructors, for "prototype", "constructor",
		// and class properties.
		smart_ptr<as_object>	m_properties;

		// NULL environment is allowed -- if so, then
		// functions will be executed in the caller's
		// environment, rather than the environment where they
		// were defined.
		as_as_function(action_buffer* ab, as_environment* env, int start, const array<with_stack_entry>& with_stack);
		~as_as_function();

		void	set_is_function2() { m_is_function2 = true; }
		void	set_local_register_count(uint8 ct) { assert(m_is_function2); m_local_register_count = ct; }
		void	set_function2_flags(uint16 flags) { assert(m_is_function2); m_function2_flags = flags; }

		void	add_arg(int arg_register, const char* name)
		{
			assert(arg_register == 0 || m_is_function2 == true);
			m_args.resize(m_args.size() + 1);
			m_args.back().m_register = arg_register;
			m_args.back().m_name = name;
		}

		void	set_length(int len) { assert(len >= 0); m_length = len; }

		// Dispatch.
		void	operator()(const fn_call& fn);
	};

}

#endif
