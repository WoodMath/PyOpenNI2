#include <string.h>
#include <boost/python.hpp>
#include <algorithm>
#include <string>
#include <set>
#include <OpenNI.h>

using namespace boost::python;

// poor man indentation
std::string indent(const char* str, int sp) {
  // calculate the length
  int len = sp;
  for (int i=0; str[i]; i++,len++)
    if (str[i]=='\n') len += sp;
  char* out = new char[len+1];
  // process
  memset(out, ' ', sp);
  for (int i=0,o=sp; (out[o++]=str[i]); i++)
    if (str[i]=='\n') {
      memset(out+o, ' ', sp);
      o += sp;
    }
  // return
  std::string ret (out);
  delete [] out;
  return ret;
}

// Nasty hack to add custom docstring to a make_function() object
void defd(const char* name, object func, const char* docstring) {
  def(name, func);
  func.attr("__doc__") = func.attr("__doc__").slice(1,_)
                       + (" :\n" + indent(docstring, 4));
}

//////#define defo(name, func, args, doc) defd(name, make_function(func, default_call_policies(), args), doc)
//////#define defop(name, func, policy, args, doc) defd(name, make_function(func, policy, args), doc)
//////#define defunc(func, args) make_function(func, default_call_policies(), args)
//////#define defuncp(func, policy, args) make_function(func, policy, args)



class Caller {
public:
  void clear() {
    listeners.clear();
  }
  void add(object cb) {
    listeners.insert(cb);
  }
  bool remove(object cb) {
    return listeners.erase(cb);
  }
protected:
  inline void callall(boost::function<void(object)> f) {
    PyGILState_STATE state = PyGILState_Ensure();
    //FIXME: handle thrown exceptions correctly
    std::for_each(listeners.begin(), listeners.end(), f);
    PyGILState_Release(state);
  }
private:
  std::set<object> listeners;
};

struct CallerEntry {
  const char* name;
  Caller* ptr;
};

#define CALLER(NAME, CLASS,ID, SIGNATURE, FUNC)                                \
  class ID##Caller : public CLASS::ID##Listener, public Caller {               \
  public:                                                                      \
    void on##ID SIGNATURE {                                                    \
      callall(FUNC);                                                           \
    }                                                                          \
  };                                                                           \
  ID##Caller NAME;

#define REG_CALLER(NAME, CLASS,ID) check(CLASS::add##ID##Listener(&NAME))

// Iterate over the caller list (see later) ------------------------------------
#define _callers_iter                                                          \
  int i = 0;                                                                   \
  CallerEntry* entry;                                                          \
  while ((entry=callers+(i++))->name)
#define _callers_find(THEN)                                                    \
  _callers_iter if (!strcmp(entry->name,name)) return THEN;                    \
  PyErr_SetString(PyExc_KeyError, "No such event.");                           \
  throw error_already_set();
// -----------------------------------------------------------------------------

#define CALLERS(...) CallerEntry callers[] = { __VA_ARGS__, {NULL,NULL} };     \
  void __on  (const char* name, object cb)                                     \
    { _callers_find(entry->ptr->add(cb)) }                                     \
  void __off0()                                                                \
    { _callers_iter entry->ptr->clear(); }                                     \
  void __off1(const char* name)                                                \
    { _callers_find(entry->ptr->clear()) }                                     \
  bool __off2(const char* name, object cb)                                     \
    { _callers_find(entry->ptr->remove(cb)) }

template <class T> tuple convertArray(const openni::Array<T>& array) {
  //FIXME: find a better way
  tuple ret;
  int length = array.getSize();
  for (int i=0; i<length; i++) ret += make_tuple(array[i]);
  return ret;
}


