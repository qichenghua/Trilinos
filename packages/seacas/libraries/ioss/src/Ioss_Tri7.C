// Copyright(C) 2016
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

// ========================================================================
// Define a variable type for storage of this elements connectivity
#include "Ioss_CodeTypes.h"           // for IntVector
#include "Ioss_ElementTopology.h"     // for ElementTopology
#include <Ioss_ElementVariableType.h> // for ElementVariableType
#include <Ioss_Tri7.h>
#include <cassert> // for assert
#include <cstddef> // for nullptr

namespace Ioss {
  class St_Tri7 : public ElementVariableType
  {
  public:
    static void factory() { static St_Tri7 registerThis; }

  protected:
    St_Tri7() : ElementVariableType(Ioss::Tri7::name, 7) {}
  };
} // namespace Ioss
//------------------------------------------------------------------------
namespace {
  struct Constants
  {
    static const int nnode     = 7;
    static const int nedge     = 3;
    static const int nedgenode = 3;
    static const int nface     = 0;
    static const int nfacenode = 0;
    static const int nfaceedge = 0;
    static int       edge_node_order[nedge][nedgenode];
  };

  // Edge numbers are zero-based [0..number_edges)
  int Constants::edge_node_order[nedge][nedgenode] = // [edge][edge_node]
      {{0, 1, 3}, {1, 2, 4}, {2, 0, 5}};
} // namespace

void Ioss::Tri7::factory()
{
  static Ioss::Tri7 registerThis;
  Ioss::St_Tri7::factory();
}

Ioss::Tri7::Tri7() : Ioss::ElementTopology(Ioss::Tri7::name, "Triangle_7")
{
  Ioss::ElementTopology::alias(Ioss::Tri7::name, "triangle7");
  Ioss::ElementTopology::alias(Ioss::Tri7::name, "Solid_Tri_7_2D");
  Ioss::ElementTopology::alias(Ioss::Tri7::name, "Face_Tri_7_3D");
  Ioss::ElementTopology::alias(Ioss::Tri7::name, "TRIANGLE_7_2D");
  Ioss::ElementTopology::alias(Ioss::Tri7::name, "triface7");
}

Ioss::Tri7::~Tri7() = default;

int Ioss::Tri7::parametric_dimension() const { return 2; }
int Ioss::Tri7::spatial_dimension() const { return 2; }
int Ioss::Tri7::order() const { return 2; }

int Ioss::Tri7::number_corner_nodes() const { return 3; }
int Ioss::Tri7::number_nodes() const { return Constants::nnode; }
int Ioss::Tri7::number_edges() const { return Constants::nedge; }
int Ioss::Tri7::number_faces() const { return Constants::nface; }

int Ioss::Tri7::number_nodes_edge(int /* edge */) const { return Constants::nedgenode; }
int Ioss::Tri7::number_nodes_face(int /* face */) const { return Constants::nfacenode; }
int Ioss::Tri7::number_edges_face(int /* face */) const { return Constants::nfaceedge; }

Ioss::IntVector Ioss::Tri7::edge_connectivity(int edge_number) const
{
  assert(edge_number > 0 && edge_number <= number_edges());
  Ioss::IntVector connectivity(Constants::nedgenode);

  for (int i = 0; i < Constants::nedgenode; i++) {
    connectivity[i] = Constants::edge_node_order[edge_number - 1][i];
  }

  return connectivity;
}

Ioss::IntVector Ioss::Tri7::face_connectivity(int /* face_number */) const
{
  Ioss::IntVector connectivity;
  return connectivity;
}

Ioss::IntVector Ioss::Tri7::element_connectivity() const
{
  Ioss::IntVector connectivity(number_nodes());

  for (int i = 0; i < number_nodes(); i++) {
    connectivity[i] = i;
  }

  return connectivity;
}

Ioss::ElementTopology *Ioss::Tri7::face_type(int /* face_number */) const
{
  return (Ioss::ElementTopology *)nullptr;
}

Ioss::ElementTopology *Ioss::Tri7::edge_type(int edge_number) const
{
  assert(edge_number >= 0 && edge_number <= number_edges());
  return Ioss::ElementTopology::factory("edge3");
}
