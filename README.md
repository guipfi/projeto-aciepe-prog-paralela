# projeto-aciepe-prog-paralela
Projeto final - ACIEPE: Programação paralela: das threads aos FPGAs - uma introdução ao tema.

Há 2 versões sequencias: token_sequential.cpp e token_sequential2.cpp e 2 versões paralelas token_parallel_for.cpp e token_parallel_sections.cpp

Dentro do código há uma função que gera caracteres randômicos, há a possibilidade de alterar o tamanho utilizando alterando o define tamanho presente nos códigos. Depois basta compilá-lo usando c++11 e o openMP.

No mac:
g++ -std=c++11 -Xpreprocessor -fopenmp -lomp token_parallel_sections.cpp -o token

No linux:
g++ -std=c++11 -fopenmp token_parallel_sections.cpp -o token
