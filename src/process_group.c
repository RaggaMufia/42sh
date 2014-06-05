#include "42sh.h"
#include "my_func.h"

/*
** Func that execute a group of command,
** It return -1 if not executable or if it has failed
*/

int	exec_process_group(t_sh *shell, t_grp *grp)
{
  int	tmp;

  if (grp == NULL)
    return (-1);
  if ((tmp = (is_grp_exec(shell, grp) - 1)) != -1)
    tmp = open_redirection(grp, shell);
  if (MEXIT)
    return (0);
  if (tmp != -1)
    if ((exec_a_pipe(shell, grp) == -1) || (MEXIT))
      return (-1);
  shell->process_group = (t_grp**)add_ptr_t_tab((void**)shell->process_group,
                         (void*)grp);
  if (GETFLAG(grp->flags, FLAGPOS(FGRP_FORGROUND)) && (grp->pid.pgid != -1))
    set_forground_process_g(shell, grp);
  else
    UNSETFLAG(grp->flags, FLAGPOS(FGRP_FORGROUND));
  return (0);
}

t_grp	*create_n_process_group(t_sh *shell, char *lign)
{
  char	**cmd_line;
  t_grp	*res;
  t_cmd	*tmp_cmd;
  int	i;

  i = 0;
  if ((res = malloc(1 * sizeof(t_grp))) == NULL)
    return (NULL);
  res->line = lign;
  init_stdfd_t_def_val(&(res->fd), 0, 1, 2);
  res->pid.sid = shell->pid.sid;
  res->pid.pgid = -1;
  res->cmds = NULL;
  res->flags = 0;
  parse_redirection(res, lign);
  if ((cmd_line = str_to_wordtab(res->line, "|", 1)) != NULL)
    while (cmd_line[i] != NULL)
      {
        tmp_cmd = create_n_cmd(shell, cmd_line[i]);
        res->cmds = (t_cmd**)add_ptr_t_tab((void**)res->cmds, (void*)tmp_cmd);
        i++;
      }
  free(cmd_line);
  return (res);
}

int	global_group_ret_status(t_grp *grp)
{
  int	i;
  int	glost;

  i = 0;
  glost = 0;
  if ((grp == NULL) || (grp->cmds == NULL))
    return (0);
  while (grp->cmds[i] != NULL)
    {
      glost += grp->cmds[i]->return_value;
      i++;
    }
  return (glost);
}

void	free_process_group(t_grp *grp)
{
  if (grp != NULL)
    {
      free(grp->line);
      free_ptr_tab((void**)grp->cmds, (void*)(&free_cmd));
      free_ptr_tab((void**)grp->redirection, &free);
      free(grp->transition_line);
      free(grp);
    }
}
