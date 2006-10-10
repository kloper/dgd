/*
 * This file was automaticly generated by clo++ version 0.6.4 (REL-0_6_4)
 * http://pmade.org/pjones/software/clo++/
 */

/** @file
 * This is the main header file (dgd\Debug\dgDebugOpt.h).
 * You can control the name of this header file using the
 * cxx_header_file variable.
**/

// you can control the include guard
// name with the cxx_header_def variable
#ifndef __dgd_Debug_dgDebugOpt_h__
#define __dgd_Debug_dgDebugOpt_h__


// standard includes
#include <stdexcept>
#include <string>
#include <vector>

/// the namespace can be controlled using the cxx_namespace variable
namespace DGD {
    /**
     * a list of autothrow ids for the autoexcept exception class
     * so that you know which option caused the exception.
    **/
    enum autothrow { 
	autothrow_help
    };

    /**
     * the following struct is used to hold the values of
     * the options. It has a constructor that sets all the option 
     * values to their default value.
    **/
    struct DGD_EXPORT options {
	options (void) :
	    trace_allow_word_wrap(true),
	    trace_allow_wrap(true),
	    trace_enable(false),
	    trace_help(false),
	    trace_indent_step(4),
	    trace_main_file("debug.log"),
	    trace_max_width(79),
	    trace_min_width(20),
	    trace_space_characters("\\t"),
	    trace_turn_off("."),
	    trace_turn_on("."),
	    trace_version(false)
	{ }

	bool trace_allow_word_wrap;
	bool trace_allow_wrap;
	bool trace_enable;
	bool trace_help;
	int trace_indent_step;
	std::string trace_main_file;
	int trace_max_width;
	int trace_min_width;
	std::string trace_space_characters;
	std::string trace_turn_off;
	std::string trace_turn_on;
	bool trace_version;
    }; // end options struct
 
    /**
     * the following struct is used to record the location
     * on the command line where the option was found.
    **/
    struct DGD_EXPORT option_locations {
	typedef int size_type;
	size_type trace_allow_word_wrap;
	size_type trace_allow_wrap;
	size_type trace_enable;
	size_type trace_help;
	size_type trace_indent_step;
	size_type trace_main_file;
	size_type trace_max_width;
	size_type trace_min_width;
	size_type trace_space_characters;
	size_type trace_turn_off;
	size_type trace_turn_on;
	size_type trace_version;
    }; // end option location struct

    /**
     * if there are any errors while parsing the command
     * line, the following class will be thrown.
    **/
    class DGD_EXPORT option_error : public std::runtime_error {
    public:
	option_error (const std::string &what_arg)
	    : runtime_error(what_arg) { }
	
	const char* get_help_comment (void) const;
    };

    /**
     * if any option that is a autothrow option is present
     * on the command line, this exception class will be
     * thrown when that option is parsed.
    **/
    class DGD_EXPORT autoexcept : public option_error {
    public:
	// constructor
	autoexcept (autothrow id, const std::string &message)
	    : option_error(message), autothrow_(id) { }
    
	/**
	 * get the autothrow enum member for the autothrow
	 * option that caused the exception.
	**/
	autothrow get_autothrow_id (void) const
	{ return autothrow_; }
    private:
	autothrow autothrow_;
    };

    /**
     * the parser class is the main command
     * line parsing class. You can control the name of it
     * using the cxx_class variable.
    **/
    class DGD_EXPORT parser {
    public:
	/// default constructor
	parser (void);
    
	/// parse the command line
	void parse(int argc, char *argv[], bool call_finalize=true);

	/// finalize all parsing (eg. check for mandatory options)
	void finalize (void);

	/// get a list of nonoptions from the command line
	const std::vector<std::string>& get_non_options (void) const
	{ return non_options_; }

	/// get the main options
	const options& get_options (void) const
	{ return options_; }

	/// get the main option locations
	const option_locations& get_locations (void) const
	{ return locations_; }
    private:
	options options_;
	option_locations locations_;

	enum options_enum {
	    option_trace_version,
		option_trace_help,
		option_trace_enable,
		option_trace_main_file,
		option_trace_min_width,
		option_trace_max_width,
		option_trace_indent_step,
		option_trace_allow_wrap,
		option_trace_allow_word_wrap,
		option_trace_space_characters,
		option_trace_turn_on,
		option_trace_turn_off
	} openum_;

	enum parser_state { state_option, state_value, state_consume } state_;
	std::vector<std::string> non_options_;

	enum opsource { source_cl, source_cf };
	void parse_element      (const char *element, int position, opsource source);
	void parse_short_option (char option, int position, opsource source);
	void parse_long_option  (const char *option, int position, opsource source);
	void parse_value        (const char *value);
    }; // end DGD::parser class
} // end DGD namespace
#endif
