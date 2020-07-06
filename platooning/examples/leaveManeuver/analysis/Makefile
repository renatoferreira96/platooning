# tool for indexing vec files
SCAVETOOL = scavetool
# scripts location
SCRIPTDIR = .
# results location
RESDIR = ../results
# script for merging
MERGESCRIPT = $(SCRIPTDIR)/merge.R

# match all .vec files for the LeaveManeuver config
LEAVEMANEUVER      = $(wildcard $(RESDIR)/LeaveManeuver*.vec)
# change suffix from .vec to .Rdata and add the leave prefix
LEAVEMANEUVER_DATA = $(LEAVEMANEUVER:$(RESDIR)/%.vec=$(RESDIR)/leave.%.Rdata)

# vector index files and Rdata files
VCI = $(VECTOR:%.vec=%.vci)
RDATA = $(VECTOR:%.vec=%.Rdata)
CSV = $(VECTOR:%.vec=%.csv)

# all make targets
all: LeaveManeuver.Rdata 

# to make LeaveManeuver.Rdata we need to merge all files starting with leave.LeaveManeuver
# before this, check that all LEAVEMANEUVER_DATA files have been processed
$(RESDIR)/LeaveManeuver.Rdata: $(LEAVEMANEUVER_DATA)
	Rscript $(MERGESCRIPT) $(RESDIR)/ leave.LeaveManeuver $(notdir $@) map-config default Rdata
LeaveManeuver.Rdata: $(RESDIR)/LeaveManeuver.Rdata

# to make all leave.*.Rdata files we need to run the generic parser
leave.%.Rdata: %.vec %.vci
	Rscript generic-parser.R $< map-config default leave Rdata

# if vec files are not indexed, index them
%.vci : %.vec
	$(SCAVETOOL) index $<

# helper to print variable values. e.g.: make print-DELAY_DATA
print-%:
	@echo '$*=$($*)'

# every intermediate file is kept instead of being automatically deleted.
# .vci files are detected as intermediate and thus cancelled when the make
# command terminates. however, such files can still be needed by other
# targets, so keep them instead of re-doing indexing
.SECONDARY :
