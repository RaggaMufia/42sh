/*
** FILE for FILE in /home/moriss_h/projet
**
** Made by hugues morisset
** Login   <moriss_h@epitech.net>
**
** Started on  Mon Oct  8 09:34:29 2012 hugues morisset
** Last update Sat May 18 13:25:13 2013 fuck you RR
*/

#include "42sh.h"

void	recalc_prompt(t_sh *shell)
{
  char	*prompt;
  char	*ps1;
  char	*tmp;

  free(shell->param.str_prompt);
  prompt = NULL;
  if ((ps1 = get_envvar("PS1", shell->env)) != NULL)
    {
      tmp = my_strdup(ps1);
      if ((prompt = check_vars_in_str(tmp, shell->env)) != NULL)
        {
          if (prompt != tmp)
            free(tmp);
        }
    }
  if (prompt == NULL)
    prompt = my_strdup("$ ");
  shell->param.str_prompt = prompt;
}

void	user_loop(t_sh *shell)
{
  char	*lign;

  recalc_prompt(shell);
  while ((lign = read_cmd(NULL, &(shell->param), &shell->history)) != NULL)
    {
      no_fg_jobs_status(shell);
      call_signal_func(shell, 0);
      wait_no_fg_grp(shell);
      parse_user_cmd(shell, lign, 1);
      if (MEXIT)
        return ;
      wait_no_fg_grp(shell);
      recalc_prompt(shell);
    }
}
