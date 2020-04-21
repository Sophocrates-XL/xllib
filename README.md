# xllib
Contains custom headers and modules for personal practice and practical use.

< xlstr.h >
- A custom c++ header with wrappers for C-style strings as well as implementations for string methods similar to JavaScript and Python. Users can perform basic string operations with the methods provided, where details of memory allocation and deallocation are hidden.
- Provides the xl_str class, which is essentially a wrapper of the char * type, where malloc, realloc and free functions are used for the underlying memory management during string processing.
- An additional xl_str_collection class is provided as an child class from std::vector<char> where a string can be split and zipped back in JavaScript style.
- As the header is initially motivated for the author's personal practice, it is not as efficient as most established libraries, and currently only supports the char type.
