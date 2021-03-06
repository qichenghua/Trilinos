/*
// Copyright (c) 2013, Sandia Corporation.
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
// 
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
// 
//     * Neither the name of Sandia Corporation nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
 */

#include <pwd.h>
#include <unistd.h>

#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <stdexcept>
#include <iomanip>
#include <algorithm>
#include <locale>

#include <stk_util/util/FeatureTest.hpp>
#include <stk_util/environment/Env.hpp>
#include <stk_util/diag/Platform.hpp>
#include <stk_util/parallel/MPI.hpp>

#include <stk_util/util/Writer.hpp>
#include <stk_util/diag/SlibDiagWriter.hpp>
#include <stk_util/diag/Timer.hpp>
#include <stk_util/environment/Trace.hpp>

#include <fcntl.h>

#if defined(__GNUC__)
#include <fstream>
#ifndef __APPLE__
#include <malloc.h>
#else
#include <sys/malloc.h>
#endif
#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>
#if __GNUC__ == 3 || __GNUC__ == 4 || __GNUC__ == 5
#include <cxxabi.h>
#endif

#elif defined(__PGI)
#include <fstream>
#include <malloc.h>
#include <cstdlib>
#include <sys/time.h>
#include <sys/resource.h>

#endif

#if defined(__PUMAGON__)
extern "C" {
#include <util.h>
#include <sys/param.h>
}

#elif defined(__JVN)
#include <sys/param.h>
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#elif defined(__IBMC__) || defined(__IBMCPP__)
#include <sys/utsname.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <netdb.h>

#else
#include <sys/utsname.h>
#include <sys/time.h>
#include <netdb.h>
#endif

#include <stk_util/util/MallocUsed.h>

//
//  NKC 2/21/08 Some of the calls in this file bomb out purify.  Uncomment the line below if trying
//  to build a purify executable.
//
//#define PURIFY_BUILD

namespace sierra {
namespace Env {

#if defined(_AIX)
// Cleanup AIX locale initialization problems
void
startup_preparallel_platform()
{
  std::locale loc("POSIX");

  std::locale::global(loc); // std::locale::classic());
  std::cout.imbue(loc); // std::locale::classic());
  std::cin.imbue(loc); // std::locale::classic());

  std::ostringstream strout;
  strout << "Don't ask why the IBM locale works if I do this " << 10000000 << std::endl;
}

#else
void
startup_preparallel_platform()
{}
#endif


void
get_heap_info(
  size_t &		heap_size,
  size_t &		largest_free)
{
  heap_size = 0;
  largest_free = 0;

#if defined(SIERRA_HEAP_INFO)

# if defined(SIERRA_PTMALLOC3_ALLOCATOR) || defined(SIERRA_PTMALLOC2_ALLOCATOR)
  heap_size = malloc_used();
  
# elif defined(__linux__) && ! defined(__IBMCPP__)
  static struct mallinfo minfo;
  minfo = mallinfo();
  heap_size = static_cast<unsigned int>(minfo.uordblks) + static_cast<unsigned int>(minfo.hblkhd);
  largest_free = static_cast<unsigned int>(minfo.fordblks);

  slibout.m(Slib::LOG_MEMORY) << "size_t size " << sizeof(size_t)*8 << " bits"
                              << ", heap size " << heap_size
                              << ", arena " << static_cast<unsigned int>(minfo.arena)
			      << ", ordblks " << minfo.ordblks
			      << ", smblks " << minfo.smblks
			      << ", hblks " << minfo.hblks
			      << ", hblkhd " << static_cast<unsigned int>(minfo.hblkhd)
			      << ", usmblks " << minfo.usmblks
			      << ", fsmblks " << minfo.fsmblks
			      << ", uordblks " << static_cast<unsigned int>(minfo.uordblks)
			      << ", fordblks " << static_cast<unsigned int>(minfo.fordblks)
			      << ", keepcost " << minfo.keepcost << Diag::dendl;


# endif
#endif // defined(SIERRA_HEAP_INFO)
}


size_t get_available_memory()
{
  // The value returned for _SC_AVPHYS_PAGES is the amount of memory
  // the application can use without hindering any other process
  // (given that no other process increases its memory usage).
#if !defined(__APPLE__) && !defined(__FreeBSD__)
  static size_t pagesize = getpagesize();
  size_t avail = sysconf(_SC_AVPHYS_PAGES);
  return avail * pagesize;
#else
  // _SC_AVPHYS_PAGES does not exist on FreeBSD/Apple
  return 0;
#endif
}

void
get_memory_info(
  size_t &		memory_usage,
  size_t &		faults)
{
  memory_usage = 0;
  faults = 0;

#if defined(SIERRA_MEMORY_INFO)

#if defined(__linux__)
  std::ifstream proc("/proc/self/stat", std::ios_base::in|std::ios_base::binary);
  if (proc) {

    std::string s("");
    int i=-1;
    for (i = 0; i < 11; ++i)
      proc >> s;

    proc >> faults;
    ++i;

    for (; i < 22; ++i)
      proc >> s;

    proc >> memory_usage;
    ++i;
  }
# endif

#endif // defined(SIERRA_MEMORY_INFO)
}


std::string
hostname()
{
  char buf[255];
  ::gethostname(buf, sizeof(buf));
  return std::string(buf);
}


std::string
domainname()
{
  std::string domain(".");
  char buf[255];

  ::getdomainname(buf, sizeof(buf));
  if (::strlen(buf)) {
    domain += buf;
  }
  return domain;
}

namespace {

std::string get_env_user()
{
  char* env_user = std::getenv("USER");
  if (env_user) {
    std::string str_env_user(env_user);
    if (!str_env_user.empty()) {
      return str_env_user;
    }
  }

  std::string unknown("unknown");
  return unknown;
}

}

std::string
username()
{
  std::string env_user_name = get_env_user();
#if defined(__CRAYXT_COMPUTE_LINUX_TARGET)
  std::string user_name(get_param("username"));

  if (user_name.empty()) {
    return env_user_name;
  }

  return user_name;
#else
  struct passwd *user_info = ::getpwuid(::geteuid());
  std::string user_name = (user_info ? user_info->pw_name : "unknown");

  if (user_name == "unknown") {
    return env_user_name;
  }

  return user_name;
#endif
}


std::string
hardware()
{
  struct utsname	uts_name;

  uname(&uts_name);

  return uts_name.machine;
}


std::string
osname()
{
  struct utsname	uts_name;

  uname(&uts_name);

  return uts_name.sysname;
}


std::string
osversion()
{
  struct utsname	uts_name;

  uname(&uts_name);

  return uts_name.release;
}


int
pid()
{
  return ::getpid();
}


int
pgrp()
{
  return ::getpgrp();
}


bool
path_access(
  const std::string &	name,
  int			mode)

{
  return !name.empty() && ::access(name.c_str(), mode) == 0;
}


bool
path_exists(
  const std::string &	name)
{
  return path_access(name, F_OK);
}


bool
path_read_access(
  const std::string &	name)
{
  return path_access(name, R_OK);
}


bool
path_write_access(
  const std::string &	name)
{
  return path_access(name, W_OK);
}


namespace {

struct flock *
file_lock(
  short	type,
  short	whence)
{
  static struct flock ret;
  ret.l_type = type;
  ret.l_start = 0;
  ret.l_whence = whence;
  ret.l_len = 0;
  ret.l_pid = 0; //getpid();
  return &ret;
}

} // namespace <unnamed>

bool
write_lock(
  int		fd)
{
  int i =::fcntl(fd, F_SETLK, file_lock(F_WRLCK, SEEK_SET));

  return i != -1;
}


bool
release_lock(
  int		fd)
{
  int i =::fcntl(fd, F_SETLK, file_lock(F_UNLCK, SEEK_SET));
  return i != -1;
}


bool
read_lock(
  int		fd)
{
  return ::fcntl(fd, F_SETLK, file_lock(F_RDLCK, SEEK_SET)) != -1;
}


bool
append_lock(
  int		fd)
{
  return ::fcntl(fd, F_SETLK, file_lock(F_WRLCK, SEEK_END)) != -1;
}

} // namespace Env
} // namespace sierra

