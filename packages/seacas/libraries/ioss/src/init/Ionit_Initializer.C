// Copyright(C) 1999-2010
// Sandia Corporation. Under the terms of Contract
// DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
// certain rights in this software.
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

#include <Ionit_Initializer.h>

#if !defined(NO_EXODUS_SUPPORT)
#include <exo_fac/Ioex_IOFactory.h>
#endif

#include <generated/Iogn_DatabaseIO.h>
#include <heartbeat/Iohb_DatabaseIO.h>

#if !defined(NO_PAMGEN_SUPPORT)
#include <pamgen/Iopg_DatabaseIO.h>
#endif

#if !defined(NO_CGNS_SUPPORT)
#include <cgns/Iocgns_IOFactory.h>
#endif

#include <visualization/Iovs_IOFactory.h>
#include <Ioss_ConcreteVariableType.h>
#include <Ioss_Initializer.h>
#include <transform/Iotr_Initializer.h>

namespace Ioss {
  namespace Init {
    Initializer &Initializer::initialize_ioss()
    {
      static Initializer ionit;
      return ionit;
    }

    /** \brief Initialize the Ioss library.
     *
     *  Calls appropriate internal functions and methods to
     *  initialize the Ioss library. Initializes all database
     *  types except xdmf.
     */
    Initializer::Initializer()
    {
#if !defined(NO_EXODUS_SUPPORT)
      Ioex::IOFactory::factory(); // Exodus
#endif
      Iohb::IOFactory::factory(); // HeartBeat
      Iogn::IOFactory::factory(); // Generated
#if !defined(NO_PAMGEN_SUPPORT)
      Iopg::IOFactory::factory(); // Pamgen
#endif
      Iovs::IOFactory::factory(); // Visualization
#if !defined(NO_CGNS_SUPPORT)
      Iocgns::IOFactory::factory();
#endif
      Ioss::StorageInitializer();
      Ioss::Initializer();
      Iotr::Initializer();
    }

    Initializer::~Initializer()
    {
      try {
        Ioss::IOFactory::clean();
        // Put code here that should run after sierra is finished
        // executing...
      }
      catch (...) {
      }
    }
  } // namespace Init
} // namespace Ioss
