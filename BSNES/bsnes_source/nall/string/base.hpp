#ifndef NALL_STRING_BASE_HPP
#define NALL_STRING_BASE_HPP

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nall/concept.hpp>
#include <nall/stdint.hpp>
#include <nall/utf8.hpp>
#include <nall/vector.hpp>

namespace nall {
  class string;

  class string {
  public:
    inline void reserve(unsigned);
    inline unsigned length() const;

    inline string& assign(const char*);
    inline string& append(const char*);
    inline string& append(bool);
    inline string& append(signed int value);
    inline string& append(unsigned int value);
    inline string& append(double value);

    template<typename T> inline string& operator= (const T& value);
    template<typename T> inline string& operator<<(const T& value);

    inline operator const char*() const;
    inline char* operator()();
    inline char& operator[](int);

    inline bool operator==(const char*) const;
    inline bool operator!=(const char*) const;
    inline bool operator< (const char*) const;
    inline bool operator<=(const char*) const;
    inline bool operator> (const char*) const;
    inline bool operator>=(const char*) const;

    inline string& operator=(const string&);

    inline string();

    template<typename T1>
    inline string(T1 const &);
    
    template<typename T1, typename T2>
    inline string(T1 const &, T2 const &);

    template<typename T1, typename T2, typename T3>
    inline string(T1 const &, T2 const &, T3 const &);

    template<typename T1, typename T2, typename T3, typename T4>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &);

    template<typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    inline string(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &, T8 const &);

    inline string(string const &);

    inline ~string();

    inline bool readfile(const string&);
    inline string& replace (const char*, const char*);
    inline string& qreplace(const char*, const char*);

    inline string& lower();
    inline string& upper();
    inline string& transform(const char *before, const char *after);
    inline string& ltrim(const char *key = " ");
    inline string& rtrim(const char *key = " ");
    inline string& trim (const char *key = " ");
    inline string& ltrim_once(const char *key = " ");
    inline string& rtrim_once(const char *key = " ");
    inline string& trim_once (const char *key = " ");

  protected:
    char *data;
    unsigned size;

  #if defined(QSTRING_H)
  public:
    inline operator QString() const;
  #endif
  };

  template<typename T> inline string to_string(const T& t)
  {
     return t;
  }

  class lstring : public linear_vector<string> {
  public:
    template<typename T> inline lstring& operator<<(const T& value);

    inline optional<unsigned> find(const char*);
    inline void split (const char*, const char*, unsigned = 0);
    inline void qsplit(const char*, const char*, unsigned = 0);

    lstring();
    
    lstring(const string &);
    lstring(const string &, const string &);
    lstring(const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &, const string &, const string &);
    lstring(const string &, const string &, const string &, const string &, const string &, const string &, const string &, const string &);
    
    lstring(const lstring & str);
    lstring(std::initializer_list<string>);
  };

  //compare.hpp
  inline char chrlower(char c);
  inline char chrupper(char c);
  inline int stricmp(const char *dest, const char *src);
  inline bool strbegin (const char *str, const char *key);
  inline bool stribegin(const char *str, const char *key);
  inline bool strend (const char *str, const char *key);
  inline bool striend(const char *str, const char *key);

  //convert.hpp
  inline char* strlower(char *str);
  inline char* strupper(char *str);
  inline char* strtr(char *dest, const char *before, const char *after);
  inline uintmax_t hex     (const char *str);
  inline intmax_t  integer  (const char *str);
  inline uintmax_t decimal (const char *str);
  inline uintmax_t binary     (const char *str);
  inline double    fp  (const char *str);

  //match.hpp
  inline bool match(const char *pattern, const char *str);

  //math.hpp
  inline bool strint (const char *str, int &result);
  inline bool strmath(const char *str, int &result);

  //strl.hpp
  inline unsigned strlcpy(char *dest, const char *src, unsigned length);
  inline unsigned strlcat(char *dest, const char *src, unsigned length);

  //trim.hpp
  inline char* ltrim(char *str, const char *key = " ");
  inline char* rtrim(char *str, const char *key = " ");
  inline char* trim (char *str, const char *key = " ");
  inline char* ltrim_once(char *str, const char *key = " ");
  inline char* rtrim_once(char *str, const char *key = " ");
  inline char* trim_once (char *str, const char *key = " ");

  //utility.hpp
  inline unsigned strlcpy(string &dest, const char *src, unsigned length);
  inline unsigned strlcat(string &dest, const char *src, unsigned length);
  inline string substr(const char *src, unsigned start = 0, unsigned length = 0);
  inline string& strtr(string &dest, const char *before, const char *after);
  
  inline string integer(intmax_t value);
  template<unsigned length> inline string linteger(intmax_t value);
  template<unsigned length> inline string rinteger(intmax_t value);
  inline string decimal(uintmax_t value);
  template<unsigned length> inline string ldecimal(uintmax_t value);
  template<unsigned length> inline string rdecimal(uintmax_t value);
  template<unsigned length> inline string hex(uintmax_t value);
  template<unsigned length> inline string binary(uintmax_t value);
  inline unsigned fp(char *str, double value);
  inline string fp(double value);

  inline string linteger(intmax_t value) { return linteger<0>(value); }
  inline string rinteger(intmax_t value) { return rinteger<0>(value); }
  inline string ldecimal(uintmax_t value) { return ldecimal<0>(value); }
  inline string rdecimal(uintmax_t value) { return rdecimal<0>(value); }
  inline string hex(uintmax_t value) { return hex<0>(value); }
  inline string binary(uintmax_t value) { return binary<0>(value); }

  //variadic.hpp
  template <typename T1>
  inline void print(T1 const &);

  template <typename T1, typename T2>
  inline void print(T1 const &, T2 const &);

  template <typename T1, typename T2, typename T3>
  inline void print(T1 const &, T2 const &, T3 const &);

  template <typename T1, typename T2, typename T3, typename T4>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &);

  template <typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
  inline void print(T1 const &, T2 const &, T3 const &, T4 const &, T5 const &, T6 const &, T7 const &, T8 const &);
};

#endif
