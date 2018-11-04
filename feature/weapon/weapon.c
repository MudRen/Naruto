// ���� weapon.c

int is_weapon() { return 1; }

varargs void init_damage(int, int, int, int, string);

void setup_weapon(int lower, int upper, int bonus, int level)
{
    if( lower < 1 || upper < 1 || upper > 200 ) error("�����������������趨����\n");
    if( lower > upper ) error("�����������������趨����\n");
    if( bonus < 1 || bonus > 200 ) error("�����������������趨����\n");
    if( level < 1 || level > 10 ) error("�����������������趨����\n");
    	
    init_damage(lower, upper, bonus, level, "righthand");
    init_damage(lower, upper, bonus/2, level, "lefthand");
    init_damage(lower, upper, bonus*3/2, level, "twohanded");
}