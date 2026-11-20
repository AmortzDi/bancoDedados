Trabalho prático desenvolvido para a disciplina de **Banco de Dados**.
Desenvolvidos pelos alunos:
Lilian Miranda Mendas GRR:20222549
Mozart Gustavo da Cruz Dias GRR:20213396

# fdtool - Ferramenta para Manipulação de Dependências Funcionais

O `fdtool` é uma ferramenta de linha de comando desenvolvida em linguagem C (padrão C11) para análise e manipulação de Dependências Funcionais (DFs) sobre esquemas relacionais com atributos definidos no intervalo `A..Z`.

---

## Organização dos Arquivos e Arquitetura

O projeto foi estruturado de forma modular, separando a representação dos dados, a leitura de arquivos e cada algoritmo exigido em unidades de compilação independentes:

* **`types.h` / `types.c`**: Define a representação fundamental do sistema (`ConjuntoAtributo` como `uint32_t` via máscara de bits) e as estruturas `DF` e `Schema`. Contém as funções de conversão entre strings e bitmasks (`attr_bit`, `str_to_ConjuntoAtributo`, `ConjuntoAtributo_to_str`) e operações auxiliares de manipulação de bits.
* **`lerarquivo.h` / `lerarquivo.c`**: Responsável pelo gerenciamento de memória dinâmica do vetor de dependências (`schema_init`, `schema_free`, `schema_add_df`) e pelo analisador sintático (`parse_fds_file`) que lê o arquivo `.fds`, ignora espaços em branco e extrai o universo $U$ e o conjunto $F$.
* **`closure.h` / `closure.c`**: Implementa o algoritmo de ponto fixo para o cálculo do fecho de atributos $X^+$ (`compute_closure`) e sua variante `compute_closure_skip`, que permite calcular o fecho ignorando uma dependência específica do conjunto.
* **`mincover.h` / `mincover.c`**: Implementa o cálculo da cobertura mínima (`compute_mincover`) em três etapas sequenciais: decomposição do lado direito (RHS) em atributos unitários, remoção de atributos estranhos no lado esquerdo (LHS) e eliminação de dependências funcionais redundantes.
* **`keys.h` / `keys.c`**: Gerencia a lista dinâmica de chaves (`KeyList`) e implementa a busca por todas as chaves candidatas mínimas (`find_candidate_keys`), utilizando a extração de atributos obrigatórios (núcleo) e busca combinatória com poda por superconjunto.
* **`normalform.h` / `normalform.c`**: Implementa a verificação das formas normais BCNF e 3FN (`check_normalform`), avaliando as dependências originais decompostas em RHS unitário com base nas superchaves e nos atributos primos obtidos pelas chaves candidatas.
* **`main.c`**: Ponto de entrada da aplicação. Realiza o processamento dos argumentos de linha de comando (`closure`, `mincover`, `keys`, `normalform`), coordena a leitura do esquema e invoca o módulo correspondente.
* **`Makefile`**: Automatiza a compilação com as flags exigidas (`-std=c11 -Wall -Wextra`) e gera o executável `fdtool`.
* **`exemplos/`**: Diretório contendo arquivos de entrada no formato `.fds` utilizados para validação e testes.

---

## Lógica e Decisões de Implementação

1. **Representação por Máscara de Bits (`uint32_t`)**: Como o domínio de atributos é limitado às 26 letras maiúsculas (`A..Z`), cada conjunto de atributos é armazenado em um inteiro sem sinal de 32 bits (`ConjuntoAtributo`), onde o bit $i$ corresponde à letra `'A' + i`. Operações de união (`|`), interseção (`&`), diferença (`& ~`) e verificação de subconjunto (`(A & B) == A`) são executadas em tempo constante $O(1)$, e a conversão para texto percorre os bits de 0 a 25, garantindo saída sempre ordenada alfabeticamente.
2. **Reutilização do Fecho ($X^+$)**: O algoritmo de fecho (`closure.c`) atua como núcleo para os demais módulos: valida superchaves em `keys` e `normalform`, verifica atributos estranhos no LHS durante o Passo 2 de `mincover` e detecta dependências redundantes no Passo 3 de `mincover` por meio de `compute_closure_skip`.
3. **Otimização na Busca de Chaves Candidatas**: Antes de testar combinações, o algoritmo identifica os atributos de $U$ que nunca ocorrem no lado direito de dependências não triviais, formando o núcleo obrigatório presente em toda chave. A busca ocorre por tamanho crescente de atributos opcionais com poda imediata sempre que um candidato contém uma chave já encontrada, assegurando a minimalidade sem gerar superchaves não mínimas.

---

## Compilação

Para compilar o projeto e gerar o executável `fdtool`, execute na raiz do diretório:

```bash
make
```

Para remover os arquivos objeto (`.o`) e o binário gerado:

```bash
make clean
```

---

## Formato do Arquivo de Entrada (`.fds`)

Os arquivos de entrada devem definir o universo de atributos `U` e o conjunto de dependências funcionais `F`, separados por vírgula ou ponto e vírgula (espaços e quebras de linha são ignorados):

```text
U = {A, B, C, D, E}
F = { A->BC, C->D, BD->E }
```

---

## Execução e Exemplos

### 1. Fecho de Atributos (`closure`)
Calcula e imprime $X^+$ em ordem alfabética:

```bash
./fdtool closure --fds exemplos/e1.fds --X A
```
Saída:
```text
ABCDE
```

```bash
./fdtool closure --fds exemplos/e1.fds --X BD
```
Saída:
```text
BDE
```

### 2. Cobertura Mínima (`mincover`)
Calcula uma cobertura mínima equivalente com RHS unitário, uma dependência por linha:

```bash
./fdtool mincover --fds exemplos/e1.fds
```
Saída:
```text
A->B
A->C
C->D
BD->E
```

### 3. Chaves Candidatas (`keys`)
Lista todas as chaves candidatas mínimas do esquema, uma por linha:

```bash
./fdtool keys --fds exemplos/e1.fds
```
Saída:
```text
A
```

### 4. Verificação de Formas Normais (`normalform`)
Verifica a conformidade com BCNF e 3FN, detalhando eventuais violações:

```bash
./fdtool normalform --fds exemplos/e3.fds
```
Saída:
```text
BCNF: VIOLATIONS
3NF: VIOLATIONS
VIOLATION BCNF: B->C (B not superkey)
VIOLATION 3NF: B->C (C not prime, B not superkey)
```