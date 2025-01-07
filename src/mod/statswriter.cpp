/*
 * remod:    statswriter.cpp
 * date:     2025
 * author:   degrave
 *
 * JSON stats writer
 */

#include "remod.h"
#include "tools.h"

EXTENSION(STATSWRITER);

namespace remod {
namespace statswriter {
struct playerinfo {
  const char *name;
  const char *authname;
  unsigned int connect_seconds;
  const char *team;
  int privilege;
  int frags, flags, deaths, teamkills, shotdamage, damage;
  float effectiveness;
  int suicides;
  struct {
    int shotdamage;
    int damage;
  } guninfo[NUMGUNS];
};
struct teaminfo {
  const char *team;
  int score;
};
struct gameinfo {
  int mode;
  const char *mapname;
};
struct stats {
  vector<playerinfo> players;
  vector<teaminfo> teams;
  struct gameinfo game;
};

void write_game(stream *statsfile, gameinfo game) {
  statsfile->printf("\"game\":{\"mode\": %d, \"map\": \"%s\"}", game.mode,
                    game.mapname);
}

void write_teams(stream *statsfile, gameinfo game) {
  // TODO
}

void write_players(stream *statsfile, gameinfo game) {
  // TODO
}

void write(const char *path, stats stats) {
  stream *statsfile = openfile(path, "wb");
  if (!statsfile) {
    conoutf("could not write stats to \"%s\"", path);
    return;
  }

  statsfile->printf("{");
  write_game(statsfile, stats.game);
  statsfile->printf("}");
};

} // namespace statswriter
} // namespace remod
