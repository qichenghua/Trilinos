//-------------------------------------------------------------------------
// Filename      : PerceptMesquiteMesh.hpp, derived from SCVFracMesquiteMesh.hpp
//
// Purpose       : mesh interface for using Mesquite 
//
// Description   : subclass of Mesquite::Mesh
//
// Creator       : Steve Kennon, derived from Steve Owen's work
//
// Creation Date : Nov 2011
//
// Owner         : Steve Kennon
//-------------------------------------------------------------------------
#ifndef PERCEPT_MESQUITE_MESH_HPP
#define PERCEPT_MESQUITE_MESH_HPP

#ifdef STK_BUILT_IN_SIERRA

// #include <mesquite/mesh/MeshInterface.hpp>
// #include <mesquite/MsqError.hpp>
#include <MeshInterface.hpp>
#include <MsqError.hpp>

#include <stk_percept/PerceptMesh.hpp>
#include <map>


namespace stk {
  namespace percept {

    class PerceptMesquiteMesh : public Mesquite::Mesh
    {
      PerceptMesh *m_eMesh;
      stk::mesh::Selector *m_boundarySelector;
      std::map<stk::mesh::Entity *, std::pair<stk::mesh::EntityId, unsigned char> > m_mesquiteNodeDataMap;

    public:

      PerceptMesquiteMesh(PerceptMesh *eMesh);
      void init(PerceptMesh *eMesh);
      int setup();
      bool is_on_my_patch_boundary(stk::mesh::Entity *node_ptr);
      void clean_out();

    public:
      virtual ~PerceptMesquiteMesh();
  
      //************ Operations on entire mesh ****************
      //! Returns whether this mesh lies in a 2D or 3D coordinate system.
      virtual int get_geometric_dimension(Mesquite::MsqError &err);
  
      //! Returns the number of entities of the indicated type.
      virtual size_t get_total_vertex_count(Mesquite::MsqError &err) const;
      virtual size_t get_total_element_count(Mesquite::MsqError &err) const;
  
      //! Fills vector with handles to all vertices/elements
      //! in the mesh.
      virtual void get_all_vertices(std::vector<Mesquite::Mesh::VertexHandle> &vertices,
                                    Mesquite::MsqError &err);
  
      virtual void get_all_elements(std::vector<Mesquite::Mesh::ElementHandle> &elements,
                                    Mesquite::MsqError &err);
  
      //! Returns a pointer to an iterator that iterates over the
      //! set of all vertices in this mesh.  The calling code should
      //! delete the returned iterator when it is finished with it.
      //! If vertices are added or removed from the Mesh after obtaining
      //! an iterator, the behavior of that iterator is undefined.
      //virtual Mesquite::VertexIterator* vertex_iterator(Mesquite::MsqError &err);
  
      //! Returns a pointer to an iterator that iterates over the
      //! set of all top-level elements in this mesh.  The calling code should
      //! delete the returned iterator when it is finished with it.
      //! If elements are added or removed from the Mesh after obtaining
      //! an iterator, the behavior of that iterator is undefined.
      //virtual Mesquite::ElementIterator* element_iterator(Mesquite::MsqError &err);
  
  
      virtual void vertices_get_fixed_flag(const Mesquite::Mesh::VertexHandle vert_array[], 
                                           std::vector<bool>& fixed_flag_array,
                                           size_t num_vtx, 
                                           Mesquite::MsqError &err );
  
      virtual void vertices_get_slaved_flag(const VertexHandle vert_array[], 
                                            std::vector<bool>& slaved_flag_array,
                                            size_t num_vtx, 
                                            Mesquite::MsqError &err );
  
  
      //! Get/set location of a vertex (vertices)
      virtual void vertices_get_coordinates(const Mesquite::Mesh::VertexHandle vert_array[],
                                            Mesquite::MsqVertex* coordinates,
                                            size_t num_vtx,
                                            Mesquite::MsqError &err);
  
      virtual void vertex_set_coordinates(Mesquite::Mesh::VertexHandle vertex,
                                          const Mesquite::Vector3D &coordinates,
                                          Mesquite::MsqError &err);
  
      //! Each vertex has a byte-sized flag that can be used to store
      //! flags.  This byte's value is neither set nor used by the mesh
      //! implementation.  It is intended to be used by Mesquite algorithms.
      //! Until a vertex's byte has been explicitly set, its value is 0.
      virtual void vertex_set_byte (Mesquite::Mesh::VertexHandle vertex,
                                    unsigned char byte,
                                    Mesquite::MsqError &err);
  
      virtual void vertices_set_byte (const Mesquite::Mesh::VertexHandle *vert_array,
                                      const unsigned char *byte_array,
                                      size_t array_size,
                                      Mesquite::MsqError &err);
  
      //! Retrieve the byte value for the specified vertex or vertices.
      //! The byte value is 0 if it has not yet been set via one of the
      //! *_set_byte() functions.
      virtual void vertex_get_byte(const Mesquite::Mesh::VertexHandle vertex,
                                   unsigned char *byte,
                                   Mesquite::MsqError &err);
  
      virtual void vertices_get_byte(const Mesquite::Mesh::VertexHandle *vertex_array,
                                     unsigned char *byte_array,
                                     size_t array_size,
                                     Mesquite::MsqError &err);
  
      // //**************** Vertex Topology *****************    
      //     //! Gets the number of elements attached to this vertex.
      //     //! Useful to determine how large the "elem_array" parameter
      //     //! of the vertex_get_attached_elements() function must be.
      //! Gets the elements attached to this vertex.
  
      virtual void vertices_get_attached_elements(const Mesquite::Mesh::VertexHandle* vertex_array,
                                                  size_t num_vertex,
                                                  std::vector<Mesquite::Mesh::ElementHandle>& elements,
                                                  std::vector<size_t> &offsets,
                                                  Mesquite::MsqError &err);
  
      virtual void elements_get_attached_vertices(const Mesquite::Mesh::ElementHandle *elem_handles,
                                                  size_t num_elems,
                                                  std::vector<Mesquite::Mesh::VertexHandle>& vert_handles,
                                                  std::vector<size_t> &offsets,
                                                  Mesquite::MsqError &err);
  
      //! Returns the topologies of the given entities.  The "entity_topologies"
      //! array must be at least "num_elements" in size.
      virtual void elements_get_topologies(const Mesquite::Mesh::ElementHandle *element_handle_array,
                                           Mesquite::EntityTopology *element_topologies,
                                           size_t num_elements,
                                           Mesquite::MsqError &err);
  
  
      //BEGIN TAGs 
      virtual Mesquite::TagHandle tag_create(const std::string& /*tag_name*/,
                                             Mesquite::Mesh::TagType /*type*/,
                                             unsigned /*length*/,
                                             const void* /*default_value*/,
                                             Mesquite::MsqError &err);
  
      virtual void tag_delete(Mesquite::TagHandle /*handle*/,
                              Mesquite::MsqError& err ); 
  
  
      virtual Mesquite::TagHandle tag_get(const std::string& /*name*/, 
                                          Mesquite::MsqError& err );
  
      virtual void tag_properties(Mesquite::TagHandle /*handle*/,
                                  std::string& /*name_out*/,
                                  Mesquite::Mesh::TagType& /*type_out*/,
                                  unsigned& /*length_out*/,
                                  Mesquite::MsqError& err );
  
  
      virtual void tag_set_element_data(Mesquite::TagHandle /*handle*/,
                                        size_t /*num_elems*/,
                                        const Mesquite::Mesh::ElementHandle* /*elem_array*/,
                                        const void* /*tag_data*/,
                                        Mesquite::MsqError& err );
  
  
      virtual void tag_set_vertex_data (Mesquite::TagHandle /*handle*/,
                                        size_t /*num_elems*/,
                                        const Mesquite::Mesh::VertexHandle* /*node_array*/,
                                        const void* /*tag_data*/,
                                        Mesquite::MsqError& err );
  
      virtual void tag_get_element_data(Mesquite::TagHandle /*handle*/,
                                        size_t /*num_elems*/,
                                        const Mesquite::Mesh::ElementHandle* /*elem_array*/,
                                        void* /*tag_data*/,
                                        Mesquite::MsqError& err );
  
      virtual void tag_get_vertex_data (Mesquite::TagHandle /*handle*/,
                                        size_t /*num_elems*/,
                                        const Mesquite::Mesh::VertexHandle* /*node_array*/,
                                        void* /*tag_data*/,
                                        Mesquite::MsqError& err );
  
      //END TAGS
  
      //**************** Memory Management ****************
      //! Tells the mesh that the client is finished with a given
      //! entity handle.  
      virtual void release_entity_handles(const Mesquite::Mesh::EntityHandle *handle_array,
                                          size_t num_handles,
                                          Mesquite::MsqError &err);
  
      //! Instead of deleting a Mesh when you think you are done,
      //! call release().  In simple cases, the implementation could
      //! just call the destructor.  More sophisticated implementations
      //! may want to keep the Mesh object to live longer than Mesquite
      //! is using it.
      virtual void release();
  
      /*
        CMLNode** get_node_array()
        {return nodeArray;}
        CMLMeshEntity** get_element_array()
        {return elementArray;}
        int get_num_nodes()
        {return numNodes;}
        int get_num_elements()
        {return numElements;}
        bool get_jacobi_vertex_coords(CMLNode *node_ptr, CubitVector &coords);
      */
  
    private:
#if 0
      // sjowen debug
      bool dump_hexes(DLIList<CMLHex *> &hex_list, DLIList<CMLNode *> &node_list, int rank);
      bool get_nodes_on_hexes(DLIList<CMLHex *> &hex_list,
                              DLIList<CMLNode *> &node_list);
      bool isSetup;
      int myRank;
  
      //MRefEntity* mOwner;
  
      DLIList<SCGeoEntity*> mOwners;
  
      /*DLIList<MeshEntity*>* cachedEntityList;*/
      /*MLNode** cachedNode;*/
      //DLIList<MeshEntity*> elementList;
  
      std::map<CMLNode *, std::pair<int, unsigned char> > mesquiteNodeDataMap;
      std::map<CMLNode *, CubitVector> nodeCoords;

      std::map<Mesquite::Mesh::VertexHandle, int> jacobiCoordsMap;
      double *jacobiCoords;
      int nextJacobiCoordsIndex;
  
      CMLNode** nodeArray;
      CMLMeshEntity** elementArray;
      std::set<CMLMeshEntity *> includedElements;
      int numNodes;
      int numElements;
  
      //booleans about what element types we have
      CubitBoolean triExists;
      CubitBoolean quadExists;
      CubitBoolean tetExists;
      CubitBoolean hexExists;
  
      bool freeScheme;
      bool doJacobiIterations;
  
      unsigned char myDimension;
    
      bool is_on_my_patch_boundary(CMLNode *node_ptr);
#endif
  

    };


    //static bool myPerceptMesquiteMesh_hpp = true; 

  } // namespace percept
} // namespace stk

#endif // STK_BUILT_IN_SIERRA

#endif //has file been included
