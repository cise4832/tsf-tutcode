﻿
#include "common.h"
#include "eucjis2004.h"
#include "parseskkdic.h"

LPCWSTR EntriesAri = L";; okuri-ari entries.\n";
LPCWSTR EntriesNasi = L";; okuri-nasi entries.\n";

int ReadSKKDicLine(FILE *fp, WCHAR bom, int &okuri, std::wstring &key,
	SKKDICCANDIDATES &c, SKKDICOKURIBLOCKS &o)
{
	CHAR buf[READBUFSIZE * sizeof(WCHAR)];
	std::string sbuf;
	WCHAR wbuf[READBUFSIZE];
	std::wstring wsbuf;
	size_t is;
	void *rp;
	std::wstring s, fmt;
	std::wregex re;

	c.clear();
	o.clear();

	switch(bom)
	{
	case BOM:
		while((rp = fgetws(wbuf, _countof(wbuf), fp)) != nullptr)
		{
			wsbuf += wbuf;

			if(!wsbuf.empty() && wsbuf.back() == L'\n')
			{
				break;
			}
		}
		break;
	default:
		while((rp = fgets(buf, _countof(buf), fp)) != nullptr)
		{
			sbuf += buf;

			if(!sbuf.empty() && sbuf.back() == '\n')
			{
				break;
			}
		}
		break;
	}

	if(rp == nullptr)
	{
		return -1;
	}

	switch(bom)
	{
	case BOM:
		break;
	default:
		wsbuf = eucjis2004_string_to_wstring(sbuf);
		if(wsbuf.empty())
		{
			return 1;
		}
		break;
	}

	if(wsbuf.empty())
	{
		return 1;
	}

	if(wsbuf.compare(EntriesAri) == 0)
	{
		okuri = 1;
		return 1;
	}
	else if(wsbuf.compare(EntriesNasi) == 0)
	{
		okuri = 0;
		return 1;
	}
	else
	{
		if(L'\0' <= wsbuf.front() && wsbuf.front() <= L'\x20')
		{
			return 1;
		}
	}

	if(okuri == -1) //header comment
	{
		if(wsbuf.front() != L';')
		{
			//;; okuri-ari/nasi行が無い、mazegaki.dicか、古いSKK辞書
			okuri = 0;
		}
		else
		{
			return 1;
		}
	}

	s = wsbuf;
	re.assign(L"[\\x00-\\x19]");
	fmt.assign(L"");
	s = std::regex_replace(s, re, fmt);

	if(okuri == 1)
	{
		//送りありエントリのブロック
		ParseSKKDicOkuriBlock(s, o);

		//送りありエントリのブロックを除去
		re.assign(L"\\[[^\\[\\]]+?/[^\\[\\]]+?/\\]/");
		fmt.assign(L"");
		s = std::regex_replace(s, re, fmt);
	}

	is = s.find_first_of(L'\x20');
	if(is == std::wstring::npos)
	{
		return 1;
	}
	key = s.substr(0, is);

	is = s.find_first_of(L'/', is);
	if(is == std::wstring::npos)
	{
		return 1;
	}
	s = s.substr(is);

	ParseSKKDicCandiate(s, c);

	return 0;
}

void ParseSKKDicCandiate(const std::wstring &s, SKKDICCANDIDATES &c)
{
	size_t i, is, ie, ia;
	std::wstring candidate, annotation;

	i = 0;
	while(i < s.size())
	{
		is = s.find_first_of(L'/', i);
		ie = s.find_first_of(L'/', is + 1);
		if(ie == std::wstring::npos)
		{
			break;
		}

		candidate = s.substr(i + 1, ie - is - 1);
		i = ie;

		ia = candidate.find_first_of(L';');

		if(ia == std::wstring::npos)
		{
			annotation.clear();
		}
		else
		{
			annotation = candidate.substr(ia + 1);
			candidate = candidate.substr(0, ia);
		}

		if(!candidate.empty())
		{
			c.push_back(SKKDICCANDIDATE(candidate, annotation));
		}
	}
}

void ParseSKKDicOkuriBlock(const std::wstring &s, SKKDICOKURIBLOCKS &o)
{
	std::wstring so, okurik, okuric;
	std::wregex re, reb;
	std::wsmatch m;
	SKKDICCANDIDATES okurics;

	re.assign(L"\\[[^\\[\\]]+?/[^\\[\\]]+?/\\]/");
	reb.assign(L"\\[([^\\[\\]]+?)(/[^\\[\\]]+?/)\\]/");
	so = s;

	while(std::regex_search(so, m, re))
	{
		okurics.clear();

		okurik = std::regex_replace(m.str(), reb, std::wstring(L"$1"));
		okuric = std::regex_replace(m.str(), reb, std::wstring(L"$2"));

		ParseSKKDicCandiate(okuric, okurics);

		std::reverse(okurics.begin(), okurics.end());

		o.insert(o.begin(), SKKDICOKURIBLOCK(okurik, okurics));
		so = m.suffix();
	}
}

std::wstring ParseConcat(const std::wstring &s)
{
	std::wstring ret, fmt, numstr, numtmpstr;
	std::wregex re;
	std::wsmatch res;
	wchar_t u;
	LPCWSTR bsrep = L"\uf05c";
	ret = s;

	re.assign(L"^\\(\\s*concat\\s+\".+\"\\s*\\)$");
	if(std::regex_search(ret, re))
	{
		re.assign(L"^\\(\\s*concat\\s+\"(.+)\"\\s*\\)$");
		fmt.assign(L"$1");
		ret = std::regex_replace(ret, re, fmt);
		re.assign(L"\"\\s+\"");
		fmt.assign(L"");
		ret = std::regex_replace(ret, re, fmt);
		//バックスラッシュ
		re.assign(L"\\\\\\\\");
		fmt.assign(bsrep);
		ret = std::regex_replace(ret, re, fmt);
		//二重引用符
		re.assign(L"\\\\\\\"");
		fmt.assign(L"\\\"");
		ret = std::regex_replace(ret, re, fmt);
		//空白文字
		re.assign(L"\\\\s");
		fmt.assign(L"\x20");
		ret = std::regex_replace(ret, re, fmt);
		//制御文字など
		re.assign(L"\\\\[abtnvfred ]");
		fmt.assign(L"");
		ret = std::regex_replace(ret, re, fmt);
		//8進数表記の文字
		re.assign(L"\\\\[0-3][0-7]{2}");
		while(std::regex_search(ret, res, re))
		{
			numstr += res.prefix();
			numtmpstr = res.str();
			numtmpstr[0] = L'0';
			u = (wchar_t)wcstoul(numtmpstr.c_str(), nullptr, 0);
			if(u >= L'\x20' && u <= L'\x7E')
			{
				numstr.append(1, u);
			}
			ret = res.suffix();
		}
		ret = numstr + ret;
		//意味なしエスケープ
		re.assign(L"\\\\");
		fmt.assign(L"");
		ret = std::regex_replace(ret, re, fmt);
		//バックスラッシュ
		re.assign(bsrep);
		fmt.assign(L"\\");
		ret = std::regex_replace(ret, re, fmt);
	}

	return ret;
}

std::wstring MakeConcat(const std::wstring &s)
{
	std::wstring ret, fmt;
	std::wregex re;

	ret = s;

	// "/" -> \057, ";" -> \073
	re.assign(L"[/;]");
	if(std::regex_search(ret, re))
	{
		// "\"" -> "\\\"", "\\" -> "\\\\"
		re.assign(L"([\\\"\\\\])");
		fmt.assign(L"\\$1");
		ret = std::regex_replace(ret, re, fmt);

		re.assign(L"/");
		fmt.assign(L"\\057");
		ret = std::regex_replace(ret, re, fmt);

		re.assign(L";");
		fmt.assign(L"\\073");
		ret = std::regex_replace(ret, re, fmt);

		ret = L"(concat \"" + ret + L"\")";
	}

	return ret;
}
