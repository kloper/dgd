/*
  File autogenerated by gengetopt version 2.4  
  generated with the following command:
  gengetopt --no-handle-help --no-handle-version --no-handle-error --file-name=dgDebugOpt --func-name=dgd_cmdline_parser 

  The developers of gengetopt consider the fixed text that goes in all
  gengetopt output files to be in the public domain:
  we make no copyright claims on it.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* If we use autoconf.  */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
/* Check for configure's getopt check result.  */
#ifndef HAVE_GETOPT_LONG
#include "getopt.h"
#else
#include <getopt.h>
#endif

#include "dgDebugOpt.h"


void
dgd_cmdline_parser_print_version (void)
{
  printf ("%s %s\n", PACKAGE, VERSION);
}

void
dgd_cmdline_parser_print_help (void)
{
  dgd_cmdline_parser_print_version ();
  printf("\n"
"Usage: %s [OPTIONS]...\n\
   -h         --help                           Print help and exit\n\
   -V         --version                        Print version and exit\n\
              --debug-version                  Print dgd version and exit (default=off)\n\
              --debug-help                     Print debug help and exit (default=off)\n\
              --debug-enable                   Enable debug (default=off)\n\
              --debug-main-file=STRING         Primary debug output file\n\
              --debug-min-width=INT            Default minimum output width\n\
              --debug-max-width=INT            Default maximum output width\n\
              --debug-indent-step=INT          Default indent step\n\
              --debug-allow-wrap               Allow character wrap by default (default=on)\n\
              --debug-allow-word-wrap          Allow word wrap by default (default=on)\n\
              --debug-space-characters=STRING  Characters separating words\n\
              --debug-turn-on=STRING           Regexp on channels to turn on\n\
              --debug-turn-off=STRING          Regexp on channels to turn off\n\
", PACKAGE);
}


static char *
gengetopt_strdup (char * s)
{
  char * n, * pn, * ps = s;
  while (*ps) ps++;
  n = (char *) malloc (1 + ps - s);
  if (n != NULL)
    {
      for (ps=s,pn=n; *ps; ps++,pn++)
        *pn = *ps;
      *pn = 0;
    }
  return n;
}


int
dgd_cmdline_parser (int argc, char * const *argv, struct dgd_gengetopt_args_info *args_info)
{
  int c;	/* Character of the parsed option.  */
  int missing_required_options = 0;	

  args_info->help_given = 0 ;
  args_info->version_given = 0 ;
  args_info->debug_version_given = 0 ;
  args_info->debug_help_given = 0 ;
  args_info->debug_enable_given = 0 ;
  args_info->debug_main_file_given = 0 ;
  args_info->debug_min_width_given = 0 ;
  args_info->debug_max_width_given = 0 ;
  args_info->debug_indent_step_given = 0 ;
  args_info->debug_allow_wrap_given = 0 ;
  args_info->debug_allow_word_wrap_given = 0 ;
  args_info->debug_space_characters_given = 0 ;
  args_info->debug_turn_on_given = 0 ;
  args_info->debug_turn_off_given = 0 ;
#define clear_args() { \
  args_info->debug_version_flag = 0;\
  args_info->debug_help_flag = 0;\
  args_info->debug_enable_flag = 0;\
  args_info->debug_main_file_arg = NULL; \
  args_info->debug_allow_wrap_flag = 1;\
  args_info->debug_allow_word_wrap_flag = 1;\
  args_info->debug_space_characters_arg = NULL; \
  args_info->debug_turn_on_arg = NULL; \
  args_info->debug_turn_off_arg = NULL; \
}

  clear_args();

  optarg = 0;
  optind = 1;
  opterr = 1;
  optopt = '?';

  while (1)
    {
      int option_index = 0;
      static struct option long_options[] = {
        { "help",	0, NULL, 'h' },
        { "version",	0, NULL, 'V' },
        { "debug-version",	0, NULL, 0 },
        { "debug-help",	0, NULL, 0 },
        { "debug-enable",	0, NULL, 0 },
        { "debug-main-file",	1, NULL, 0 },
        { "debug-min-width",	1, NULL, 0 },
        { "debug-max-width",	1, NULL, 0 },
        { "debug-indent-step",	1, NULL, 0 },
        { "debug-allow-wrap",	0, NULL, 0 },
        { "debug-allow-word-wrap",	0, NULL, 0 },
        { "debug-space-characters",	1, NULL, 0 },
        { "debug-turn-on",	1, NULL, 0 },
        { "debug-turn-off",	1, NULL, 0 },
        { NULL,	0, NULL, 0 }
      };

      c = getopt_long (argc, argv, "hV", long_options, &option_index);

      if (c == -1) break;	/* Exit from `while (1)' loop.  */

      switch (c)
        {
        case 'h':	/* Print help and exit.  */
          if (args_info->help_given)
            {
              fprintf (stderr, "%s: `--help' (`-h') option given more than once\n", PACKAGE);
              clear_args ();
              return (1);
            }
          args_info->help_given = 1;
          return 0;

        case 'V':	/* Print version and exit.  */
          if (args_info->version_given)
            {
              fprintf (stderr, "%s: `--version' (`-V') option given more than once\n", PACKAGE);
              clear_args ();
              return (1);
            }
          args_info->version_given = 1;
          return 0;

        
        
        
        
        
        
        
        
        
        
        
        
        case 0:	/* Long option with no short option */
          /* Print dgd version and exit.  */
          if (strcmp (long_options[option_index].name, "debug-version") == 0)
          {
            args_info->debug_version_flag = !(args_info->debug_version_flag);
            break;
          }
          /* Print debug help and exit.  */
          else if (strcmp (long_options[option_index].name, "debug-help") == 0)
          {
            args_info->debug_help_flag = !(args_info->debug_help_flag);
            break;
          }
          /* Enable debug.  */
          else if (strcmp (long_options[option_index].name, "debug-enable") == 0)
          {
            args_info->debug_enable_flag = !(args_info->debug_enable_flag);
            break;
          }
          /* Primary debug output file.  */
          else if (strcmp (long_options[option_index].name, "debug-main-file") == 0)
          {
            if (args_info->debug_main_file_given)
              {
                fprintf (stderr, "%s: `--debug-main-file' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_main_file_given = 1;
            args_info->debug_main_file_arg = gengetopt_strdup (optarg);
            break;
          }
          /* Default minimum output width.  */
          else if (strcmp (long_options[option_index].name, "debug-min-width") == 0)
          {
            if (args_info->debug_min_width_given)
              {
                fprintf (stderr, "%s: `--debug-min-width' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_min_width_given = 1;
            args_info->debug_min_width_arg = atoi (optarg);
            break;
          }
          /* Default maximum output width.  */
          else if (strcmp (long_options[option_index].name, "debug-max-width") == 0)
          {
            if (args_info->debug_max_width_given)
              {
                fprintf (stderr, "%s: `--debug-max-width' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_max_width_given = 1;
            args_info->debug_max_width_arg = atoi (optarg);
            break;
          }
          /* Default indent step.  */
          else if (strcmp (long_options[option_index].name, "debug-indent-step") == 0)
          {
            if (args_info->debug_indent_step_given)
              {
                fprintf (stderr, "%s: `--debug-indent-step' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_indent_step_given = 1;
            args_info->debug_indent_step_arg = atoi (optarg);
            break;
          }
          /* Allow character wrap by default.  */
          else if (strcmp (long_options[option_index].name, "debug-allow-wrap") == 0)
          {
            args_info->debug_allow_wrap_flag = !(args_info->debug_allow_wrap_flag);
            break;
          }
          /* Allow word wrap by default.  */
          else if (strcmp (long_options[option_index].name, "debug-allow-word-wrap") == 0)
          {
            args_info->debug_allow_word_wrap_flag = !(args_info->debug_allow_word_wrap_flag);
            break;
          }
          /* Characters separating words.  */
          else if (strcmp (long_options[option_index].name, "debug-space-characters") == 0)
          {
            if (args_info->debug_space_characters_given)
              {
                fprintf (stderr, "%s: `--debug-space-characters' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_space_characters_given = 1;
            args_info->debug_space_characters_arg = gengetopt_strdup (optarg);
            break;
          }
          /* Regexp on channels to turn on.  */
          else if (strcmp (long_options[option_index].name, "debug-turn-on") == 0)
          {
            if (args_info->debug_turn_on_given)
              {
                fprintf (stderr, "%s: `--debug-turn-on' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_turn_on_given = 1;
            args_info->debug_turn_on_arg = gengetopt_strdup (optarg);
            break;
          }
          /* Regexp on channels to turn off.  */
          else if (strcmp (long_options[option_index].name, "debug-turn-off") == 0)
          {
            if (args_info->debug_turn_off_given)
              {
                fprintf (stderr, "%s: `--debug-turn-off' option given more than once\n", PACKAGE);
                clear_args ();
                return (1);
              }
            args_info->debug_turn_off_given = 1;
            args_info->debug_turn_off_arg = gengetopt_strdup (optarg);
            break;
          }

        case '?':	/* Invalid option.  */
          /* `getopt_long' already printed an error message.  */
          return (1);

        default:	/* bug: option not considered.  */
          fprintf (stderr, "%s: option unknown: %c\n", PACKAGE, c);
          abort ();
        } /* switch */
    } /* while */

  if ( missing_required_options )
    return (1);


  return 0;
}
