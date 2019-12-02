MDs = $(shell hx-srcs.sh)
CPPs = $(shell hx-files.sh $(MDs))
APPs = $(CPPs:.cpp=)

hx-run: $(MDs)
	@echo "HX"
	@hx
	@date >hx-run
	@make --no-print-directory ktk

ktk: $(CPPs)

clean:
	@echo "RM"
	@rm -f $(APPs) $(CPPs) hx-run
