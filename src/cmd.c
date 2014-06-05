#include "42sh.h"

t_cmd	*cmd_f_pid(int pid, t_sh *shell)
{
  int	i;
  int	j;
  t_grp	*tmpgrp;
  t_cmd	*tmpcmd;

  i = 0;
  if ((shell == NULL) || (shell->process_group == NULL) || (pid == -1))
    return (NULL);
  while ((tmpgrp = shell->process_group[i]) != NULL)
    {
      if (tmpgrp->cmds != NULL)
        {
          j = 0;
          while ((tmpcmd = tmpgrp->cmds[j]) != NULL)
            {
              if (tmpcmd->pid.pid == pid)
                return (tmpcmd);
              j++;
            }
        }
      i++;
    }
  return (NULL);
}

t_cmd	*create_n_cmd(t_sh *shell, char *lign)
{
  t_cmd	*res;

  if ((res = malloc(1 * sizeof(t_cmd))) == NULL)
    return (NULL);
  res->line = lign;
  res->pid.pid = -1;
  res->pid.pgid = -1;
  res->return_value = 0;
  tr_str(res->line, '\t', ' ');
  res->argv = str_to_wordtab(res->line, " ", 2);
  rm_empty_str_f_tab(res->argv);
  alias_multiple_replace(&(res->argv), shell->alias_tab);
  replace_var_in_argv(res->argv, shell->env);
  expand_reg_epx_on_argv(&(res->argv));
  res->cmd_fpath = exec_full_path(res->argv[0], shell->path);
  return (res);
}

void	free_cmd(t_cmd *cmd)
{
  if (cmd == NULL)
    return ;
  free(cmd->line);
  free_ptr_tab((void**)cmd->argv, &free);
  free(cmd->cmd_fpath);
  free(cmd);
}
