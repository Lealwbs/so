# Shell Basico (TP0)

This is the repository with skeleton source code for DCC605's simple shell programming assignment. Please check the [Google Docs][1] with the full assignment.

[1]: https://docs.google.com/document/d/14cl6I8xLJD6dnet3fEEFEqtpZkK7TYS4hoq9nBqoKVg/edit?usp=sharing

Implementação de um shell simplificado baseado no xv6, desenvolvido como
trabalho prático da disciplina DCC605 (Sistemas Operacionais) da UFMG.

O shell suporta execução de comandos simples, redirecionamento de
entrada/saída (`<` e `>`), e pipes (`|`).

## Requisitos

- GCC
- Python 3 (para o script de correção)

## Comandos

| Comando      | Descrição                                              |
|--------------|--------------------------------------------------------|
| `make`       | Compila o shell                                        |
| `make run`   | Compila e executa o shell (use `Ctrl+D` para sair)     |
| `make test`  | Roda os testes automatizados                           |
| `make mark`  | Extrai o código dentro das marcações `MARK`            |
| `make clean` | Remove os arquivos gerados pela compilação             |

## Exemplos de uso
```sh
ls
ls > output.txt
cat < output.txt
cat output.txt | sort | uniq | wc -l
```

## Estrutura do projeto
```
tp0/
├── src/        # Código-fonte (sh.c)
├── bin/        # Binário compilado
├── tests/      # Scripts de teste e correção
└── docs/       # Especificação do trabalho
```

## Windows

Type in the terminal: `wsl`