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
        statsfile->printf(" \"guninfo\": [");
        loopj(NUMGUNS)
        {
          if (i != 0)
          {
            statsfile->printf(",");
          }
          statsfile->printf("{ \"gun\": %d,", j);
          statsfile->printf("  \"shotdamage\": %d,", players[i].gi[j].shotdamage);
          statsfile->printf("  \"damage\": %d}", players[i].gi[j].damage);
        }
        statsfile->printf("  ]}");
      }
      statsfile->printf("]");
    }

    void writeto(stream *statsfile, stats stats)
    {
      statsfile->printf("{ \"version\": %d,", STATSFILE_FORMAT_VERSION);
      write_game(statsfile, stats.game);
      statsfile->printf(",");
      write_teams(statsfile, stats.teams);
      statsfile->printf(",");
      write_players(statsfile, stats.players);
      statsfile->printf("}");
    };

    SVARP(statsdir, "statsdir");

    void write(const char *path)
    {
      static string s;
      formatstring(s, "%s/%s", statsdir, path);

      stream *statsfile = openfile(s, "wb");
      if (!statsfile)
      {
        conoutf("could not write stats to \"%s\"", s);
        return;
      }

      vector<playerinfo> players;

      loopv(server::clients)
      {
        playerinfo player = {
            .name = server::clients[i]->name,
            .authname = server::clients[i]->authname,
            .connect_seconds = (unsigned int) server::clients[i]->connectmillis / 1000,
            .team = server::clients[i]->team,
            .privilege = server::clients[i]->privilege,
            .frags = server::clients[i]->state.frags,
            .flags = server::clients[i]->state.flags,
            .deaths = server::clients[i]->state.deaths,
            .teamkills = server::clients[i]->state.teamkills,
            .shotdamage = server::clients[i]->state.shotdamage,
            .damage = server::clients[i]->state.damage,
            .effectiveness = server::clients[i]->state.effectiveness,
            .suicides = server::clients[i]->state.ext.suicides};

        loopj(NUMGUNS)
        {
          player.gi[j].shotdamage = server::clients[i]->state.ext.guninfo[j].shotdamage;
          player.gi[j].damage = server::clients[i]->state.ext.guninfo[j].damage;
        }

        players.add(player);
      }

      vector<teamscore> scores;

      server::smode->getteamscores(scores);

      vector<teaminfo> teams;
      loopv(scores)
      {
        teaminfo teaminfo = {
            .team = newstring(scores[i].team),
            .score = scores[i].score,
        };
        teams.add(teaminfo);
      }

      stats stats = {
          .players = players,
          .teams = teams,
          .game = {
              .mode = server::gamemode,
              .mapname = newstring(server::smapname)},
      };

      writeto(statsfile, stats);
    }

    COMMANDN(writestats, write, "s");
  } // namespace statswriter
} // namespace remod
