# b-tree

O algoritmo implementado neste projeto tem como objetivo implementar uma Árvore B e implementar métodos de inserção, remoção e travessia. A ordem escolhida para implementar a árvore foi 03.

### Funcionamento do algoritmo

O algoritmo espera um arquivo de entrada com as operações desejadas no seguinte formato genérico:
```sh
<operacao> <chave>
```
As operações disponíveis são: __insere__, __remove__, __imprime__ e __fim__ e a __chave__ assume valores do tipo __inteiro__.

Exemplo funcional de arquivo de entrada:
```sh
insere 30
insere 40
insere 50
insere 60
remove 20
remove 40
remove 30
imprime
insere 20
insere 40
insere 15
remove 15
imprime
fim
```

### Compilando o algoritmo

Para compilar o código, deve-se utilizar o seguinte comando:
```sh
gcc d_12690692.c <nomedoexecutavel>
```

### Executando o algoritmo

Para executando o código, deve-se utilizar o seguinte comando:
```sh
<nomedoexecutavel> <nomearquivoentrada.txt> <nomearquivosaida.txt>
```


