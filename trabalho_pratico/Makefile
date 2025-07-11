# Nome do compilador
CXX = g++

# Nome do executável final
TARGET = programa

# Arquivos fonte
SRCS = main.cpp QuineMcCluskey.cpp aig.cpp

# Diretorio das regras de teste
TESTDIR = testFiles

# Arquivos de teste
TESTS_ENTRY = $(wildcard $(TESTDIR)/*.txt)

# Nome das regras de teste
TESTS = $(TESTS_ENTRY:.txt=)

# Objetos gerados a partir do fonte
OBJS = $(SRCS:.cpp=.o)


# Alvo padrão (executado com `make`)
all: $(TARGET)

run_tests : $(TESTS)

$(TESTDIR)/test% : $(TARGET)
	@echo
	./$(TARGET) < $@.txt

# Regra de linkagem
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS)

# Regra para gerar .o
%.o: %.cpp
	$(CXX)  -c $< -o $@

# Alvo para limpar arquivos gerados
clean:
	rm -f $(TARGET) $(OBJS)

# Evita que arquivos como "clean" virem arquivos reais
.PHONY: all clean run_tests &(TESTS)