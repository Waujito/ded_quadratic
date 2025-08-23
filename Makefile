CFLAGS = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -Iinclude

# -flto-odr-type-merging

CPPSRC = src/solver.cpp src/equations.cpp
CPPOBJ = $(CPPSRC:%.cpp=%.o)
CPPD = $(CPPSRC:%.cpp=%.d)
TESTSRC = test/equations.cpp
TESTOBJ = $(TESTSRC:%.cpp=%.o)

APP = solver
MAIN_FN_OBJ = src/solver.o
TEST_APP = test_solver

.PHONY: build clean run test document

build: $(APP)

run: build
	./$(APP)

$(CPPOBJ) $(TESTOBJ): %.o: %.cpp
	$(CXX) $(CFLAGS) -I./ -MP -MMD -c $< -o $@

$(APP): $(CPPOBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(CPPOBJ) -o $(APP)

$(TEST_APP): $(APP) $(TESTOBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(filter-out $(MAIN_FN_OBJ),$(CPPOBJ)) $(TESTOBJ) -lgtest_main -lgtest -o $(TEST_APP) 

test: $(TEST_APP) 
	./$(TEST_APP)

document: 
	doxygen doxygen.conf

clean:
	rm -f ./**/*.o
	rm -f ./**/*.d
	rm -f $(APP) $(TEST_APP)
	rm -rf doxygen

-include $(CPPD)
