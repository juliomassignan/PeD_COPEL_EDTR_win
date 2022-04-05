CC=gcc
CFLAGS= -o -W -g
LDFLAGS= -lcholmod -lspqr -lsuitesparseconfig -lm -lstdc++ -fopenmp
OBJFILES = main.o funcoesCalculoEletrico.o funcoesFluxoVarredura.o funcoesLeitura.o funcoesModCarga.o funcoesTopologia.o funcoesMatematicas.o
TARGET = fp

all: $(TARGET)

$(TARGET): $(OBJFILES)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~ 