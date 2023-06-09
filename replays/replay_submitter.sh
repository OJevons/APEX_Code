#!/bin/tcsh

# submission script for mass scan jobs

# set name of job
setenv job_name "APEX_replay"

echo "submitted APEX replay jobs as $job_name"

set username = "oliver"
set joblim = 800 # max limit for number of jobs
set waitime = 30 # time to wait between checking queue (seconds)

#setenv OUTPUT_DIR "/w/work2/home/oliver/APEX/replays/JWTrim50k/"
setenv OUTPUT_DIR "/w/work2/home/oliver/APEX/replays/50kReplay_LVDCgap0/"

#foreach line (`cat ../runlists/production_runlist_new.txt`)
foreach line (`cat ../runlists/production_runlist_17Jan23_VDCGap0.txt`)
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
    
    #qsub replay_job.sh
    ./replay_job.sh
    echo "Submitted replay Job for Run $RUN_NUMBER"
    sleep 1
end
