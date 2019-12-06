MDs = $(shell hx-srcs.sh)
CPPs = $(shell hx-files.sh $(MDs))
APPs = $(CPPs:.cpp=)
CXXFLAGS += -Wall -std=c++17

hx-run: $(MDs)
	@echo "HX"
	@hx
	@date >hx-run
	@make --no-print-directory ktk chess

ktk: ktk.cpp

chess: chess.cpp

clean:
	@echo "RM"
	@rm -f $(APPs) $(CPPs) hx-run
