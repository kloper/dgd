/* dgDebugOpt.h */

/* File autogenerated by gengetopt version 2.4  */

#ifndef _dgDebugOpt_h
#define _dgDebugOpt_h

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Don't define PACKAGE and VERSION if we use automake.  */
#if defined PACKAGE
#  undef PACKAGE
#endif
#define PACKAGE "dgd"
#if defined VERSION
#  undef VERSION
#endif
#define VERSION "dgd-0.19 Fri Nov  1 22:40:16  2002"

struct dgd_gengetopt_args_info {
  int debug_version_flag;	/* Print dgd version and exit (default=off).  */
  int debug_help_flag;	/* Print debug help and exit (default=off).  */
  int debug_enable_flag;	/* Enable debug (default=off).  */
  char * debug_main_file_arg;	/* Primary debug output file.  */
  int debug_min_width_arg;	/* Default minimum output width.  */
  int debug_max_width_arg;	/* Default maximum output width.  */
  int debug_indent_step_arg;	/* Default indent step.  */
  int debug_allow_wrap_flag;	/* Allow character wrap by default (default=on).  */
  int debug_allow_word_wrap_flag;	/* Allow word wrap by default (default=on).  */
  char * debug_space_characters_arg;	/* Characters separating words.  */
  char * debug_turn_on_arg;	/* Regexp on channels to turn on.  */
  char * debug_turn_off_arg;	/* Regexp on channels to turn off.  */

  int help_given ;	/* Whether help was given.  */
  int version_given ;	/* Whether version was given.  */
  int debug_version_given ;	/* Whether debug-version was given.  */
  int debug_help_given ;	/* Whether debug-help was given.  */
  int debug_enable_given ;	/* Whether debug-enable was given.  */
  int debug_main_file_given ;	/* Whether debug-main-file was given.  */
  int debug_min_width_given ;	/* Whether debug-min-width was given.  */
  int debug_max_width_given ;	/* Whether debug-max-width was given.  */
  int debug_indent_step_given ;	/* Whether debug-indent-step was given.  */
  int debug_allow_wrap_given ;	/* Whether debug-allow-wrap was given.  */
  int debug_allow_word_wrap_given ;	/* Whether debug-allow-word-wrap was given.  */
  int debug_space_characters_given ;	/* Whether debug-space-characters was given.  */
  int debug_turn_on_given ;	/* Whether debug-turn-on was given.  */
  int debug_turn_off_given ;	/* Whether debug-turn-off was given.  */
} ;

int dgd_cmdline_parser (int argc, char * const *argv, struct dgd_gengetopt_args_info *args_info);

void dgd_cmdline_parser_print_help(void);
void dgd_cmdline_parser_print_version(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _dgDebugOpt_h */
