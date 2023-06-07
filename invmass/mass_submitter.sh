#!/bin/tcsh

# submission script for mass scan jobs

# set name of job
setenv job_name "APEX_invariant_mass"

echo "submitted APEX invariant mass jobs as $job_name"

set username = "oliver"
set joblim = 800 # max limit for number of jobs
set waitime = 30 # time to wait between checking queue (seconds)

foreach line (`cat ../runlists/production_runlist_new.txt`)
#foreach line (`cat ../runlists/production_runlist_17Jan23_VDCGap0.txt`)
    setenv RUN_NUMBER $line
    echo $RUN_NUMBER

      while(`qselect -u $username | wc -l` >= $joblim)
	sleep $waitime
	 @ count++
	 
	 if ($count > 9) then
	    echo "waiting for jobs in queue to finish"
	    set nojobs = `qselect -u oliver | wc -l`
	    echo "Currently $nojobs in queue for $username"
	    set count = 0
	 endif
	 
    end
    
    qsub mass_job.sh
    #./mass_job.sh
    echo "Submitted mass Job for Run $RUN_NUMBER"
    sleep 1
end
    
