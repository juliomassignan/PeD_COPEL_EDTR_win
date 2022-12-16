all: fp 
    fp: main.o funcoesCalculoEletrico_tf.o funcoesFluxoVarredura_tf.o funcoesLeitura_tf.o funcoesModCarga_tf.o funcoesTopologia_tf.o funcoesMatematicas_tf.o funcoesAuxiliares.o funcoesLeituraDados.o funcoesRNP.o funcoesIntegracao_tf.o funcoesSetor.o funcoesInicializacao.o funcoesProblema.o funcoesNowCasting.o
	gcc -g -o fp main.o funcoesCalculoEletrico_tf.o funcoesFluxoVarredura_tf.o funcoesLeitura_tf.o funcoesModCarga_tf.o funcoesTopologia_tf.o funcoesMatematicas_tf.o funcoesAuxiliares.o funcoesLeituraDados.o funcoesRNP.o funcoesIntegracao_tf.o  funcoesSetor.o funcoesInicializacao.o funcoesProblema.o funcoesNowCasting.o -lm -lstdc++ -fopenmp
	
    funcoesCalculoEletrico_tf.o: funcoesCalculoEletrico_tf.c data_structures_tf.h funcoesCalculoEletrico_tf.h funcoesMatematicas_tf.h
	gcc -g -c funcoesCalculoEletrico_tf.c

    funcoesFluxoVarredura_tf.o: funcoesFluxoVarredura_tf.c data_structures_tf.h funcoesTopologia_tf.h funcoesFluxoVarredura_tf.h funcoesCalculoEletrico_tf.h funcoesMatematicas_tf.h
	gcc -g -c funcoesFluxoVarredura_tf.c
	
    funcoesLeitura_tf.o: funcoesLeitura_tf.c data_structures_tf.h funcoesLeitura_tf.h
	gcc -g -c funcoesLeitura_tf.c 
 
    funcoesModCarga_tf.o: funcoesModCarga_tf.c data_structures_tf.h data_structures_modcarga_tf.h funcoesLeitura_tf.h funcoesTopologia_tf.h
	gcc -g -c funcoesModCarga_tf.c

    funcoesTopologia_tf.o: funcoesTopologia_tf.c data_structures_tf.h funcoesMatematicas_tf.h 
	gcc -g -c funcoesTopologia_tf.c
 
    funcoesMatematicas_tf.o: funcoesMatematicas_tf.c data_structures_tf.h funcoesMatematicas_tf.h
	gcc -g -c funcoesMatematicas_tf.c
	
    funcoesIntegracao_tf.o: funcoesIntegracao_tf.c data_structures_tf.h funcoesTopologia_tf.h funcoesFluxoVarredura_tf.h funcoesCalculoEletrico_tf.h funcoesMatematicas_tf.h data_structures_modcarga_tf.h funcoesLeituraDados.h funcoesInicializacao.h funcoesAuxiliares.h funcoesRNP.h funcoesProblema.h funcoesSetor.h funcoesIntegracao_tf.h
	gcc -g -c funcoesIntegracao_tf.c
	
    funcoesNowCasting.o: funcoesNowCasting.c data_structures_tf.h funcoesLeitura_tf.h
	gcc -g -c funcoesNowCasting.c

    funcoesRNP.o: funcoesRNP.c data_structures.h funcoesRNP.h funcoesAuxiliares.h
	gcc -g -c funcoesRNP.c

    funcoesInicializacao.o: funcoesInicializacao.c data_structures.h funcoesInicializacao.h funcoesSetor.h
	gcc -g -c funcoesInicializacao.c
	
    funcoesSetor.o: funcoesSetor.c funcoesSetor.h data_structures.h funcoesInicializacao.h
	gcc -g -c funcoesSetor.c 
 
    funcoesLeituraDados.o: data_structures.h funcoesLeituraDados.h funcoesLeituraDados.c
	gcc -g -c funcoesLeituraDados.c

    funcoesAuxiliares.o: funcoesAuxiliares.h funcoesAuxiliares.c
	gcc -g -c funcoesAuxiliares.c
 
    funcoesModCarga.o: funcoesModCarga.c  data_structures.h data_structures_modcarga.h funcoesModCarga.h funcoesSetor.h funcoesLeitura_tf.h funcoesTopologia_tf.h funcoesIntegracao_tf.h funcoesFluxoVarredura_tf.h
	gcc -g -c funcoesModCarga.c
	
    funcoesProblema.o: funcoesProblema.c data_structures.h funcoesProblema.h funcoesAuxiliares.h funcoesRNP.h funcoesInicializacao.h
	gcc -g -c funcoesProblema.c
	
    main.o: main.c data_structures_tf.h funcoesLeitura_tf.h  funcoesTopologia_tf.h funcoesFluxoVarredura_tf.h  data_structures_modcarga_tf.h funcoesModCarga_tf.h
	gcc -g -c main.c 
 
clean: 
	$(RM) count *.o *~