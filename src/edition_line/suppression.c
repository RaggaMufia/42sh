#include "42sh.h"

void	decal_pointeur(t_param *param)
{
  if (param->pos == 0)
    return ;
  param->pos -= 1;
  param->x -= 1;
  if (param->x < 0)
    {
      param->x = return_x() - 1;
      if (param->y > 0)
  	param->y -= 1;
    }
}

void	gere_delete(char *cmd, t_param *param, char *buff)
{
  int	size;
  int	indice;

  size = my_strlen(cmd);
  if (cmd == NULL || size == 0 || param->pos < 0 ||
      param->pos > size || (param->pos == 0 &&
			    (buff[0] == DEL && buff[1] == END)))
    return ;
  if ((buff[0] == DEL && buff[1] == END))
    indice = param->pos - 1;
  else
    indice = param->pos;
  while (indice + 1 < SIZE_BUFFER && cmd[(indice + 1) % SIZE_BUFFER] != '\0')
    {
      cmd[indice] = cmd[indice + 1];
      indice += 1;
    }
  cmd[indice % SIZE_BUFFER] = '\0';
  if ((buff[0] == DEL && buff[1] == END) && param->pos - 1 >= 0)
    decal_pointeur(param);
  view(cmd, param);
}
