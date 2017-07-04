#include "stringtools.h"

// cpp字符串切割
vector<string> split(string &str, const char *c)
{
	char *cstr, *p;
	vector<string> res;
	cstr = new char[str.size() + 1];
	strcpy(cstr, str.c_str());
	p = strtok(cstr, c);
	while (p != NULL)
	{
		res.push_back((basic_string<char, char_traits<char>, allocator<char>> &&) p);
		p = strtok(NULL, c);
	}
	return res;
}
