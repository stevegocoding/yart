#pragma once

#include <wchar.h>
#include <assert.h>

#include <vector>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <limits>

#define BOOST_MEM_FN_ENABLE_STDCALL 
#include <boost/assign.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/assert.hpp>
#include <boost/utility.hpp>
#include <boost/noncopyable.hpp>
#include <boost/foreach.hpp>
#include <boost/foreach_fwd.hpp>
#include <boost/mem_fn.hpp>

#include <Windows.h>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH