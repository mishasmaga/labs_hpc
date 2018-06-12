#!/bin/bash
#PBS -N SMAGA_MPI
#PBS -l nodes=2:ppn=2
#PBS -e mpi_logs/error.log
#PBS -o mpi_logs/output.log
#PBS -V

cd $PBS_O_WORKDIR
JOBS_TO_RUN=${1}
echo $JOBS_TO_RUN >> "totals"
JOBS_TO_RUN=$((JOBS_TO_RUN-1))
LEFT_CHILD=$((JOBS_TO_RUN / 2))
RIGHT_CHILD=$((JOBS_TO_RUN / 2 + JOBS_TO_RUN % 2))

mpirun -n 2 ./exe
echo $HOSTNAME >> "hostnames"
echo $LEFT_CHILD >> "lefts"
echo $RIGHT_CHILD >> "rights"

if [ "$LEFT_CHILD" -gt 0 ]; then
	qsub mpi.sh -v total=$((JOBS_TO_RUN / 2))
fi
#if [ "$RIGHT_CHILD" -gt 0 ]; then
#	qsub mpi.sh -v total=${RIGHT_CHILD}
#fi;
