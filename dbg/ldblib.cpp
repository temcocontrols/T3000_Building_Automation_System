
#line 282 "ldb.nw"

#line 2585 "ldb.nw"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "T3000Debug.h"
#include "T3000Debuglib.h"
#include "T3000Debugdebug.h"
#include "ldblib.h"

#line 282 "ldb.nw"


#line 531 "ldb.nw"
static void verify_line_counters (void);

#line 554 "ldb.nw"
static void execute_global_action (void);

#line 589 "ldb.nw"
static void check_current_location (void);

#line 624 "ldb.nw"
static void call_procedure (char *file, int line);

#line 643 "ldb.nw"
static void verify_call_stops (void);

#line 672 "ldb.nw"
static void verify_call_counters (void);

#line 697 "ldb.nw"
static void return_procedure (char *file, int line);

#line 727 "ldb.nw"
static void verify_return_stops (void);

#line 794 "ldb.nw"
static void command_line (void);

#line 811 "ldb.nw"
static void ldb_linehook (int line);
static void ldb_callhook (T3000Debug_Object func, char *file, int line);

#line 861 "ldb.nw"
static void execute_command (char *s);

#line 990 "ldb.nw"
static int local_env (int level);

#line 1059 "ldb.nw"
static int global_env (int level);

#line 1711 "ldb.nw"
static void list_lines (char *file_name, int first, int num_lines);

#line 1746 "ldb.nw"
static void ldb_next (void);

#line 283 "ldb.nw"


#line 337 "ldb.nw"
#define IN_RUN_MODE	(mode == RUN_MODE)
#define IN_STEP_MODE	(mode == STEP_MODE)
#define IN_NEXT_MODE	(mode == NEXT_MODE)

#line 1180 "ldb.nw"
#define LDB_LINE_NUMBER		1
#define LDB_CALL_FUNCTION	0

#line 1543 "ldb.nw"
#define TAM_BUF 512

#line 2118 "ldb.nw"
#define ldbI_existbreak()	(last_mark > 0)

#line 2194 "ldb.nw"
#define ldbI_exprfunc(_)	(exprs[_])

#line 2241 "ldb.nw"
#define ldbI_existGAction()	(GActionString)

#line 2247 "ldb.nw"
#define ldbI_GAction()	(GActionFunc)

#line 2253 "ldb.nw"
#define ldbI_GActionString()	(GActionString)

#line 284 "ldb.nw"


#line 171 "ldb.nw"
typedef struct {
  int line;
  char *file;
} location;

#line 1996 "ldb.nw"
typedef struct Marks
{
  location loc;
  int  code;
} Marks;

#line 2131 "ldb.nw"
#define MAX_EXPRESSIONS 30
static int exprs[MAX_EXPRESSIONS];

#line 285 "ldb.nw"


#line 179 "ldb.nw"
static location curr_loc = {0, NULL};

#line 316 "ldb.nw"
static int stop_now = 0;

#line 323 "ldb.nw"
static int line_count;

#line 331 "ldb.nw"
enum {RUN_MODE, STEP_MODE, NEXT_MODE} mode = RUN_MODE;

#line 350 "ldb.nw"
enum {OFF_HOOK, ON_HOOK, DONT_TURN_ON} line_on = OFF_HOOK,
                                       call_on = OFF_HOOK;

#line 357 "ldb.nw"
static int func_level;

#line 824 "ldb.nw"
static int shortcut[256];

#line 910 "ldb.nw"
static int global_ref;

#line 1399 "ldb.nw"
static int curr_level = 0;

#line 2006 "ldb.nw"
#define MAX_MARKS 30
static Marks marks[MAX_MARKS];
static int last_mark;

#line 2137 "ldb.nw"
static int last_expr;

#line 2209 "ldb.nw"
static char *GActionString = NULL;
static int GActionFunc = -1;

#line 286 "ldb.nw"


#line 238 "ldb.nw"
static int create_action (char *body, char *params)
{
  char s[512];
  int i;
  T3000Debug_beginblock ();
    sprintf (s, "function dummy (%s) %s end", params, body);
    if (T3000Debug_dostring(s))
      T3000Debug_error ("ldb: incorrect action format");
    T3000Debug_pushobject (T3000Debug_getglobal ("dummy"));
    i = T3000Debug_ref (1);
  T3000Debug_endblock ();
  return i;
}

#line 258 "ldb.nw"
static void do_action (int action, char *arg)
{
  T3000Debug_Object func;
  T3000Debug_beginblock ();
    func = T3000Debug_getref (action);
    if (arg)
      T3000Debug_pushstring (arg);
    T3000Debug_callfunction (func);
  T3000Debug_endblock ();
}

#line 2014 "ldb.nw"
static void ldbI_initmarks (void)
{
  last_mark = 0;
}

#line 2027 "ldb.nw"
static int ldbI_alreadyset (location *loc)
{
  int i = 0;
  while (i < last_mark)
  {
    if (marks[i].loc.line == loc->line)
      if (!strcmp (marks[i].loc.file, loc->file))
        return i;
    i++;
  }
  return last_mark;
}

#line 2046 "ldb.nw"
static void ldbI_setbreak (location *loc, int code)
{
  int m = ldbI_alreadyset (loc);
  if (m == last_mark)
    if (last_mark < MAX_MARKS)
      last_mark++;
    else
    {
      /* too much break points */
      return;
    }
  marks[m].loc = *loc;
  marks[m].code = code;
}

#line 2066 "ldb.nw"
static int ldbI_getcode (location *loc)
{
  int i = ldbI_alreadyset (loc);
  if (i == last_mark)
    return 0;
  else
    return marks[i].code;
}

#line 2083 "ldb.nw"
static void ldbI_delete (location *loc)
{
  int i = 0;
  while (i < last_mark)
  {
    if (marks[i].loc.line == loc->line)
      if (!strcmp (marks[i].loc.file, loc->file))
      {
        T3000Debug_unref (marks[i].code);
        last_mark--;
        marks[i] = marks[last_mark];
        return;
      }
    i++;
  }
}

#line 2105 "ldb.nw"
static int ldbI_getbreak (int n, location *loc, int *code)
{
  if (n >= last_mark)
    return 0;
  *loc = marks[n].loc;
  *code = marks[n].code;
  return 1;
}

#line 2142 "ldb.nw"
static void ldbI_initexprs (void)
{
  int i;
  for (i=0; i<MAX_EXPRESSIONS; i++)
    exprs[i] = 0;
  last_expr = 0;
}

#line 2157 "ldb.nw"
static void ldbI_setdisplay (int func_ref)
{
  int i;
  for (i=0; i<last_expr; i++)
    if (exprs[i] == 0)
    {
      exprs[i] = func_ref;
      return;
    }
  if (last_expr == MAX_EXPRESSIONS)
    T3000Debug_error ("ldb: too many expressions to display");
  exprs[last_expr] = func_ref;
  last_expr++;
}

#line 2179 "ldb.nw"
static void ldbI_unsetdisplay (int exp_number)
{
  if ((exp_number-1) < last_expr && exp_number > 0)
  {
    exprs[exp_number-1] = 0;
    if (last_expr == exp_number-1)
      while (exprs[last_expr-1] == 0)
        last_expr--;
  }
}

#line 2218 "ldb.nw"
static void ldbI_setGAction (char *expr)
{
  if (ldbI_existGAction ())
  {
    free (GActionString);
    T3000Debug_unref (GActionFunc);
  }
  if (expr)
  {
    GActionFunc = create_action (expr, "");
    GActionString = strdup (expr);
  }
  else
  {
    GActionString = NULL;
    GActionFunc = -1;
  }
}

#line 287 "ldb.nw"


#line 189 "ldb.nw"
static void update_location (int level, int line)
{
  T3000Debug_Object f = T3000Debug_stackedfunction (level);
  if (f == T3000Debug_NOOBJECT)		/* beware! Not tested properly */
  {
    curr_loc.file=NULL;
    curr_loc.line=0;
    return;
  }
  T3000Debug_funcinfo (f, &curr_loc.file, &curr_loc.line);
  if ((line != -1)  || ((line = T3000Debug_currentline (f)) != -1))
    curr_loc.line = line;
}

#line 375 "ldb.nw"
static void disable_hooks (void)
{
  if (T3000Debug_callhook)
  {
    T3000Debug_callhook = NULL;
    call_on = ON_HOOK;
  }
  else
    call_on = DONT_TURN_ON;
  if (T3000Debug_linehook)
  {
    T3000Debug_linehook = NULL;
    line_on = ON_HOOK;
  }
  else
    line_on = DONT_TURN_ON;
}

#line 402 "ldb.nw"
static void restore_hooks (void)
{
  if (call_on == ON_HOOK)
    T3000Debug_callhook = ldb_callhook;
  call_on = OFF_HOOK;
  if (line_on == ON_HOOK)
    T3000Debug_linehook = ldb_linehook;
  line_on = OFF_HOOK;
}

#line 424 "ldb.nw"
static void enable_linehook (void)
{
  if (line_on == OFF_HOOK)
    T3000Debug_linehook = ldb_linehook;
  else if (line_on == DONT_TURN_ON)
    line_on = ON_HOOK;
}

#line 437 "ldb.nw"
static void enable_callhook (void)
{
  if (call_on == OFF_HOOK)
    T3000Debug_callhook = ldb_callhook;
  else if (call_on == DONT_TURN_ON)
    call_on = ON_HOOK;
}

#line 458 "ldb.nw"
static void maybe_disable_hooks (void)
{
  if (!ldbI_existbreak() && !ldbI_existGAction() && IN_RUN_MODE)
  {
    T3000Debug_linehook = NULL;
    if (line_on == ON_HOOK)
      line_on = DONT_TURN_ON;
    T3000Debug_callhook = NULL;
    if (call_on == ON_HOOK)
      call_on = DONT_TURN_ON;
  }
}

#line 501 "ldb.nw"
static void verify_line_stops (void)
{
  execute_global_action ();
  check_current_location ();
  verify_line_counters ();
}

#line 516 "ldb.nw"
static void verify_line_counters (void)
{
  if (IN_STEP_MODE || (IN_NEXT_MODE && !func_level))
  {
    line_count--;
    if (line_count == 0)
    {
      mode = RUN_MODE;
      stop_now = 1;
    }
  }
}

#line 542 "ldb.nw"
static void execute_global_action (void)
{
  if (ldbI_existGAction ())
  {
    disable_hooks ();
    do_action (ldbI_GAction (), NULL);
    restore_hooks ();
  }
}

#line 572 "ldb.nw"
static void check_current_location (void)
{
  int code = ldbI_getcode (&curr_loc);
  if (code < 0)
    stop_now = 1;
  else if (code > 0)
  {
    disable_hooks ();
    local_env (0);
    do_action (code, NULL);
    global_env (0);
    restore_hooks ();
  }
}

#line 613 "ldb.nw"
static void call_procedure (char *file, int line)
{
  curr_loc.file = file;
  curr_loc.line = line;
  verify_call_stops ();
  if (stop_now)
    command_line ();
}

#line 634 "ldb.nw"
static void verify_call_stops (void)
{
  execute_global_action ();
  check_current_location ();
  verify_call_counters ();
}

#line 655 "ldb.nw"
static void verify_call_counters (void)
{
  if (IN_NEXT_MODE)
    func_level++;
  else if (IN_STEP_MODE)
  {
    line_count--;
    if (line_count == 0)
    {
      mode = RUN_MODE;
      stop_now = 1;
    }
  }
}

#line 683 "ldb.nw"
static void return_procedure (char *file, int line)
{
  verify_return_stops ();
  if (stop_now)
  {
    curr_loc.line = 0;
    curr_loc.file = "<return>";
    command_line ();
  }
  update_location (1, -1);
}

#line 704 "ldb.nw"
static void verify_return_stops (void)
{
  if (IN_NEXT_MODE)
  {
    if (--func_level < 0)
    {
      mode = RUN_MODE;
      stop_now = 1;
    }
  }
  else if (IN_STEP_MODE)
  {
    line_count--;
    if (line_count == 0)
    {
      mode = RUN_MODE;
      stop_now = 1;
    }
  }
}

#line 746 "ldb.nw"
static void show_expressions (void)
{
  int i;
  for (i=0; i<last_expr; i++)
  {
    T3000Debug_beginblock ();
      if (ldbI_exprfunc (i) != 0)
      {
        char s[255+1+7];
        T3000Debug_Object expr = T3000Debug_getref (ldbI_exprfunc(i));
        char *e = T3000Debug_getstring (expr);
        fprintf (stderr, "%d: %s = ", i+1, e);
        sprintf (s, "print(%255s)", e);
        T3000Debug_dostring (s);
      }
    T3000Debug_endblock ();
  }
}

#line 772 "ldb.nw"
static void command_line (void)
{
  char s[256];
  disable_hooks ();
  curr_level = 0;
  local_env (0);
  mode = RUN_MODE;
  show_expressions ();
  
#line 800 "ldb.nw"
if (curr_loc.line <= 0)
  fprintf (stderr, "<%s>\n", curr_loc.file);
else
  list_lines (curr_loc.file, curr_loc.line, 1);
fprintf (stderr, "ldb>");

#line 780 "ldb.nw"

  while (fgets (s, 256, stdin) != 0)
  {
    execute_command (s);
    if (!stop_now)
      break;
    fprintf (stderr, "ldb>");
  }
  global_env (curr_level);
  restore_hooks ();
}

#line 834 "ldb.nw"
static void execute_command (char *s)
{
  static char old_command = 0;
  /* avoid problems with a '\n' at the end of the string */
  if (s[strlen(s)-1] == '\n')
    s[strlen(s)-1] = '\0';

  if (s[0] == 0)
  {
    s[0] = '\'';
    s[1] = old_command;
  }
  if (s[0] != '\'')
    T3000Debug_dostring (s);
  else
  {
    if (s[1])
    {
      int action = shortcut[(int)s[1]];
      old_command = s[1];
      do_action (action, s+2);
    }
  }
}

#line 870 "ldb.nw"
static void setshortcut (int k, char *s)
{
  shortcut[k] = create_action (s, "line");
}

#line 879 "ldb.nw"
static void shortcut_init (void)
{
  int i;
  int action = create_action ("error('Shortcut not defined')", "");
  for (i=0; i < 256; i++)
    shortcut[i] = action;
}

#line 938 "ldb.nw"
static int local_env (int level)
{
  char       *name;
  T3000Debug_Object func = T3000Debug_stackedfunction (level);
  T3000Debug_Object table;
  T3000Debug_Object local;
  int        n = 1;
  if (func == T3000Debug_NOOBJECT)
    return 0;
  else if (T3000Debug_iscfunction (func))
    return -1;

  T3000Debug_beginblock ();
    table = T3000Debug_getref (global_ref);
    while ((local = T3000Debug_getlocal (func, n, &name)) != T3000Debug_NOOBJECT)
    {
      T3000Debug_Object value;
      T3000Debug_beginblock ();
        
#line 971 "ldb.nw"
value = T3000Debug_getglobal (name);
if (!T3000Debug_isnil (value))
{
  T3000Debug_pushobject (table);
  T3000Debug_pushstring (name);
  T3000Debug_pushobject (value);
  T3000Debug_settable ();
}

#line 956 "ldb.nw"

        
#line 985 "ldb.nw"
T3000Debug_pushobject (local);
T3000Debug_setglobal (name);

#line 957 "ldb.nw"

      T3000Debug_endblock ();
      n++;
    }
  T3000Debug_endblock ();
  return 1;
}

#line 1006 "ldb.nw"
static int global_env (int level)
{
  char       *name;
  T3000Debug_Object func = T3000Debug_stackedfunction (level);
  T3000Debug_Object table;
  int        n = 1;
  if (func == T3000Debug_NOOBJECT)
    return 0;
  else if (T3000Debug_iscfunction (func))
    return -1;

  T3000Debug_beginblock ();
    table = T3000Debug_getref (global_ref);
    while (T3000Debug_getlocal (func, n, &name) != T3000Debug_NOOBJECT)
    {
      T3000Debug_Object global;
      T3000Debug_beginblock ();
        
#line 1035 "ldb.nw"
T3000Debug_pushobject (T3000Debug_getglobal (name));
T3000Debug_setlocal (func, n);

#line 1023 "ldb.nw"

        
#line 1044 "ldb.nw"
/* global = global_ref[name] */
T3000Debug_pushobject (table);
T3000Debug_pushstring (name);
global = T3000Debug_gettable ();
/* name = global */
T3000Debug_pushobject (global);
T3000Debug_setglobal (name);
/* global_ref[name] = nil */
T3000Debug_pushobject (table);
T3000Debug_pushstring (name);
T3000Debug_pushnil ();
T3000Debug_settable ();

#line 1024 "ldb.nw"

      T3000Debug_endblock ();
      n++;
    }
  T3000Debug_endblock ();
  return 1;
}

#line 1150 "ldb.nw"
static int get_location (int p1, location *loc)
{
  T3000Debug_Object o1 = T3000Debug_getparam (p1);
  T3000Debug_Object o2 = T3000Debug_getparam (p1+1);
  if (T3000Debug_isfunction (o1))
  {
    T3000Debug_funcinfo (o1, &(loc->file), &(loc->line));
    return LDB_CALL_FUNCTION;
  }
  else if (T3000Debug_isnumber (o1))
  {
    loc->line = (int)T3000Debug_getnumber (o1);
    if (T3000Debug_isstring (o2))
      loc->file = T3000Debug_getstring (o2);
    else
      loc->file = curr_loc.file;
    return LDB_LINE_NUMBER;
  }
  else
    T3000Debug_error ("Invalid identification of source point");

  /* unreachable code. just to avoid warning messages */
  return LDB_CALL_FUNCTION;	
}

#line 1551 "ldb.nw"
static void enable_hooks (int is_function)
{
  if (is_function)
    enable_callhook ();
  else
    enable_linehook ();
}

#line 1691 "ldb.nw"
static void list_lines (char *file_name, int first_line, int num_lines)
{
  FILE *arq;
  int i;
  char buffer[TAM_BUF+1];
  if ((arq = fopen(file_name, "r")) == NULL)
    T3000Debug_error ("can't open file");
  for (i=1; i<first_line; i++)
    fgets (buffer, TAM_BUF, arq);
  for (i=0; i<num_lines; i++)
  {
    if (fgets (buffer, TAM_BUF, arq) == NULL)
      break;
    fprintf (stderr, "%d\t%s", first_line+i, buffer);
  }
  fclose (arq);
}

#line 1958 "ldb.nw"
static void ldb_error (void)
{
  T3000Debug_Object o = T3000Debug_getparam(1);
  if (T3000Debug_isstring (o))
    fprintf (stderr, "T3000Debug: %s\n", T3000Debug_getstring(o));
  else
    fprintf(stderr, "T3000Debug: unknown error\n");
  enable_linehook ();
  enable_callhook ();
  stop_now = 1;
}

#line 2266 "ldb.nw"
static void arg_error(char *funcname)
{
  char buff[100];
  sprintf (buff, "incorrect arguments to function `%s'", funcname);
  T3000Debug_error (buff);
}

#line 2279 "ldb.nw"
static char *check_string (int numArg, char *funcname)
{
  T3000Debug_Object o = T3000Debug_getparam(numArg);
  if (!T3000Debug_isstring(o))
    arg_error(funcname);
  return T3000Debug_getstring (o);
}

#line 2293 "ldb.nw"
static int check_number (int numArg, char *funcname)
{
  T3000Debug_Object o = T3000Debug_getparam(numArg);
  if (!T3000Debug_isnumber(o))
    arg_error(funcname);
  return (int)T3000Debug_getnumber(o);
}

#line 2306 "ldb.nw"
static int get_opt_number (int numArg, int def, char *funcname)
{
  return (T3000Debug_getparam(numArg) == T3000Debug_NOOBJECT) ? def :
                              (int)check_number(numArg, funcname);
}

#line 2391 "ldb.nw"
void load_config_file (void)
{
  char *config_file_name = getenv("LDB_CONFIG");
  if (config_file_name == NULL)
    config_file_name = "ldbrc.T3000Debug";
  if (T3000Debug_dofile(config_file_name) != 0)
  {
    char *home = getenv("HOME");
    config_file_name = malloc ((strlen(home)+11)*sizeof(char));
    strcpy (config_file_name, home);
    strcpy (config_file_name+strlen(home), "/ldbrc.T3000Debug");
    if (T3000Debug_dofile(config_file_name) != 0)
      fprintf (stderr, "Warning: T3000Debug debugger cannot load its configuration"
		" file (%s)\n", config_file_name);
    free (config_file_name);
  }
}

#line 288 "ldb.nw"


#line 1192 "ldb.nw"
static void ldb_break (void)
{
  location loc;
  if (get_location (1, &loc) == LDB_LINE_NUMBER)
    enable_linehook ();
  else
    enable_callhook ();
  ldbI_setbreak (&loc, -1);
}

#line 1213 "ldb.nw"
static void ldb_cont (void)
{
  stop_now = 0;
  maybe_disable_hooks ();
}

#line 1232 "ldb.nw"
static void ldb_delete (void)
{
  location loc;
  get_location (1, &loc);
  ldbI_delete (&loc);
  maybe_disable_hooks ();
}

#line 1259 "ldb.nw"
static void ldb_depth (void)
{
  int level = get_opt_number(1, curr_level, "depth");
  global_env (curr_level+2);
  if (local_env (level+2))
    curr_level = level;
  else
    local_env (curr_level+2);
  T3000Debug_pushnumber (curr_level);
}

#line 1288 "ldb.nw"
static void ldb_display (void)
{
  int value;
  char *s = check_string (1, "display");
  T3000Debug_pushstring (s);
  value = T3000Debug_ref (1);
  ldbI_setdisplay (value);
}

#line 1313 "ldb.nw"
static void ldb_doalways (void)
{
  T3000Debug_Object o = T3000Debug_getparam (1);
  if (o == T3000Debug_NOOBJECT)
    ldbI_setGAction (NULL);
  else
    ldbI_setGAction (check_string (1, "doalways"));
  enable_linehook ();
  enable_callhook ();
}

#line 1346 "ldb.nw"
static void ldb_doat (void)
{
  int value;
  location loc;
  if (get_location (2, &loc) == LDB_LINE_NUMBER)
    enable_linehook ();
  else
    enable_callhook ();
  value = create_action (check_string(1, "doat"), "");
  ldbI_setbreak (&loc, value);
}

#line 1378 "ldb.nw"
static void ldb_down (void)
{
  int level = get_opt_number(1, 1, "down");
  global_env (curr_level+2);
  if (local_env (curr_level+2+level))
    curr_level += level;
  else
    local_env (curr_level+2);
  enable_callhook ();
}

#line 1415 "ldb.nw"
static void ldb_dump (void)
{
  char       *name;
  int level = get_opt_number(1, 2, "dump");
  T3000Debug_Object func = T3000Debug_stackedfunction (level);
  T3000Debug_Object local;
  int        n = 1;
  if (func == T3000Debug_NOOBJECT)
    return;
  else if (T3000Debug_iscfunction (func))
    return;

  T3000Debug_beginblock ();
    
#line 1442 "ldb.nw"
while ((local = T3000Debug_getlocal (func, n, &name)) != T3000Debug_NOOBJECT)
{
  T3000Debug_beginblock ();
    fprintf (stderr, "%s = ", name);
    T3000Debug_pushobject (local);
    T3000Debug_callfunction (T3000Debug_getglobal ("print"));
  T3000Debug_endblock ();
  n++;
}

#line 1428 "ldb.nw"

  T3000Debug_endblock ();
}

#line 1459 "ldb.nw"
static void ldb_file (void)
{
  if (curr_loc.file)
    T3000Debug_pushstring (curr_loc.file);
  else
    T3000Debug_pushnil ();
}

#line 1487 "ldb.nw"
static void ldb_finish (void)
{
  line_count = -1;
  mode = NEXT_MODE;
  func_level = 0;
  stop_now = 0;
  enable_linehook ();
  enable_callhook ();
/*
  T3000Debug_pushnumber (-1);
  ldb_next ();
*/
}

#line 1514 "ldb.nw"
static void ldb_goto (void)
{
  int value;
  location loc;
  int is_function = (get_location (1, &loc) == LDB_CALL_FUNCTION);
  
#line 1533 "ldb.nw"
{
  char s[TAM_BUF];
  sprintf (s, "delete(%d,'%s') stop()", loc.line, loc.file);
  value = create_action (s, "");
}

#line 1519 "ldb.nw"

  ldbI_setbreak (&loc, value);
  stop_now = 0;
  enable_hooks (is_function);
}

#line 1574 "ldb.nw"
static void ldb_hooks (void)
{
  int i = 0;
  if (T3000Debug_linehook == ldb_linehook)
    i++;
  if (T3000Debug_callhook == ldb_callhook)
    i+=2;
  T3000Debug_pushnumber (i);
}

#line 1595 "ldb.nw"
static void ldb_line (void)
{
  T3000Debug_pushnumber (curr_loc.line);
}

#line 1615 "ldb.nw"
static void ldb_list (void)
{
  T3000Debug_Object o1 = T3000Debug_getparam (1);
  T3000Debug_Object o2 = T3000Debug_getparam (2);
  T3000Debug_Object o3 = T3000Debug_getparam (3);
  unsigned num_lines = 0;		/* to avoid warnings */
  int first_line;
  char *file_name;
  
#line 1644 "ldb.nw"
if (T3000Debug_isnumber(o1))

#line 1668 "ldb.nw"
{
  num_lines = T3000Debug_getnumber(o1);
  if (T3000Debug_isfunction(o2))
    T3000Debug_funcinfo (o2, &file_name, &first_line);  
  else if (T3000Debug_isstring (o2) && !T3000Debug_isnumber(o2))
  {
    first_line = 1;
    file_name = T3000Debug_getstring(o2);
  }
  else
  {
    first_line = T3000Debug_isnumber(o2) ? T3000Debug_getnumber(o2) :
	(curr_loc.line==0 ? 1 : curr_loc.line);
    file_name = T3000Debug_isstring(o3) ? T3000Debug_getstring (o3) : curr_loc.file;
  }
}

#line 1645 "ldb.nw"

else if (T3000Debug_isstring(o1))
{
  num_lines = -1;
  first_line = 1;
  file_name = T3000Debug_getstring(o1);
}
else
  T3000Debug_error ("first parameter must be a number");

#line 1623 "ldb.nw"

  list_lines(file_name, first_line, num_lines);
}

#line 1734 "ldb.nw"
static void ldb_next (void)
{
  line_count = get_opt_number (1, 1, "Next");
  mode = NEXT_MODE;
  func_level = 0;
  stop_now = 0;
  enable_linehook ();
  enable_callhook ();
}

#line 1759 "ldb.nw"
static void ldb_setshortcut (void)
{
  char *s1 = check_string(1, "setshortcut");
  char *s2 = check_string(2, "setshortcut");
  setshortcut(*s1, s2);
}

#line 1777 "ldb.nw"
static void ldb_show (void)
{
  int code, i;
  location loc;
  for (i=0; ldbI_getbreak (i, &loc, &code); i++)
    fprintf (stderr, "[%d:%s]\n", loc.line, loc.file);
  if (ldbI_existGAction ())
    fprintf (stderr, ">>%s<<\n", ldbI_GActionString ());
}

#line 1804 "ldb.nw"
static void ldb_step (void)
{
  line_count = get_opt_number (1, 1, "step");
  mode = STEP_MODE;
  stop_now = 0;
  enable_linehook ();
  enable_callhook ();
}

#line 1833 "ldb.nw"
static void ldb_stop (void)
{
  T3000Debug_Object o = T3000Debug_getparam (1);
  if (o != T3000Debug_NOOBJECT && T3000Debug_isnil(o))
    return;
  stop_now = 1;
  enable_linehook ();
  enable_callhook ();
}

#line 1854 "ldb.nw"
static void ldb_undisplay (void)
{
  ldbI_unsetdisplay (check_number (1, "undisplay"));
}

#line 1872 "ldb.nw"
static void ldb_up (void)
{
  int old_level = curr_level;
  int level = get_opt_number (1, 1, "up");
  global_env (curr_level+2);
  curr_level -= level;
  if (curr_level < 0)
    curr_level = 0;
  if (local_env (curr_level+2) == 0)
  {
    curr_level = old_level;
    local_env (curr_level);
  }
}

#line 1897 "ldb.nw"
static void ldb_where (void)
{
  int first_level = 0;
  T3000Debug_Object func;
  T3000Debug_Object o = T3000Debug_getparam (1);
  if (T3000Debug_isnumber (o))
    first_level = (int)T3000Debug_getnumber (o);
  fprintf(stderr, "Active Stack:\n");
  while ((func = T3000Debug_stackedfunction(first_level++)) != T3000Debug_NOOBJECT)
  {
    char *name;
    int currentline;
    char *filename;
    int linedefined;
    fprintf(stderr, "\t");
    T3000Debug_funcinfo(func, &filename, &linedefined);
    if (linedefined == 0)
      fprintf(stderr, "main of %s", filename);
    else
    {
      switch (*T3000Debug_getobjname(func, &name))
      {
        case 'g':
          fprintf(stderr, "function %s", name);
          break;
        case 't':
          fprintf(stderr, "`%s' tag-method", name);
          break;
        default:
          fprintf(stderr, "function defined at ");
      }
      if (linedefined < 0)
        fprintf(stderr, " (%s)", filename);
      else
        fprintf(stderr, " (%s:%d)", filename, linedefined);
    }
    if ((currentline = T3000Debug_currentline(func)) > 0)
      fprintf(stderr, " - at line %d", currentline);
    fprintf(stderr, "\n");
  }
}

#line 289 "ldb.nw"


#line 484 "ldb.nw"
static void ldb_linehook (int line)
{
  update_location (0, line);
  verify_line_stops ();
  if (stop_now)
    command_line ();
}

#line 598 "ldb.nw"
static void ldb_callhook (T3000Debug_Object func, char *file, int line)
{
  if (func == T3000Debug_NOOBJECT)
    return_procedure (file, line);
  else
    call_procedure (file, line);
}

#line 290 "ldb.nw"


#line 2324 "ldb.nw"

#line 2331 "ldb.nw"
static struct FuncList {
  char *name;
  T3000Debug_CFunction func;
} FuncList [] = {
                
#line 1204 "ldb.nw"
{"break", ldb_break},

#line 1221 "ldb.nw"
{"cont", ldb_cont},

#line 1245 "ldb.nw"
{"delete", ldb_delete},

#line 1272 "ldb.nw"
{"depth", ldb_depth},

#line 1299 "ldb.nw"
{"display", ldb_display},

#line 1326 "ldb.nw"
{"doalways", ldb_doalways},

#line 1360 "ldb.nw"
{"doat", ldb_doat},

#line 1391 "ldb.nw"
{"down", ldb_down},

#line 1434 "ldb.nw"
{"dump", ldb_dump},

#line 1469 "ldb.nw"
{"file", ldb_file},

#line 1503 "ldb.nw"
{"finish", ldb_finish},

#line 1561 "ldb.nw"
{"goto", ldb_goto},

#line 1586 "ldb.nw"
{"hooks", ldb_hooks},

#line 1602 "ldb.nw"
{"line", ldb_line},

#line 1716 "ldb.nw"
{"list", ldb_list},

#line 1750 "ldb.nw"
{"Next", ldb_next},

#line 1768 "ldb.nw"
{"setshortcut", ldb_setshortcut},

#line 1789 "ldb.nw"
{"show", ldb_show},

#line 1815 "ldb.nw"
{"step", ldb_step},

#line 1845 "ldb.nw"
{"stop", ldb_stop},

#line 1861 "ldb.nw"
{"undisplay", ldb_undisplay},

#line 1889 "ldb.nw"
{"up", ldb_up},

#line 1941 "ldb.nw"
{"where", ldb_where},

#line 2335 "ldb.nw"

                {"stack", ldb_where}
                };

#define SizeFuncList (sizeof(FuncList)/sizeof(struct FuncList))


#line 2324 "ldb.nw"


#line 2360 "ldb.nw"
void ldblib_open (void)
{
  int i;

  ldbI_initmarks ();
  ldbI_initexprs ();
  
#line 915 "ldb.nw"
T3000Debug_pushobject (T3000Debug_createtable ());
global_ref = T3000Debug_ref (1);

#line 2366 "ldb.nw"

  for (i = 0; i < SizeFuncList; i++)
    T3000Debug_register(FuncList[i].name, FuncList[i].func);
  shortcut_init ();
  load_config_file ();
  T3000Debug_pushcfunction (ldb_error);
  T3000Debug_setglobal ("error");
}


#line 2325 "ldb.nw"


#line 291 "ldb.nw"

