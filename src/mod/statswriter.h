#ifndef __STATSWRITER_H__
#define __STATSWRITER_H__

#include "game.h"

namespace remod
{
  namespace statswriter
  {
    struct playerinfo
    {
      const char *name;
      const char *authname;
      unsigned int connect_seconds;
      const char *team;
      int privilege;
      int frags, flags, deaths, teamkills, shotdamage, damage;
      float effectiveness;
      int suicides;
      struct
      {
        int shotdamage;
        int damage;
      } guninfo[NUMGUNS];
    };
    struct teaminfo
    {
      const char *team;
      int score;
    };
    struct gameinfo
    {
      int mode;
      const char *mapname;
    };
    struct stats
    {
      vector<playerinfo> players;
      vector<teaminfo> teams;
      struct gameinfo game;
    };
  } // namespace statswriter
} // namespace remod
#endif