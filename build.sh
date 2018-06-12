#!/bin/bash
num_steps=100
tasks_per_man=10
let num_managers=$num_steps/$tasks_per_man
int_from=-100
int_to=100
awk -v xmin="$int_from" -v xmax="$int_to" -v num_steps="$num_steps" 'BEGIN {dx=(xmax-xmin)/num_steps;  for (i = xmin; i < xmax; i+=dx) print i" "i+dx }' > ranges.txt
g++ integral.cxx -oexe

# res - results for individual tasks
# out - errors
# host - results of `hostname` of each task
# host2 - results of managers cat of host
rm -rf res out host host2
mkdir res out host host2
for m in $(seq 1 $num_managers); do
  mkdir "host/$m"
done

# calc separate pieces
pieces_job_id=$(qsub build_job.pbs -t 1-$num_steps)

# create managers for collecting hosts
mans=""
for m in $(seq 1 $num_managers); do
  args=""
  for t in $(seq 1 $tasks_per_man); do
    let i=($m-1)*$tasks_per_man+$t
    arg=":${pieces_job_id:0:-1}$i]"
    args=$args$arg
  done
  man=$(qsub -W depend=afterok$args -v man_id=$m build_job_man.pbs)
  #echo "man com qsub -W depend=afterok$args -v man_id=$m build_job_man.pbs"
  #echo "man $man"
  mans="$mans:$man"
done

# cat all host stats in ./stats
echo "mans $mans"
qsub -W depend=afterok$mans build_job_man2.pbs

# sum pieces
qsub -W depend=afterokarray:$pieces_job_id build_job_sum.pbs