#include "stats.h"
#include "json_utils.h"

#define STAT_NAME_HEALTH_POINTS "hp"
#define STAT_NAME_MAGICAL_POINTS "mp"
#define STAT_NAME_PHYSICAL_ATTACK "atk"
#define STAT_NAME_PHYSICAL_DEFENSE "def"
#define STAT_NAME_MAGICAL_ATTACK "m_atk"
#define STAT_NAME_MAGICAL_DEFENSE "m_def"

Stat stat_from_json(const cJSON* json)
{
    if (json == NULL)
        return STAT_HEALTH_POINTS;

    if (cJSON_IsNumber(json))
    {
        switch (json->valueint)
        {
            case STAT_HEALTH_POINTS:
            case STAT_MAGICAL_POINTS:
            case STAT_PHYSICAL_ATTACK:
            case STAT_PHYSICAL_DEFENSE:
            case STAT_MAGICAL_ATTACK:
            case STAT_MAGICAL_DEFENSE:
                return json->valueint;
        }
    }
    else if (cJSON_IsString(json))
    {
        if (strcmp(json->valuestring, STAT_NAME_HEALTH_POINTS) == 0) return STAT_HEALTH_POINTS;
        if (strcmp(json->valuestring, STAT_NAME_MAGICAL_POINTS) == 0) return STAT_MAGICAL_POINTS;
        if (strcmp(json->valuestring, STAT_NAME_PHYSICAL_ATTACK) == 0) return STAT_PHYSICAL_ATTACK;
        if (strcmp(json->valuestring, STAT_NAME_PHYSICAL_DEFENSE) == 0) return STAT_PHYSICAL_DEFENSE;
        if (strcmp(json->valuestring, STAT_NAME_MAGICAL_ATTACK) == 0) return STAT_MAGICAL_ATTACK;
        if (strcmp(json->valuestring, STAT_NAME_MAGICAL_DEFENSE) == 0) return STAT_MAGICAL_DEFENSE;
    }

    return STAT_HEALTH_POINTS;
}

cJSON* stat_to_json(Stat stat)
{
    switch (stat)
    {
        case STAT_HEALTH_POINTS: return cJSON_CreateString(STAT_NAME_HEALTH_POINTS);
        case STAT_MAGICAL_POINTS: return cJSON_CreateString(STAT_NAME_MAGICAL_POINTS);
        case STAT_PHYSICAL_ATTACK: return cJSON_CreateString(STAT_NAME_PHYSICAL_ATTACK);
        case STAT_PHYSICAL_DEFENSE: return cJSON_CreateString(STAT_NAME_PHYSICAL_DEFENSE);
        case STAT_MAGICAL_ATTACK: return cJSON_CreateString(STAT_NAME_PHYSICAL_DEFENSE);
        case STAT_MAGICAL_DEFENSE: return cJSON_CreateString(STAT_NAME_MAGICAL_DEFENSE);

        default:
            return cJSON_CreateNumber((double)stat);
    }
}




void stats_init(Stats* stats)
{
    stats->hp = MIN_STAT_VALUE;
    stats->mp = MIN_STAT_VALUE;
    stats->atk = MIN_STAT_VALUE;
    stats->def = MIN_STAT_VALUE;
    stats->m_atk = MIN_STAT_VALUE;
    stats->m_def = MIN_STAT_VALUE;
}

void stats_copy(Stats* dest, const Stats* source)
{
    dest->hp = source->hp;
    dest->mp = source->mp;
    dest->atk = source->atk;
    dest->def = source->def;
    dest->m_atk = source->m_atk;
    dest->m_def = source->m_def;
}

void stats_from_json(Stats* stats, const cJSON* json)
{
    if (json == NULL || !cJSON_IsObject(json))
    {
        stats_init(stats);
        return;
    }

    stats_set_hp(stats, json_object_get_int(json, "hp", 0));
    stats_set_mp(stats, json_object_get_int(json, "mp", 0));
    stats_set_atk(stats, json_object_get_int(json, "atk", 0));
    stats_set_def(stats, json_object_get_int(json, "def", 0));
    stats_set_m_atk(stats, json_object_get_int(json, "m_atk", 0));
    stats_set_m_def(stats, json_object_get_int(json, "m_def", 0));
}

cJSON* stats_to_json(const Stats* stats)
{
    cJSON* json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "hp", stats_get_hp(stats));
    cJSON_AddNumberToObject(json, "mp", stats_get_mp(stats));
    cJSON_AddNumberToObject(json, "atk", stats_get_atk(stats));
    cJSON_AddNumberToObject(json, "def", stats_get_def(stats));
    cJSON_AddNumberToObject(json, "m_atk", stats_get_m_atk(stats));
    cJSON_AddNumberToObject(json, "m_def", stats_get_m_def(stats));
    return json;
}

int stats_get_hp(const Stats* stats) { return stats->hp; }
int stats_get_mp(const Stats* stats) { return stats->mp; }
int stats_get_atk(const Stats* stats) { return stats->atk; }
int stats_get_def(const Stats* stats) { return stats->def; }
int stats_get_m_atk(const Stats* stats) { return stats->m_atk; }
int stats_get_m_def(const Stats* stats) { return stats->m_def; }

void stats_set_hp(Stats* stats, int value) { stats->hp = max(MIN_STAT_VALUE, value); }
void stats_set_mp(Stats* stats, int value){ stats->mp = max(MIN_STAT_VALUE, value); }
void stats_set_atk(Stats* stats, int value){ stats->atk = max(MIN_STAT_VALUE, value); }
void stats_set_def(Stats* stats, int value){ stats->def = max(MIN_STAT_VALUE, value); }
void stats_set_m_atk(Stats* stats, int value){ stats->m_atk = max(MIN_STAT_VALUE, value); }
void stats_set_m_def(Stats* stats, int value){ stats->m_def = max(MIN_STAT_VALUE, value); }

int stats_get(const Stats* stats, Stat stat)
{
    switch(stat)
    {
        case STAT_HEALTH_POINTS: return stats_get_hp(stats);
        case STAT_MAGICAL_POINTS: return stats_get_mp(stats);
        case STAT_PHYSICAL_ATTACK: return stats_get_atk(stats);
        case STAT_PHYSICAL_DEFENSE: return stats_get_def(stats);
        case STAT_MAGICAL_ATTACK: return stats_get_m_atk(stats);
        case STAT_MAGICAL_DEFENSE: return stats_get_m_def(stats);
    }

    printf("[ERROR]: Invalid Stat Type\n");
    return 0;
}

void stats_set(Stats* stats, Stat stat, int value)
{
    switch (stat)
    {
        case STAT_HEALTH_POINTS: stats_set_hp(stats, value); break;
        case STAT_MAGICAL_POINTS: stats_set_mp(stats, value); break;
        case STAT_PHYSICAL_ATTACK: stats_set_atk(stats, value); break;
        case STAT_PHYSICAL_DEFENSE: stats_set_def(stats, value); break;
        case STAT_MAGICAL_ATTACK: stats_set_m_atk(stats, value); break;
        case STAT_MAGICAL_DEFENSE: stats_set_m_def(stats, value); break;
        default: printf("[ERROR]: Invalid Stat Type\n"); break;
    }
}

const char* stats_name(Stat stat)
{
    switch (stat)
    {
        case STAT_HEALTH_POINTS: return "Salud";
        case STAT_MAGICAL_POINTS: return "Magia";
        case STAT_PHYSICAL_ATTACK: return "Ataque f�sico";
        case STAT_PHYSICAL_DEFENSE: return "Defensa f�sica";
        case STAT_MAGICAL_ATTACK: return "Ataque m�gico";
        case STAT_MAGICAL_DEFENSE: return "Defensa m�gica";
    }
    printf("[ERROR]: Invalid Stat Type\n");
    return "<Stat undefined>";
}



void stats_mod_init(StatsModifications* mods)
{
    mods->atk = 0;
    mods->def = 0;
    mods->m_atk = 0;
    mods->m_def = 0;
}

void stats_mod_copy(StatsModifications* dest, const StatsModifications* source)
{
    dest->atk = source->atk;
    dest->def = source->def;
    dest->m_atk = source->m_atk;
    dest->m_def = source->m_def;
}

bool is_modificable_stat(Stat stat)
{
    return	stat == STAT_PHYSICAL_ATTACK ||
        stat == STAT_PHYSICAL_DEFENSE ||
        stat == STAT_MAGICAL_ATTACK ||
        stat == STAT_MAGICAL_DEFENSE;
}

int stats_mod_get_atk(const StatsModifications* mods) { return mods->atk; }
int stats_mod_get_def(const StatsModifications* mods) { return mods->def; }
int stats_mod_get_m_atk(const StatsModifications* mods) { return mods->m_atk; }
int stats_mod_get_m_def(const StatsModifications* mods) { return mods->m_def; }

void stats_mod_set_atk(StatsModifications* mods, int value) { mods->atk = value; }
void stats_mod_set_def(StatsModifications* mods, int value) { mods->def = value; }
void stats_mod_set_m_atk(StatsModifications* mods, int value) { mods->m_atk = value; }
void stats_mod_set_m_def(StatsModifications* mods, int value) { mods->m_def = value; }

void stats_mod_add_atk(StatsModifications* mods, int amount) { stats_mod_set_atk(mods, stats_mod_get_atk(mods) + amount); }
void stats_mod_add_def(StatsModifications* mods, int amount) { stats_mod_set_def(mods, stats_mod_get_def(mods) + amount); }
void stats_mod_add_m_atk(StatsModifications* mods, int amount) { stats_mod_set_m_atk(mods, stats_mod_get_m_atk(mods) + amount); }
void stats_mod_add_m_def(StatsModifications* mods, int amount) { stats_mod_set_m_def(mods, stats_mod_get_m_def(mods) + amount); }

int stats_mod_get(const StatsModifications* mods, Stat stat)
{
    switch (stat)
    {
        case STAT_PHYSICAL_ATTACK: return stats_mod_get_atk(mods);
        case STAT_PHYSICAL_DEFENSE: return stats_mod_get_def(mods);
        case STAT_MAGICAL_ATTACK: return stats_mod_get_m_atk(mods);
        case STAT_MAGICAL_DEFENSE: return stats_mod_get_m_def(mods);
        default:
            printf("[ERROR]: Invalid Modificable Stat Type\n");
            return 0;
    }
}

void stats_mod_set(StatsModifications* mods, Stat stat, int value)
{
    switch (stat)
    {
        case STAT_PHYSICAL_ATTACK: stats_mod_set_atk(mods, value); break;
        case STAT_PHYSICAL_DEFENSE: stats_mod_set_def(mods, value); break;
        case STAT_MAGICAL_ATTACK: stats_mod_set_m_atk(mods, value); break;
        case STAT_MAGICAL_DEFENSE: stats_mod_set_m_def(mods, value); break;
        default: printf("[ERROR]: Invalid Modificable Stat Type\n"); break;
    }
}

void stats_mod_add(StatsModifications* mods, Stat stat, int amount)
{
    switch (stat)
    {
        case STAT_PHYSICAL_ATTACK: stats_mod_add_atk(mods, amount); break;
        case STAT_PHYSICAL_DEFENSE: stats_mod_add_def(mods, amount); break;
        case STAT_MAGICAL_ATTACK: stats_mod_add_m_atk(mods, amount); break;
        case STAT_MAGICAL_DEFENSE: stats_mod_add_m_def(mods, amount); break;
        default: printf("[ERROR]: Invalid Modificable Stat Type\n"); break;
    }
}

int stats_get_with_powerups(const Stats* stats, const StatsModifications* mods, Stat stat)
{
    int value = stats_get(stats, stat);
    if(is_modificable_stat(stat))
        value += stats_mod_get(mods, stat);

    return max(MIN_STAT_VALUE, value);
}
