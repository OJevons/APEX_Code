# APEX Analysis Code
====================

These are scripts for running replay jobs and post-processing analysis on the data from the APEX experiment, hosted at Jefferson Lab Hall A, in 2019. For running these, the [APEX branch of the Hall A analyzer](https://github.com/johnaldhino/analyzer/tree/APEX) is required. Before running any of the scripts located here, edit the `.apex_setup.csh` file to point to your ROOT and analyzer installations.

/replay/
--------

This folder contains the relevant files for running both single run replays, and scripts to run multiple replay jobs on a batch system.

### Single replays
The files listed as `replay_apex*.C` can be used for replay jobs on the raw data files for a single APEX run. In order to run these, first `def_apex.h` must be modified to point to the location you have `apex_<RunNumber>.dat` files stored in - within the `PATHS[]` array. With the filepaths set, use the following command to run a replay (replace the file name with the name of the relevant script you wish to use).

```
analyzer replay_apex.C
```

When the replay script is running, you will be prompted for the run number to be analyzed, and then the number of events you wish to replay. The output ROOT tree will contain the branches listed in the `.odef` file listed in the replay scipt, using the `.cuts` file for data selection.

## Batch replays
Batch running of mass replays can be performed using the `replay_submitter.sh` script, which calls and runs the `replay_job.sh` script for each file in the chosen run list. The submission script needs to be modified to be relevant batch system being used. The job script makes a temporary directory, into which the relevant data files are copied, and the output `.root` file is stored. The batch jobs are controlled by `def_apex_batch.h`.

/invmass/
---------

The scripts within this directory take the replayed APEX files, applies geometric cuts, and reconstructs the invariant mass of coincidence electron-positron pairs. These also come with submission scripts for running invariant mass reconstruction jobs on a batch system (`mass_submitter.sh` and `mass_job.sh`). The scripts save the invariant mass spectra both as a ROOT histogram, and as a text file; one for each run.

Once all runs have been turned into invariant mass spectra, run `combmass.cxx` on the directory containing the output of the invariant mass jobs. This script combines the spectra from each run, rebins them from bins of width 5 MeV to bins of width 15 MeV, and takes only a portion of the data if you wish to 'blind' the spectra (by default, only 10% of the data is taken for a blinded spectrum).

/peaksearch/
------------

Within this directory are scripts to run peak searches on the combined invariant mass spectra. These peak searches are performed using [hpstr](https://github.com/johnaldhino/hpstr), the reconstruction toolkit created for the Heavy Photon Search in Hall B. Once hpstr is set up, the peak searches can be ran with the following,

```
python mass_submitter.py </path-to-parameter-file.csv>
```

, where `python` is a command to run a python3 installation.

/plotting/
----------

The scripts contained here are used to plot the output of the hpstr peak searches. First, you need to run `PeakSearchReadable.cxx` to turn the ROOT files that comprise the peak search output, and combine them all into a single output ROOT file, which contains separate histograms for each A' mass hypothesis.

With the peak search outputs condensed, run `plotresults.cxx` to extract plots for the chi2, p values, signal upper limits and sensitivities calculated for the used peak search parameters.

/utils/
-------

This folder contains scripts for checking the status of batch jobs, comparing the existing list of files to what would be expected from the list of runs or peak search parameters.

