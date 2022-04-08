CC=gcc
CFLAGS= -o -W -g
LDFLAGS= -lcholmod -lspqr -lsuitesparseconfig -lm -lstdc++ -fopenmp
OBJFILES = main.o funcoesCalculoEletrico_tf.o funcoesFluxoVarredura_tf.o funcoesLeitura_tf.o funcoesModCarga_tf.o funcoesTopologia_tf.o funcoesMatematicas_tf.o
TARGET = fp

all: $(TARGET)

$(TARGET): $(OBJFILES)
		$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~ 