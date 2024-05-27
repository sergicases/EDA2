#ifndef STATES_H
#define STATES_H

#include "commons.h"

#define MIN_STAT_VALUE 1

typedef enum {
    STAT_HEALTH_POINTS,
    STAT_MAGICAL_POINTS,
    STAT_PHYSICAL_ATTACK,
    STAT_PHYSICAL_DEFENSE,
    STAT_MAGICAL_ATTACK,
    STAT_MAGICAL_DEFENSE
} Stat;

Stat stat_from_json(const cJSON* json);
cJSON* stat_to_json(Stat stat);

typedef struct {
    int hp;
    int mp;
    int atk;
    int def;
    int m_atk;
    int m_def;
} Stats;

void stats_init(Stats* stats);

void stats_copy(Stats* dest, const Stats* source);

void stats_from_json(Stats* stats, const cJSON* json);
cJSON* stats_to_json(const Stats* stats);

int stats_get_hp(const Stats* stats);
int stats_get_mp(const Stats* stats);
int stats_get_atk(const Stats* stats);
int stats_get_def(const Stats* stats);
int stats_get_m_atk(const Stats* stats);
int stats_get_m_def(const Stats* stats);

void stats_set_hp(Stats* stats, int value);
void stats_set_mp(Stats* stats, int value);
void stats_set_atk(Stats* stats, int value);
void stats_set_def(Stats* stats, int value);
void stats_set_m_atk(Stats* stats, int value);
void stats_set_m_def(Stats* stats, int value);

int stats_get(const Stats* stats, Stat stat);
void stats_set(Stats* stats, Stat stat, int value);

const char* stats_name(Stat stat);


typedef struct {
    int atk;
    int def;
    int m_atk;
    int m_def;
} StatsModifications;

void stats_mod_init(StatsModifications* mods);

void stats_mod_copy(StatsModifications* dest, const StatsModifications* source);

bool is_modificable_stat(Stat stat);

int stats_mod_get_atk(const StatsModifications* mods);
int stats_mod_get_def(const StatsModifications* mods);
int stats_mod_get_m_atk(const StatsModifications* mods);
int stats_mod_get_m_def(const StatsModifications* mods);

void stats_mod_set_atk(StatsModifications* mods, int value);
void stats_mod_set_def(StatsModifications* mods, int value);
void stats_mod_set_m_atk(StatsModifications* mods, int value);
void stats_mod_set_m_def(StatsModifications* mods, int value);

void stats_mod_add_atk(StatsModifications* mods, int amount);
void stats_mod_add_def(StatsModifications* mods, int amount);
void stats_mod_add_m_atk(StatsModifications* mods, int amount);
void stats_mod_add_m_def(StatsModifications* mods, int amount);

int stats_mod_get(const StatsModifications* mods, Stat stat);
void stats_mod_set(StatsModifications* mods, Stat stat, int value);
void stats_mod_add(StatsModifications* mods, Stat stat, int amount);

int stats_get_with_powerups(const Stats* stats, const StatsModifications* mods, Stat stat);

#endif // STATES_H