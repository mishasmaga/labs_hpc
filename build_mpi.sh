#remove files
rm hostnames

#build mpi program
ml icc openmpi
mpicxx integral_mpi.cxx -std=c++11

#run mpi
 z=$(qsub -v total=100 mpi.sh -t 1-1 );  
 qsub -W depend=afterokarray:$z calc_total_mpi.pbs