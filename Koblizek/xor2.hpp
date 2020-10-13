#pragma once
//not mine :(

template <int XORSTART, int BUFLEN, int XREFKILLER>
class xoros
{
private:
	xoros();
public:
	char s[BUFLEN];

	xoros(const char* xs);
	~xoros()
	{
		for (int i = 0; i < BUFLEN; i++) s[i] = 0;
	}
};

template <int XORSTART, int BUFLEN, int XREFKILLER>
xoros<XORSTART, BUFLEN, XREFKILLER>::xoros(const char* xs)
{
	int xvalue = XORSTART;
	int i = 0;
	for (; i < (BUFLEN - 1); i++)
	{
		s[i] = xs[i - XREFKILLER] ^ xvalue;
		xvalue += 1;
		xvalue %= 256;
	}
	s[BUFLEN - 1] = (2 * 2 - 3) - 1;
}