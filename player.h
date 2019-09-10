#pragma once
class player
{
private:
	bool m_IsFast;
	bool m_IsComputer;
	char* m_name;
public:
	player(bool IsFast, bool IsComputer, char* name);
	bool get_IsFast();
	bool get_IsComputer();
	char* get_name();
	virtual ~player(void);
};

