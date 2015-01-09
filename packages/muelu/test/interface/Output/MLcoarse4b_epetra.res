ML ParameterList: 
coarse: type = Gauss-Seidel   [unused]
coarse: sweeps = 25   [unused]
coarse: damping factor = 0.6   [unused]

MueLu ParameterList: 
max levels = 10   [unused]
cycle type = V   [unused]
sa: damping factor = 1.333   [unused]
sa: eigenvalue estimate num iterations = 10   [unused]
smoother: type = RELAXATION   [unused]
smoother: pre or post = both   [unused]
coarse: type = RELAXATION   [unused]
coarse: max size = 128   [unused]
smoother: params -> 
 relaxation: type = Symmetric Gauss-Seidel   [unused]
 relaxation: sweeps = 2   [unused]
 relaxation: damping factor = 1   [unused]
coarse: params -> 
 relaxation: type = Gauss-Seidel   [unused]
 relaxation: sweeps = 25   [unused]
 relaxation: damping factor = 0.6   [unused]

max levels = 10
cycle type = V
sa: damping factor = 1.33
sa: eigenvalue estimate num iterations = 10   [unused]
smoother: type = RELAXATION
smoother: pre or post = both
coarse: type = RELAXATION
coarse: max size = 128
number of equations = 1   [default]
smoother: params -> 
 relaxation: type = Symmetric Gauss-Seidel   [unused]
 relaxation: sweeps = 2   [unused]
 relaxation: damping factor = 1.00   [unused]
coarse: params -> 
 relaxation: type = Gauss-Seidel   [unused]
 relaxation: sweeps = 25   [unused]
 relaxation: damping factor = 0.60   [unused]


******* WARNING *******
The following parameters were not used:
    sa: eigenvalue estimate num iterations = 10   [unused]

Clearing old data (if any)
Using default factory (MueLu::AmalgamationFactory) for building 'UnAmalgamationInfo'.
Level 0
 Setup Smoother (MueLu::IfpackSmoother{type = point relaxation stand-alone})
  IFPACK (Local SGS, sweeps=2, damping=1)
Using default factory (MueLu::AmalgamationFactory) for building 'UnAmalgamationInfo'.
Level 1
 Prolongator smoothing (MueLu::SaPFactory)
  Matrix filtering (MueLu::FilteredAFactory)
   Build (MueLu::CoalesceDropFactory)
    lightweight wrap = 1
    algorithm = "classical": threshold = 0.00, blocksize = 1
    Detected 0 Dirichlet nodes
    Number of dropped entries in unamalgamated matrix graph: 0/29995 (0.00%)
   Filtered matrix is not being constructed as no filtering is being done
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Algo "Phase - (Dirichlet)"
     BuildAggregates (Phase - (Dirichlet))
       aggregated : 0 (phase), 0/9999 [0.00%] (total)
       remaining  : 9999
       aggregates : 0 (phase), 0 (total)
    Algo "Phase 1 (main)"
     BuildAggregates (Phase 1 (main))
       aggregated : 9998 (phase), 9998/9999 [99.99%] (total)
       remaining  : 1
       aggregates : 3333 (phase), 3333 (total)
    Algo "Phase 2 (cleanup)"
     BuildAggregates (Phase 2 (cleanup))
       aggregated : 1 (phase), 9999/9999 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 3333 (total)
    Algo "Phase 3 (emergency)"
     BuildAggregates (Phase 3 (emergency))
       aggregated : 0 (phase), 9999/9999 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 3333 (total)
    Algo "Phase - (isolated)"
     BuildAggregates (Phase - (isolated))
       aggregated : 0 (phase), 9999/9999 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 3333 (total)
    "UC": MueLu::Aggregates{nGlobalAggregates = 3333}
   Build (MueLu::AmalgamationFactory)
    AmalagamationFactory::Build(): found fullblocksize=1 and stridedblocksize=1 from strided maps. offset=0
   Nullspace factory (MueLu::NullspaceFactory)
    Generating canonical nullspace: dimension = 1
   Build (MueLu::CoarseMapFactory)
    domainGIDOffset: 0 block size: 1 stridedBlockId: -1
   Column map is consistent with the row map, good.
   TentativePFactory : aggregates do not cross process boundaries
   Ptent size =  9999 x 3333, nnz = 9999
   Ptent Load balancing info
   Ptent   # active processes: 1/1
   Ptent   # rows per proc   : avg = 1.00e+04,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
   Ptent   #  nnz per proc   : avg = 1.00e+04,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Eigenvalue estimate
   Calculating max eigenvalue estimate now (max iters = 10)
   Prolongator damping factor = 0.68 (1.33 / 1.95)
  Fused (I-omega*D^{-1} A)*Ptent
  P size =  9999 x 3333, nnz = 16663
  P Load balancing info
  P   # active processes: 1/1
  P   # rows per proc   : avg = 1.00e+04,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  P   #  nnz per proc   : avg = 1.67e+04,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Transpose P (MueLu::TransPFactory)
  R size =  3333 x 9999, nnz = 16663
  R Load balancing info
  R   # active processes: 1/1
  R   # rows per proc   : avg = 3.33e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  R   #  nnz per proc   : avg = 1.67e+04,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Computing Ac (MueLu::RAPFactory)
  MxM: A x P
  MxM: R x (AP) (explicit)
  Ac size =  3333 x 3333, nnz = 9997
  Ac Load balancing info
  Ac   # active processes: 1/1
  Ac   # rows per proc   : avg = 3.33e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Ac   #  nnz per proc   : avg = 1.00e+04,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Setup Smoother (MueLu::IfpackSmoother{type = point relaxation stand-alone})
  IFPACK (Local SGS, sweeps=2, damping=1)
Using default factory (MueLu::AmalgamationFactory) for building 'UnAmalgamationInfo'.
Level 2
 Prolongator smoothing (MueLu::SaPFactory)
  Matrix filtering (MueLu::FilteredAFactory)
   Build (MueLu::CoalesceDropFactory)
    lightweight wrap = 1
    algorithm = "classical": threshold = 0.00, blocksize = 1
    Detected 0 Dirichlet nodes
    Number of dropped entries in unamalgamated matrix graph: 0/9997 (0.00%)
   Filtered matrix is not being constructed as no filtering is being done
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Algo "Phase - (Dirichlet)"
     BuildAggregates (Phase - (Dirichlet))
       aggregated : 0 (phase), 0/3333 [0.00%] (total)
       remaining  : 3333
       aggregates : 0 (phase), 0 (total)
    Algo "Phase 1 (main)"
     BuildAggregates (Phase 1 (main))
       aggregated : 3332 (phase), 3332/3333 [99.97%] (total)
       remaining  : 1
       aggregates : 1111 (phase), 1111 (total)
    Algo "Phase 2 (cleanup)"
     BuildAggregates (Phase 2 (cleanup))
       aggregated : 1 (phase), 3333/3333 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 1111 (total)
    Algo "Phase 3 (emergency)"
     BuildAggregates (Phase 3 (emergency))
       aggregated : 0 (phase), 3333/3333 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 1111 (total)
    Algo "Phase - (isolated)"
     BuildAggregates (Phase - (isolated))
       aggregated : 0 (phase), 3333/3333 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 1111 (total)
    "UC": MueLu::Aggregates{nGlobalAggregates = 1111}
   Build (MueLu::AmalgamationFactory)
    AmalagamationFactory::Build(): found fullblocksize=1 and stridedblocksize=1 from strided maps. offset=0
   Nullspace factory (MueLu::NullspaceFactory)
   Build (MueLu::CoarseMapFactory)
    domainGIDOffset: 0 block size: 1 stridedBlockId: -1
   Column map is consistent with the row map, good.
   TentativePFactory : aggregates do not cross process boundaries
   Ptent size =  3333 x 1111, nnz = 3333
   Ptent Load balancing info
   Ptent   # active processes: 1/1
   Ptent   # rows per proc   : avg = 3.33e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
   Ptent   #  nnz per proc   : avg = 3.33e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Eigenvalue estimate
   Calculating max eigenvalue estimate now (max iters = 10)
   Prolongator damping factor = 0.68 (1.33 / 1.95)
  Fused (I-omega*D^{-1} A)*Ptent
  P size =  3333 x 1111, nnz = 5553
  P Load balancing info
  P   # active processes: 1/1
  P   # rows per proc   : avg = 3.33e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  P   #  nnz per proc   : avg = 5.55e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Transpose P (MueLu::TransPFactory)
  R size =  1111 x 3333, nnz = 5553
  R Load balancing info
  R   # active processes: 1/1
  R   # rows per proc   : avg = 1.11e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  R   #  nnz per proc   : avg = 5.55e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Computing Ac (MueLu::RAPFactory)
  MxM: A x P
  MxM: R x (AP) (explicit)
  Ac size =  1111 x 1111, nnz = 3331
  Ac Load balancing info
  Ac   # active processes: 1/1
  Ac   # rows per proc   : avg = 1.11e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Ac   #  nnz per proc   : avg = 3.33e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Setup Smoother (MueLu::IfpackSmoother{type = point relaxation stand-alone})
  IFPACK (Local SGS, sweeps=2, damping=1)
Using default factory (MueLu::AmalgamationFactory) for building 'UnAmalgamationInfo'.
Level 3
 Prolongator smoothing (MueLu::SaPFactory)
  Matrix filtering (MueLu::FilteredAFactory)
   Build (MueLu::CoalesceDropFactory)
    lightweight wrap = 1
    algorithm = "classical": threshold = 0.00, blocksize = 1
    Detected 0 Dirichlet nodes
    Number of dropped entries in unamalgamated matrix graph: 0/3331 (0.00%)
   Filtered matrix is not being constructed as no filtering is being done
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Algo "Phase - (Dirichlet)"
     BuildAggregates (Phase - (Dirichlet))
       aggregated : 0 (phase), 0/1111 [0.00%] (total)
       remaining  : 1111
       aggregates : 0 (phase), 0 (total)
    Algo "Phase 1 (main)"
     BuildAggregates (Phase 1 (main))
       aggregated : 1111 (phase), 1111/1111 [100.00%] (total)
       remaining  : 0
       aggregates : 371 (phase), 371 (total)
    Algo "Phase 2 (cleanup)"
     BuildAggregates (Phase 2 (cleanup))
       aggregated : 0 (phase), 1111/1111 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 371 (total)
    Algo "Phase 3 (emergency)"
     BuildAggregates (Phase 3 (emergency))
       aggregated : 0 (phase), 1111/1111 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 371 (total)
    Algo "Phase - (isolated)"
     BuildAggregates (Phase - (isolated))
       aggregated : 0 (phase), 1111/1111 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 371 (total)
    "UC": MueLu::Aggregates{nGlobalAggregates = 371}
   Build (MueLu::AmalgamationFactory)
    AmalagamationFactory::Build(): found fullblocksize=1 and stridedblocksize=1 from strided maps. offset=0
   Nullspace factory (MueLu::NullspaceFactory)
   Build (MueLu::CoarseMapFactory)
    domainGIDOffset: 0 block size: 1 stridedBlockId: -1
   Column map is consistent with the row map, good.
   TentativePFactory : aggregates do not cross process boundaries
   Ptent size =  1111 x 371, nnz = 1111
   Ptent Load balancing info
   Ptent   # active processes: 1/1
   Ptent   # rows per proc   : avg = 1.11e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
   Ptent   #  nnz per proc   : avg = 1.11e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Eigenvalue estimate
   Calculating max eigenvalue estimate now (max iters = 10)
   Prolongator damping factor = 0.69 (1.33 / 1.95)
  Fused (I-omega*D^{-1} A)*Ptent
  P size =  1111 x 371, nnz = 1851
  P Load balancing info
  P   # active processes: 1/1
  P   # rows per proc   : avg = 1.11e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  P   #  nnz per proc   : avg = 1.85e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Transpose P (MueLu::TransPFactory)
  R size =  371 x 1111, nnz = 1851
  R Load balancing info
  R   # active processes: 1/1
  R   # rows per proc   : avg = 3.71e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  R   #  nnz per proc   : avg = 1.85e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Computing Ac (MueLu::RAPFactory)
  MxM: A x P
  MxM: R x (AP) (explicit)
  Ac size =  371 x 371, nnz = 1111
  Ac Load balancing info
  Ac   # active processes: 1/1
  Ac   # rows per proc   : avg = 3.71e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Ac   #  nnz per proc   : avg = 1.11e+03,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Setup Smoother (MueLu::IfpackSmoother{type = point relaxation stand-alone})
  IFPACK (Local SGS, sweeps=2, damping=1)
Using default factory (MueLu::AmalgamationFactory) for building 'UnAmalgamationInfo'.
Level 4
 Prolongator smoothing (MueLu::SaPFactory)
  Matrix filtering (MueLu::FilteredAFactory)
   Build (MueLu::CoalesceDropFactory)
    lightweight wrap = 1
    algorithm = "classical": threshold = 0.00, blocksize = 1
    Detected 0 Dirichlet nodes
    Number of dropped entries in unamalgamated matrix graph: 0/1111 (0.00%)
   Filtered matrix is not being constructed as no filtering is being done
  Build (MueLu::TentativePFactory)
   Build (MueLu::UncoupledAggregationFactory)
    Algo "Phase - (Dirichlet)"
     BuildAggregates (Phase - (Dirichlet))
       aggregated : 0 (phase), 0/371 [0.00%] (total)
       remaining  : 371
       aggregates : 0 (phase), 0 (total)
    Algo "Phase 1 (main)"
     BuildAggregates (Phase 1 (main))
       aggregated : 371 (phase), 371/371 [100.00%] (total)
       remaining  : 0
       aggregates : 124 (phase), 124 (total)
    Algo "Phase 2 (cleanup)"
     BuildAggregates (Phase 2 (cleanup))
       aggregated : 0 (phase), 371/371 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 124 (total)
    Algo "Phase 3 (emergency)"
     BuildAggregates (Phase 3 (emergency))
       aggregated : 0 (phase), 371/371 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 124 (total)
    Algo "Phase - (isolated)"
     BuildAggregates (Phase - (isolated))
       aggregated : 0 (phase), 371/371 [100.00%] (total)
       remaining  : 0
       aggregates : 0 (phase), 124 (total)
    "UC": MueLu::Aggregates{nGlobalAggregates = 124}
   Build (MueLu::AmalgamationFactory)
    AmalagamationFactory::Build(): found fullblocksize=1 and stridedblocksize=1 from strided maps. offset=0
   Nullspace factory (MueLu::NullspaceFactory)
   Build (MueLu::CoarseMapFactory)
    domainGIDOffset: 0 block size: 1 stridedBlockId: -1
   Column map is consistent with the row map, good.
   TentativePFactory : aggregates do not cross process boundaries
   Ptent size =  371 x 124, nnz = 371
   Ptent Load balancing info
   Ptent   # active processes: 1/1
   Ptent   # rows per proc   : avg = 3.71e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
   Ptent   #  nnz per proc   : avg = 3.71e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Eigenvalue estimate
   Calculating max eigenvalue estimate now (max iters = 10)
   Prolongator damping factor = 0.69 (1.33 / 1.94)
  Fused (I-omega*D^{-1} A)*Ptent
  P size =  371 x 124, nnz = 617
  P Load balancing info
  P   # active processes: 1/1
  P   # rows per proc   : avg = 3.71e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  P   #  nnz per proc   : avg = 6.17e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Transpose P (MueLu::TransPFactory)
  R size =  124 x 371, nnz = 617
  R Load balancing info
  R   # active processes: 1/1
  R   # rows per proc   : avg = 1.24e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  R   #  nnz per proc   : avg = 6.17e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Computing Ac (MueLu::RAPFactory)
  MxM: A x P
  MxM: R x (AP) (explicit)
  Ac size =  124 x 124, nnz = 370
  Ac Load balancing info
  Ac   # active processes: 1/1
  Ac   # rows per proc   : avg = 1.24e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
  Ac   #  nnz per proc   : avg = 3.70e+02,  dev =   0.0%,  min =   +0.0%,  max =   +0.0%
 Max coarse size (<= 128) achieved
 Setup Smoother (MueLu::IfpackSmoother{type = point relaxation stand-alone})
  IFPACK (Local GS, sweeps=25, damping=0.6)

--------------------------------------------------------------------------------
---                            Multigrid Summary                             ---
--------------------------------------------------------------------------------
Number of levels    = 5
Operator complexity = 1.49

matrix rows    nnz  nnz/row procs
A 0    9999  29995     3.00  1
A 1    3333   9997     3.00  1
A 2    1111   3331     3.00  1
A 3     371   1111     2.99  1
A 4     124    370     2.98  1

Smoother (level 0) both : IFPACK (Local SGS, sweeps=2, damping=1)

Smoother (level 1) both : IFPACK (Local SGS, sweeps=2, damping=1)

Smoother (level 2) both : IFPACK (Local SGS, sweeps=2, damping=1)

Smoother (level 3) both : IFPACK (Local SGS, sweeps=2, damping=1)

Smoother (level 4) pre  : IFPACK (Local GS, sweeps=25, damping=0.6)
Smoother (level 4) post : no smoother
