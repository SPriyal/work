#ifndef _MIS_ENGLISHDICT_H_
#define _MIS_ENGLISHDICT_H_

#include "dict.h"
#include "string_hash.h"

class MISEnglishDict : MISDict {
	public:
		MISEnglishDict(int id);
		~MISEnglishDict();
	
	public:
		void addPattern(const string& pattern);
		void removePattern(const string& pattern);
		int match(const string& text, MISMatchedResult** result = NULL);

	protected:
		string_hash_set m_patterns;
		size_t m_min_length;
		size_t m_max_length;
};
#endif
