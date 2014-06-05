#include "42sh.h"

void	builtin_history(t_cmd *cmd, t_fds *fd, t_sh *shell)
{
  if (cmd->argv[1] == NULL)
    {
      exec_process(cmd, fd, shell,  &view_history);
      return ;
    }
  if (cmd->argv[1] != NULL && str_cmp(cmd->argv[1], "-c") == 1)
    {
      clear_history((shell->history));
      shell->history = NULL;
      return ;
    }
  if (cmd->argv[1] != NULL && str_cmp(cmd->argv[1], "-d") == 1 &&
      cmd->argv[2] != NULL)
    {
      rm_history_d(&(shell->history), my_getnbr(cmd->argv[2]));
      return ;
    }
  my_putstr("history: usage: history [-c] [-d offset]\n", 1, -1);
}
