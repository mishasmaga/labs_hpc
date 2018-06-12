 z=$(qsub ./build_job_individual.pbs -t 1); 
 
 qsub -W depend=afterokarray:$z build_job_stat.pbs