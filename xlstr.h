// READY FOR UPLOAD, XLSTR.H BETA VERSION.

#pragma once

#include <vector>
#include <cctype>
#include <cstring>
#include <cstdlib>



/*

<[ xlstr ]>

< General >
# Represents a C-str wrapper that holds a C-style str as its content.
# Generally, an xlstr is immutable once instantiated.
# Methods associated with xlstr implement C functions such as malloc, realloc and free,
and almost always returns a new xlstr.
# Only the operators += and *= modifies the xlstr itself, as a way to speed up the operation.
# The xlstr class implements the most common str methods in Javascript, whereas operator overloads allows user to write codes similar to Python. The class and its methods are meant provide an extra level of abstraction that hides the details of memory allocations and deallocations from the user.

< Communication betwen str and substr arrays >
# The split and zip methods mediate the interconversion between str and substr arrays.
# These methods are mediated by a dedicated xl_str_collection class, which inherits std::vector<xl_str>.

< Comments on efficiency >
# This header is written initially for personal practice to reinvent the wheel in C++.
# During testing, most methods are not as efficient as the native c++ str class, although the latter does not support split and zip operations or other methods in newer programming languages.
# Compared with an alternative implementation that stores str contents via vector<char> instead of C-style str, vector<char> usually yields better efficiency than the current implementation especially for methods such as concat and repeat that produce a longer xlstr, likely because vector<char> has optimizations that reserve some spare memory for element insertion which avoids expensive memory reallocations at each call, whereas the current implementation calls realloc each time an xlstr is extended. Again, this is partly for practice purposes.

< Address of xlstr and address of its contents >
# In some methods such as printf, xlstr and xlstr::operator() produces the same result.
# This is likely because xlstr has only one member, the a char pointer, so that the addresses of xlstr and its char pointer are essentially the same.
# However, the user should always use xlstr::operator() to expose the const char * for its C-str contents, to avoid ambiguity.



<[ xlstr_collection ]>

# A dedicated wrapper to support JavaScript-style split and zip operations for strs.
# Essentially an inherited class from std::vector<xl_str>, with one only extra implementation, the zip method, which concatenates a list of xlstrs.

*/



// DECLARATIONS OF RELEVANT CLASSES.
class xl_str;
class xl_str_collection : public std::vector<xl_str> {
public:
	xl_str zip(const char *) const;
};



// THE XL_STR CLASS.
class xl_str {

	// Wraps an '\0'-terminated C-style str that is not directly accessible.
	char *str;

public:

	// Default constructor.
	xl_str() {
		this->str = (char *)malloc(sizeof(char));
		*(this->str) = 0;
	}
	// Parametric constructor: Instantiates an xlstr as a copy of the C-style str.
	xl_str(const char *str2) {
		size_t memcount = strlen(str2) + 1;
		this->str = (char *)malloc(sizeof(char) * memcount);
		memcpy(this->str, str2, sizeof(char) * memcount);
	}
	// Copy constructor: For a new xlstr instantiated from an Lvalue, the contents are copied.
	xl_str(const xl_str& xlstr2) {
		const char *str2 = xlstr2();
		size_t memcount = strlen(str2) + 1;
		this->str = (char *)malloc(sizeof(char) * memcount);
		memcpy(this->str, str2, sizeof(char) * memcount);
	}
	// Move constructor: For a new xlstr instantiated from an Rvalue, the contents are copied.
	xl_str(xl_str&& xlstr2) {
		const char *str2 = xlstr2();
		size_t memcount = strlen(str2) + 1;
		this->str = (char *)malloc(sizeof(char) * memcount);
		memcpy(this->str, str2, sizeof(char) * memcount);
	}
	// Copy operator: For an existing xlstr reassigned from an Lvalue, the contents are copied.
	xl_str& operator=(const xl_str& xlstr2) {
		free(this->str);
		const char *str2 = xlstr2();
		size_t memcount = strlen(str2) + 1;
		this->str = (char *)malloc(sizeof(char) * memcount);
		memcpy(this->str, str2, sizeof(char) * memcount);
		return *this;
	}
	// Move operator: For an existing xlstr reassigned from an Rvalue, the contents are copied.
	xl_str& operator=(xl_str&& xlstr2) {
		free(this->str);
		const char *str2 = xlstr2();
		size_t memcount = strlen(str2) + 1;
		this->str = (char *)malloc(sizeof(char) * memcount);
		memcpy(this->str, str2, sizeof(char) * memcount);
		return *this;
	}

	// Destructor: Deallocates str upon destruction.
	~xl_str() {
		free(this->str);
	}

	// Returns a const pointer to the xlstr's C-style str content. The str content is readonly and cannot be modified.
	// The main purpose is to provide interface for C-style functions that accept const char *.
	const char *operator()() const {
		return this->str;
	}
	
	// Returns the character at a given position.
	// If the index overflows, returns '\0'.
	// The operator[] overload cannot modify the xlstr's content.
	char operator[](size_t i) const {
		if (i >= strlen(this->str)) return 0;
		else return this->str[i];
	}

	// Returns new xlstr that represents a slice of the xlstr.
	// Equivalent as the slice method.
	xl_str operator()(size_t start, size_t end) const {
		return this->slice(start, end);
	}

	// Returns a new xlstr that concatenates str2 to the end of the current xlstr.
	// Equivalent as the concat method.
	// Provides overload for C-str and xlstr.
	xl_str operator+(const char *str2) const {
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * strlen(this->str));
		memcpy(newstr + strlen(this->str), str2, sizeof(char) * (strlen(str2) + 1));
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}
	xl_str operator+(const xl_str& xlstr2) const {
		const char *str2 = xlstr2();
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * strlen(this->str));
		memcpy(newstr + strlen(this->str), str2, sizeof(char) * (strlen(str2) + 1));
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}
	xl_str operator+(xl_str&& xlstr2) const {
		const char *str2 = xlstr2();
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * strlen(this->str));
		memcpy(newstr + strlen(this->str), str2, sizeof(char) * (strlen(str2) + 1));
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}

	// Concatenates str2 to the end of the current xlstr.
	// This operation modifies the current xlstr.
	// This operation is more efficient than xlstr1 = xlstr1 + str2 since no new copy of xlstr is made.
	// Provides overload for C-str and xlstr.
	void operator+=(const char *str2) {
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		size_t oldstrlen = strlen(this->str);
		this->str = (char *)realloc(this->str, sizeof(char) * memcount);
		memcpy(this->str + oldstrlen, str2, sizeof(char) * (strlen(str2) + 1));
	}
	void operator+=(const xl_str& xlstr2) {
		const char *str2 = xlstr2();
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		size_t oldstrlen = strlen(this->str);
		this->str = (char *)realloc(this->str, sizeof(char) * memcount);
		memcpy(this->str + oldstrlen, str2, sizeof(char) * (strlen(str2) + 1));
	}
	void operator+=(xl_str&& xlstr2) {
		const char *str2 = xlstr2();
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		size_t oldstrlen = strlen(this->str);
		this->str = (char *)realloc(this->str, sizeof(char) * memcount);
		memcpy(this->str + oldstrlen, str2, sizeof(char) * (strlen(str2) + 1));
	}

	// Returns a new xlstr that repeats the current xlstr for count times.
	// Equivalent as the repeat method.
	xl_str operator*(unsigned count) const {
		return this->repeat(count);
	}

	// Compares if the current xlstr has the same content as str2.
	// Provides overload for C-str and xlstr.
	bool operator==(const char *str2) const {
		return strcmp(this->str, str2) == 0;
	}
	bool operator==(const xl_str& xlstr2) const {
		return strcmp(this->str, xlstr2()) == 0;
	}
	bool operator==(xl_str&& xlstr2) const {
		return strcmp(this->str, xlstr2()) == 0;
	}

	// Compares if the current xlstr has different contents from str2.
	// Provides overload for C-str and xlstr.
	bool operator!=(const char *str2) const {
		return strcmp(this->str, str2) != 0;
	}
	bool operator!=(const xl_str& xlstr2) const {
		return strcmp(this->str, xlstr2()) != 0;
	}
	bool operator!=(xl_str&& xlstr2) const {
		return strcmp(this->str, xlstr2()) != 0;
	}

	// Returns the size of the xlstr's character contents excluding the ending '\0'.
	size_t size() const {
		return strlen(this->str);
	}

	// Determines if the xlstr consists of alphabetic letters only.
	// Returns true only when isalpha tests true for all characters.
	bool isalphabetic() const {
		for (char *ptr = this->str; *ptr != 0; ptr++) if (!isalpha(*ptr)) return false;
		return true;
	}

	// Determines if the xlstr consists of alphanumeric characters only.
	// Returns true only when isalnum tests true for all characters.
	bool isalnumeric() const {
		for (char *ptr = this->str; *ptr != 0; ptr++) if (!isalnum(*ptr)) return false;
		return true;
	}

	// Determines if an xlstr represents a valid signed integer.
	// An xlstr that passes this test is convertible to a valid integer via atoi, atol and atoll functions.
	// Consistent with the implementations of atoi, atol and atoll, hexadecimal and exponential notations will be interpreted as false. For hexadecimal and exponential notations, use the more general isfloat and isnumeric methods.
	bool isint() const {
		char *startptr = this->str[0] == '-' ? (this->str + 1) : this->str;
		if (*startptr == 0) return false;
		for (char *ptr = startptr; *ptr != 0; ptr++) if (!isdigit(*ptr)) return false;
		return true;
	}

	// Determines if an xlstr represents a valid floating point number.
	// An xlstr that passes this test is convertible to a valid float via the atof function.
	bool isfloat() const {
		char *ptr = this->str[0] == '-' ? (this->str + 1) : this->str;
		bool hashexnote = (strstr(ptr, "0x") == ptr) || (strstr(ptr, "0X") == ptr);
		ptr += hashexnote ? 2 : 0;
		int(*predicate)(int) = hashexnote ? isxdigit : isdigit;
		char explower = hashexnote ? 'p' : 'e';
		char expupper = hashexnote ? 'P' : 'E';
		if (*ptr == 0 || *ptr == explower || *ptr == expupper) return false;
		bool hasdecimalpoint = false;
		while (*ptr != 0 && *ptr != explower && *ptr != expupper) {
			if (*ptr == '.') {
				if (hasdecimalpoint) return false;
				else hasdecimalpoint = true;
			} else if (!predicate(*ptr)) {
				return false;
			}
			ptr++;
		}
		if (*ptr == 0) return true;
		ptr++;
		if (*ptr == 0) return false;
		ptr += (*ptr == '-') ? 1 : 0;
		hasdecimalpoint = false;
		while (*ptr != 0) {
			if (*ptr == '.') {
				if (hasdecimalpoint) return false;
				else hasdecimalpoint = true;
			} else if (!predicate(*ptr)) {
				return false;
			}
			ptr++;
		}
		return true;
	}

	// Determines if an xlstr represents a valid number.
	// Not to be confused with isalnumeric, which simply tests if the xlstr contains alphabets and numerical characters only.
	bool isnumeric() const {
		return this->isint() || this->isfloat();
	}

	// Returns a new xlstr that concatenates str2 to the current xlstr.
	// Provides overload for C-str and xlstr.
	// Provides overload for single and multiple strs.
	xl_str concat(const char *str2) const {
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * strlen(this->str));
		memcpy(newstr + strlen(this->str), str2, sizeof(char) * (strlen(str2) + 1));
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}
	xl_str concat(const xl_str& xlstr2) const {
		const char *str2 = xlstr2();
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * strlen(this->str));
		memcpy(newstr + strlen(this->str), str2, sizeof(char) * (strlen(str2) + 1));
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}
	xl_str concat(xl_str&& xlstr2) const {
		const char *str2 = xlstr2();
		size_t memcount = strlen(this->str) + strlen(str2) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * strlen(this->str));
		memcpy(newstr + strlen(this->str), str2, sizeof(char) * (strlen(str2) + 1));
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}
	xl_str concat(std::vector<const char *> strs) const {
		size_t memcount = strlen(this->str) + 1;
		for (const char *str : strs) memcount += strlen(str);
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		char *startptr = newstr;
		memcpy(startptr, this->str, sizeof(char) * strlen(this->str));
		startptr += strlen(this->str);
		for (const char *str : strs) {
			memcpy(startptr, str, sizeof(char) * strlen(str));
			startptr += strlen(str);
		}
		*startptr = 0;
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}

	// Determine if the current xlstr ends with the substr.
	// Provides overload for C-str and xlstr.
	bool endswith(const char *substr) const {
		if (strlen(this->str) < strlen(substr)) {
			return false;
		} else {
			char *startcmpptr = this->str + strlen(this->str) - strlen(substr);
			return strcmp(startcmpptr, substr) == 0 ? true : false;
		}
	}
	bool endswith(const xl_str& xlsubstr) const {
		const char *substr = xlsubstr();
		if (strlen(this->str) < strlen(substr)) {
			return false;
		} else {
			char *startcmpptr = this->str + strlen(this->str) - strlen(substr);
			return strcmp(startcmpptr, substr) == 0 ? true : false;
		}
	}
	bool endswith(xl_str&& xlsubstr) const {
		const char *substr = xlsubstr();
		if (strlen(this->str) < strlen(substr)) {
			return false;
		} else {
			char *startcmpptr = this->str + strlen(this->str) - strlen(substr);
			return strcmp(startcmpptr, substr) == 0 ? true : false;
		}
	}

	// Determine if the current xlstr includes the substr.
	// Provides overload for C-str and xlstr.
	bool includes(const char *substr) const {
		return strstr(this->str, substr) != nullptr;
	}
	bool includes(const xl_str& xlsubstr) const {
		return strstr(this->str, xlsubstr()) != nullptr;
	}
	bool includes(xl_str&& xlsubstr) const {
		return strstr(this->str, xlsubstr()) != nullptr;
	}
	
	// Determines the left-most index where substr is found. Returns -1 if no substr is found.
	// Provides overload for C-str and xlstr.
	ptrdiff_t indexof(const char *substr) const {
		char *idxptr = strstr(this->str, substr);
		return (idxptr == nullptr) ? -1 : idxptr - this->str;
	}
	ptrdiff_t indexof(const xl_str& xlsubstr) const {
		char *idxptr = strstr(this->str, xlsubstr());
		return (idxptr == nullptr) ? -1 : idxptr - this->str;
	}
	ptrdiff_t indexof(xl_str&& xlsubstr) const {
		char *idxptr = strstr(this->str, xlsubstr());
		return (idxptr == nullptr) ? -1 : idxptr - this->str;
	}

	// Determines the right-most index where substr is found. Returns -1 if no substr is found.
	// Provides overload for C-str and xlstr.
	ptrdiff_t lastindexof(const char *substr) const {
		char *lastidxptr = nullptr;
		char *startcmpptr = this->str;
		char *tempidxptr = nullptr;
		while (true) {
			tempidxptr = strstr(startcmpptr, substr);
			if (tempidxptr != nullptr) {
				lastidxptr = tempidxptr;
				startcmpptr++;
			} else {
				break;
			}
		}
		return (lastidxptr == nullptr) ? -1 : lastidxptr - this->str;
	}
	ptrdiff_t lastindexof(const xl_str& xlsubstr) const {
		const char *substr = xlsubstr();
		char *lastidxptr = nullptr;
		char *startcmpptr = this->str;
		char *tempidxptr = nullptr;
		while (true) {
			tempidxptr = strstr(startcmpptr, substr);
			if (tempidxptr != nullptr) {
				lastidxptr = tempidxptr;
				startcmpptr++;
			} else {
				break;
			}
		}
		return (lastidxptr == nullptr) ? -1 : lastidxptr - this->str;
	}
	ptrdiff_t lastindexof(xl_str&& xlsubstr) const {
		const char *substr = xlsubstr();
		char *lastidxptr = nullptr;
		char *startcmpptr = this->str;
		char *tempidxptr = nullptr;
		while (true) {
			tempidxptr = strstr(startcmpptr, substr);
			if (tempidxptr != nullptr) {
				lastidxptr = tempidxptr;
				startcmpptr++;
			} else {
				break;
			}
		}
		return (lastidxptr == nullptr) ? -1 : lastidxptr - this->str;
	}

	// Pads after the end of the current xlstr with padstr until targetlen is reached.
	// Provides overload for C-str and xlstr.
	xl_str padend(size_t targetlen, const char *padstr) const {
		if (targetlen <= strlen(this->str)) {
			return xl_str(this->str);
		} else {
			size_t memcount = targetlen + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			char *startptr = newstr;
			memcpy(startptr, this->str, strlen(this->str));
			startptr += strlen(this->str);
			size_t padlen = strlen(padstr);
			for (size_t i = 0; i < targetlen - strlen(this->str); i++) {
				startptr[i] = padstr[i % padlen];
			}
			newstr[memcount - 1] = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}
	xl_str padend(size_t targetlen, xl_str& padxlstr) const {
		if (targetlen <= strlen(this->str)) {
			return xl_str(this->str);
		} else {
			size_t memcount = targetlen + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			char *startptr = newstr;
			memcpy(startptr, this->str, strlen(this->str));
			startptr += strlen(this->str);
			const char *padstr = padxlstr();
			size_t padlen = strlen(padstr);
			for (size_t i = 0; i < targetlen - strlen(this->str); i++) {
				startptr[i] = padstr[i % padlen];
			}
			newstr[memcount - 1] = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}
	xl_str padend(size_t targetlen, xl_str&& padxlstr) const {
		if (targetlen <= strlen(this->str)) {
			return xl_str(this->str);
		} else {
			size_t memcount = targetlen + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			char *startptr = newstr;
			memcpy(startptr, this->str, strlen(this->str));
			startptr += strlen(this->str);
			const char *padstr = padxlstr();
			size_t padlen = strlen(padstr);
			for (size_t i = 0; i < targetlen - strlen(this->str); i++) {
				startptr[i] = padstr[i % padlen];
			}
			newstr[memcount - 1] = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}

	// Pads before the start of the current xlstr with padstr until targetlen is reached.
	// Provides overload for C-str and xlstr.
	xl_str padstart(size_t targetlen, const char *padstr) const {
		if (targetlen <= strlen(this->str)) {
			return xl_str(this->str);
		} else {
			size_t memcount = targetlen + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			char *startptr = newstr;
			size_t padlen = strlen(padstr);
			for (size_t i = 0; i < targetlen - strlen(this->str); i++) {
				*startptr++ = padstr[i % padlen];
			}
			memcpy(startptr, this->str, strlen(this->str));
			startptr += strlen(this->str);
			*startptr = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}
	xl_str padstart(size_t targetlen, xl_str& padxlstr) const {
		if (targetlen <= strlen(this->str)) {
			return xl_str(this->str);
		} else {
			size_t memcount = targetlen + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			char *startptr = newstr;
			const char *padstr = padxlstr();
			size_t padlen = strlen(padstr);
			for (size_t i = 0; i < targetlen - strlen(this->str); i++) {
				*startptr++ = padstr[i % padlen];
			}
			memcpy(startptr, this->str, strlen(this->str));
			startptr += strlen(this->str);
			*startptr = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}
	xl_str padstart(size_t targetlen, xl_str&& padxlstr) const {
		if (targetlen <= strlen(this->str)) {
			return xl_str(this->str);
		} else {
			size_t memcount = targetlen + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			char *startptr = newstr;
			const char *padstr = padxlstr();
			size_t padlen = strlen(padstr);
			for (size_t i = 0; i < targetlen - strlen(this->str); i++) {
				*startptr++ = padstr[i % padlen];
			}
			memcpy(startptr, this->str, strlen(this->str));
			startptr += strlen(this->str);
			*startptr = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}

	// Replaces all occurrences of searchstr with replacestr.
	// No overloads for xlstr are provided.
	xl_str replace(const char *searchstr, const char *replacestr) const {
		return this->split(searchstr).zip(replacestr);
	}

	// Returns a new xlstr that repeats the current xlstr's content for count times.
	// Returns an empty xlstr if count = 0.
	xl_str repeat(unsigned count) const {
		size_t cpycount = strlen(this->str);
		size_t memcount = strlen(this->str) * count + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		char *startptr = newstr;
		while (count--) {
			memcpy(startptr, this->str, sizeof(char) * cpycount);
			startptr += cpycount;
		}
		newstr[memcount - 1] = 0; // MUST APPEND THE ENDING '\0' SINCE IT HAS NOT BEEN COPIED.
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}

	// Returns a new xlstr as the substr including the start but NOT the end index.
	// Returns an empty str if start overflows or start >= end.
	// An end index that overflows will be clamped to the last index of the str.
	xl_str slice(size_t start, size_t end) const {
		end = end > this->size() ? this->size() : end;
		if (start >= strlen(this->str) || end <= start) {
			return xl_str("");
		} else {
			size_t memcount = end - start + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			memcpy(newstr, this->str + start, sizeof(char) * (end - start));
			newstr[memcount - 1] = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}

	// Returns an xl_str_collection instance that contains substrs split by the specified token.
	xl_str_collection split(const char *token) const {
		xl_str_collection tmpxlstrs;
		char *startptr = this->str;
		size_t toklen = strlen(token);
		while (true) {
			char *idxptr = strstr(startptr, token);
			if (idxptr == nullptr) {
				size_t cpycount = strlen(startptr);
				size_t memcount = cpycount + 1;
				char *newstr = (char *)malloc(sizeof(char) * memcount);
				memcpy(newstr, startptr, sizeof(char) * cpycount);
				newstr[memcount - 1] = 0;
				tmpxlstrs.push_back(xl_str(newstr));
				free(newstr);
				break;
			} else {
				size_t cpycount = idxptr - startptr;
				size_t memcount = cpycount + 1;
				char *newstr = (char *)malloc(sizeof(char) * memcount);
				memcpy(newstr, startptr, sizeof(char) * cpycount);
				newstr[memcount - 1] = 0;
				tmpxlstrs.push_back(xl_str(newstr));
				free(newstr);
				startptr = idxptr + toklen;
			}
		}
		return tmpxlstrs;
	}

	// Determines if the current xlstr starts with the substr.
	// Provides overload for C-str and xlstr.
	bool startswith(const char *substr) const {
		return strstr(this->str, substr) == this->str;
	}
	bool startswith(const xl_str& xlsubstr) const {
		return strstr(this->str, xlsubstr()) == this->str;
	}
	bool startswith(xl_str&& xlsubstr) const {
		return strstr(this->str, xlsubstr()) == this->str;
	}

	// Returns a new xlstr where the content in the old xlstr is converted to upper case.
	xl_str touppercase() const {
		size_t memcount = strlen(this->str) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		for (size_t i = 0; this->str[i] != 0; i++) newstr[i] = toupper(this->str[i]);
		newstr[memcount - 1] = 0;
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}

	// Returns a new xlstr where the content in the old xlstr is converted to lower case.
	xl_str tolowercase() const {
		size_t memcount = strlen(this->str) + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		for (size_t i = 0; this->str[i] != 0; i++) newstr[i] = tolower(this->str[i]);
		newstr[memcount - 1] = 0;
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}

	// Returns a new xlstr where spaces at the start and end are removed.
	// Whether or not a character is space depends on the implementation of the isspace() function in C.
	xl_str trim() const {
		char *startptr = this->str;
		while (*startptr != 0 && isspace(*startptr)) startptr++;
		char *endptr = this->str + strlen(this->str) - 1;
		while (endptr >= this->str && isspace(*endptr)) endptr--;
		// If the entire str is spaces, startptr will overrun endptr.
		if (startptr > endptr) {
			return xl_str("");
		} else {
			size_t cpycount = endptr - startptr + 1;
			size_t memcount = cpycount + 1;
			char *newstr = (char *)malloc(sizeof(char) * memcount);
			memcpy(newstr, startptr, sizeof(char) * cpycount);
			newstr[memcount - 1] = 0;
			xl_str newxlstr(newstr);
			free(newstr);
			return newxlstr;
		}
	}

	// Returns a new xlstr where spaces at the start are removed.
	xl_str trimleft() const {
		char *ptr = this->str;
		while (isspace(*ptr)) ptr++;
		return xl_str(ptr);
	}

	// Returns a new xlstr where spaces at the start and end are removed.
	xl_str trimright() const {
		char *ptr = this->str + strlen(this->str) - 1;
		while (ptr - this->str >= 0 && isspace(*ptr)) ptr--;
		size_t cpycount = ptr - this->str + 1;
		size_t memcount = cpycount + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		memcpy(newstr, this->str, sizeof(char) * cpycount);
		newstr[memcount - 1] = 0;
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}

};



// Joins all xlstrs in the xl_str_collection instance with the token and return this as a new xlstr.
xl_str xl_str_collection::zip(const char *token) const {
	if (this->size() == 0) {
		return xl_str("");
	} else {
		size_t toklen = strlen(token);
		size_t lastidx = this->size() - 1;
		size_t cpycount = 0;
		for (size_t i = 0; i < lastidx; i++) cpycount += (*this)[i].size() + toklen;
		cpycount += (*this)[lastidx].size();
		size_t memcount = cpycount + 1;
		char *newstr = (char *)malloc(sizeof(char) * memcount);
		char *startptr = newstr;
		for (size_t i = 0; i < lastidx; i++) {
			memcpy(startptr, (*this)[i](), sizeof(char) * (*this)[i].size());
			startptr += (*this)[i].size();
			memcpy(startptr, token, sizeof(char) * toklen);
			startptr += toklen;
		}
		memcpy(startptr, (*this)[lastidx](), (*this)[lastidx].size() + 1);
		xl_str newxlstr(newstr);
		free(newstr);
		return newxlstr;
	}
}