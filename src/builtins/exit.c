#include "42sh.h"

void	builtin_exit(t_cmd *cmd, UNSEDP t_fds *fd, t_sh *shell)
{
  if (cmd->argv[1] != NULL)
    shell->beepbeepexit = my_getnbr(cmd->argv[1]);
  else
    shell->beepbeepexit = 0;
  SETFLAG(shell->beepbeepexit, FLAGPOS(EXIT_F_POS));
  UNSETFLAG(shell->beepbeepexit, FLAGPOS(EXIT_FORK));
}
