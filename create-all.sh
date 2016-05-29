#!/bin/sh
./create_table livro id 6 titulo 30 editora 30 anoPublicacao 11 isbn 20
./create_table leitor id 6 nome 30 fone 20 endereco 40 cidade 40 estado 2
./create_table autor id 4 nome 20 sobrenome 20
./create_table autorDoLivro autorId 4 livroId 6 sequence 2
./create_table emprestimo livroId 6 leitorId 6 sequencia 4 dataRetirada 22 dataPrevistaParaDevolucao 22
