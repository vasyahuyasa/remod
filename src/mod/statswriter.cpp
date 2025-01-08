/*
 * remod:    statswriter.cpp
 * date:     2025
 * author:   degrave
 *
 * JSON stats writer
 */

#include "statswriter.h"
#include "remod.h"
#include "fpsgame.h"

#define STATSFILE_FORMAT_VERSION 1

EXTENSION(STATSWRITER);

namespace remod
{
  namespace statswriter
  {

    void write_game(stream *statsfile, gameinfo game)
    {
      statsfile->printf("\"game\":{");
      statsfile->printf("  \"mode\": %d,", game.mode);
      statsfile->printf("  \"map\": \"%s\"}", game.mapname);
    }

    void write_teams(stream *statsfile, vector<teaminfo> teams)
    {
      statsfile->printf("\"teams\":[");
      loopv(teams)
      {
        if (i != 0)
        {
          statsfile->printf(",");
        }
        statsfile->printf("{\"team\": \"%s\",", teams[i].team);
        statsfile->printf(" \"score\": \"%d\"}", teams[i].score);
      }
      statsfile->printf("]");
    }

    void write_players(stream *statsfile, vector<playerinfo> players)
    {
      statsfile->printf("\"players\":[");
      loopv(players)
      {
        if (i != 0)
        {
          statsfile->printf(",");
        }
        statsfile->printf("{\"name\": \"%s\",", players[i].name);
        statsfile->printf(" \"authname\": \"%s\",", players[i].authname);
        statsfile->printf(" \"connect_seconds\": %d,", players[i].connect_seconds);
        statsfile->printf(" \"team\": \"%s\",", players[i].team);
        statsfile->printf(" \"privilege\": %d,", players[i].privilege);
        statsfile->printf(" \"frags\": %d,", players[i].frags);
        statsfile->printf(" \"flags\": %d,", players[i].flags);
        statsfile->printf(" \"deaths\": %d,", players[i].deaths);
        statsfile->printf(" \"teamkills\": %d,", players[i].teamkills);
        statsfile->printf(" \"shotdamage\": %d,", players[i].shotdamage);
        statsfile->printf(" \"damage\": %d", players[i].damage);
        statsfile->printf(" \"effectiveness\": %f,", players[i].effectiveness);
        statsfile->printf(" \"suicides\": %d,", players[i].suicides);
        statsfile->printf(" \"guninfo\": [", players[i].suicides);
        loopj(NUMGUNS)
        {
          if (i != 0)
          {
            statsfile->printf(",");
          }
          statsfile->printf("{ \"gun\": %d,", j);
          statsfile->printf("  \"shotdamage\": %d,", players[i].guninfo[j].shotdamage);
          statsfile->printf("  \"damage\": %d}", players[i].guninfo[j].damage);
        }
        statsfile->printf("  ]}");
      }
      statsfile->printf("]");
    }

    void writeto(const char *path, stats stats)
    {
      stream *statsfile = openfile(path, "wb");
      if (!statsfile)
      {
        conoutf("could not write stats to \"%s\"", path);
        return;
      }

      statsfile->printf("{ \"version\": %d,", STATSFILE_FORMAT_VERSION);
      write_game(statsfile, stats.game);
      statsfile->printf(",");
      write_teams(statsfile, stats.teams);
      statsfile->printf(",");
      write_players(statsfile, stats.players);
      statsfile->printf("}");
    };

    void write(const char *path)
    {
      vector<playerinfo> players;
      vector<teaminfo> teams;

      loopv(server::clients)
      {
      }

      struct stats stats = {
        players : {

        },
        teams : {

        },
        game : {
          mode : server::gamemode,
          mapname : newstring(server::smapname),
        },
      };
    }
  } // namespace statswriter
} // namespace remod
