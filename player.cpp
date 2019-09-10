#include "player.h"
player::player(bool IsFast, bool IsComputer, char* name):m_IsFast(IsFast),m_IsComputer(IsComputer),m_name(name)
{
}

player::~player(void)
{
}
bool player::get_IsFast()
{
	return m_IsFast;
}
bool player::get_IsComputer()
{
	return m_IsComputer;
}
char* player::get_name()
{
	return m_name;
}