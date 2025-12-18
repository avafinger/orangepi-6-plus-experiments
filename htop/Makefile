# Makefile for htop

include Rules.mak

all: _all
	@echo ""
	@echo "+-----------------------------------------------------------------------------+"
	@echo "| License:                                                                    |"
	@echo "+-----------------------------------------------------------------------------+"
	@echo ""
	@echo "Thank you."
	@echo ""

_all: _htop

_htop:
	@echo "Building htop..."
	@cd src;$(MAKE) -f Makefile
	@echo "\nBuilding finished!"

clean:
	@echo "\nCleaning..."
	@cd src;$(MAKE) -f Makefile clean
	@echo "\nClean finished!"

