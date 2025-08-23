CFLAGS := -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr -Iinclude
CXX := g++

export CXX CFLAGS

# -flto-odr-type-merging

LIBSRC := src/equations.cpp
LIBOBJ := $(LIBSRC:%.cpp=%.o)
STATIC_LIB := equations.a

TESTSRC := test/equations.cpp
TESTOBJ := $(TESTSRC:%.cpp=%.o)

CPPSRC := src/solver.cpp
CPPOBJ := $(CPPSRC:%.cpp=%.o)
CPPD := $(CPPSRC:%.cpp=%.d)

APP := solver
TEST_LIB_APP := test_equations

.PHONY: build clean run test document build_test

build: $(APP)

run: build
	./$(APP)

$(CPPOBJ) $(TESTOBJ) $(LIBOBJ): %.o: %.cpp
	$(CXX) $(CFLAGS) -I./ -MP -MMD -c $< -o $@

$(STATIC_LIB): $(LIBOBJ)
	ar rvs $@ $<

$(TEST_LIB_APP): $(STATIC_LIB) $(TESTOBJ)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(TESTOBJ) $(STATIC_LIB) -lgtest_main -lgtest -o $(TEST_LIB_APP) 


build_test: $(TEST_LIB_APP)

test: build_test
	./$(TEST_LIB_APP)

$(APP): $(CPPOBJ) $(STATIC_LIB)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(CPPOBJ) $(STATIC_LIB) -o $(APP)

document: 
	doxygen doxygen.conf

clean:
	rm -f *.o *.d *.a ./**/*.o ./**/*.d ./**/*.a
	rm -f $(APP) $(TEST_LIB_APP) $(STATIC_LIB)
	rm -rf doxygen

-include $(CPPD)
