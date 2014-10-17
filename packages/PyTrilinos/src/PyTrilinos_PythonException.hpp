// @HEADER
// ***********************************************************************
//
//              PyTrilinos: Python Interface to Trilinos
//                 Copyright (2005) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
// USA
// Questions? Contact Bill Spotz (wfspotz@sandia.gov)
//
// ***********************************************************************
// @HEADER

#ifndef PYTRILINOS_PYTHONEXCEPTION_HPP
#define PYTRILINOS_PYTHONEXCEPTION_HPP

// The purpose of the PythonException class is to convert a python
// error to a C++ exception.  This is useful, especially in C++
// constructors that access the python/C API, when a python error is
// raised but a C++ exception should be thrown in order to ensure
// proper cleanup of a failed operation.

// When a python error is detected, or if the code sets a python
// error, this should be followed with
//
//     throw PyTrilinos::PythonException();
//
// The PythonException will extract the information it needs directly
// from the python API.

// Typically, the wrapper code should catch this exception and convert
// it back to a python error, using the restore() method.  Using SWIG,
// this can be done with the %exception directive:

// %exception
// {
//    try
//    {
//      $action
//    }
//    catch (PyTrilinos::PythonException &e)
//    {
//       e.restore();
//       SWIG_fail;
//    }
// }

#include <Python.h>
#include <stdexcept>

namespace PyTrilinos
{

class PythonException : public std::runtime_error
{
public:
  PythonException();
  ~PythonException() throw();
  const char * what() const throw();
  void restore();
private:
  PyObject * errorType;
  PyObject * errorValue;
  PyObject * errorTraceback;
  char     * errorMsg;
};

}  // Namespace PyTrilinos

#endif