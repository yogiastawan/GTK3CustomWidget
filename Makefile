release:
	@echo -e "=>\e[94mCreate \"\e[4mGTK 3 Custom Widget\e[24m\" executable to release\e[0m"
	@mkdir -p build-release
	@cd src; make -f source.mk release

debug:
	@mkdir -p build-debug
	@echo -e "=>\e[94mCreate \"\e[4mGTK 3 Custom Widget\e[24m\" executable to debug\e[0m"
	@cd src; make -f source.mk debug

clean:	
	@cd src; make -f source.mk clean
	@rm -rf build-debug
	@rm -rf build-release