# fdtool – Ferramenta para Manipulação de Dependências Funcionais

## Trabalho Prático – Banco de Dados

**Desenvolvido pelos alunos:**

- Lilian Miranda Mendas – GRR: 20222549
- Mozart Gustavo da Cruz Dias – GRR: 20213396

---

## 1. Apresentação do Projeto

O **fdtool** é uma ferramenta de linha de comando desenvolvida em linguagem C, seguindo o padrão C11, para realizar operações relacionadas às Dependências Funcionais (DFs) em bancos de dados relacionais.

O projeto tem como objetivo implementar algoritmos fundamentais para a análise de esquemas relacionais, contemplando quatro funcionalidades principais:

- Cálculo do fecho de atributos;
- Cálculo da cobertura mínima;
- Identificação de chaves candidatas;
- Verificação das formas normais BCNF e 3FN.

A ferramenta trabalha com atributos representados pelas letras maiúsculas de A até Z e utiliza arquivos com extensão `.fds` para receber as informações dos esquemas relacionais e suas respectivas dependências funcionais.

## 2. Organização dos Arquivos

O projeto foi estruturado em módulos independentes, permitindo separar a leitura dos dados, a representação dos atributos e a implementação dos algoritmos.

### 2.1. Arquivos de suporte

Esses arquivos são responsáveis por preparar e organizar as informações utilizadas pelos algoritmos principais.

| Arquivo | Descrição |
|---|---|
| `types.h` / `types.c` | Definem as estruturas utilizadas pelo programa, como conjuntos de atributos, dependências funcionais e esquemas relacionais. Também implementam operações de conversão e manipulação desses conjuntos. |
| `lerarquivo.h` / `lerarquivo.c` | Responsáveis pela leitura, interpretação e manipulação dos arquivos `.fds`. Realizam a identificação do universo de atributos e das dependências funcionais, além do gerenciamento de memória. |
| `main.c` | Contém o ponto de entrada da aplicação, interpreta os argumentos informados pelo usuário e direciona a execução para a funcionalidade solicitada. |
| `Makefile` | Automatiza o processo de compilação do projeto, utilizando as configurações exigidas para o padrão C11. |
| `exemplos/` | Diretório que contém arquivos de entrada no formato `.fds`, utilizados para testes e validação das funcionalidades. |

### 2.2. Módulos dos algoritmos principais

Esses módulos concentram a implementação das funcionalidades centrais exigidas no trabalho.

| Arquivo | Descrição |
|---|---|
| `closure.h` / `closure.c` | Implementam o cálculo do fecho de atributos (X+), identificando todos os atributos que podem ser determinados a partir de um conjunto inicial. |
| `mincover.h` / `mincover.c` | Implementam o cálculo da cobertura mínima, realizando a decomposição do lado direito, a remoção de atributos desnecessários do lado esquerdo e a eliminação de dependências redundantes. |
| `keys.h` / `keys.c` | Implementam a identificação de todas as chaves candidatas mínimas do esquema relacional. |
| `normalform.h` / `normalform.c` | Implementam a verificação das formas normais BCNF e 3FN, identificando possíveis violações das regras de normalização. |

## 3. Lógica e Decisões de Implementação

### 3.1. Representação dos Conjuntos de Atributos

Como o projeto trabalha com atributos representados pelas 26 letras maiúsculas do alfabeto (A até Z), foi utilizada uma máscara de bits (`uint32_t`) para armazenar os conjuntos de atributos.

Nessa representação, cada bit corresponde a uma letra. Quando o bit está ativado, significa que o atributo correspondente pertence ao conjunto.

Essa abordagem permite realizar operações como união, interseção, diferença e verificação de subconjuntos de maneira eficiente.

A conversão dos conjuntos para texto percorre os atributos em ordem alfabética, garantindo uma saída organizada.

### 3.2. Cálculo do Fecho de Atributos

O cálculo do fecho de atributos (X+) é uma das operações fundamentais do projeto.

O algoritmo parte de um conjunto inicial de atributos e verifica repetidamente quais dependências funcionais podem ser aplicadas. Quando o lado esquerdo de uma dependência está contido no conjunto atual, os atributos do lado direito são adicionados.

Esse processo continua até que nenhum novo atributo possa ser incluído.

O resultado representa todos os atributos que podem ser determinados a partir do conjunto inicial, considerando as dependências funcionais do esquema.

### 3.3. Cálculo da Cobertura Mínima

A cobertura mínima tem como objetivo simplificar um conjunto de dependências funcionais, mantendo sua equivalência com o conjunto original.

A implementação é realizada em três etapas:

1. **Decomposição do lado direito:** cada dependência funcional é dividida para que tenha apenas um atributo no lado direito.
2. **Remoção de atributos desnecessários:** são identificados e removidos os atributos do lado esquerdo que não são necessários para manter a dependência.
3. **Eliminação de dependências redundantes:** são removidas as dependências que podem ser obtidas a partir das demais.

Ao final, o programa apresenta um conjunto simplificado de dependências funcionais.

### 3.4. Identificação de Chaves Candidatas

O algoritmo de identificação de chaves candidatas procura os menores conjuntos de atributos capazes de determinar todos os atributos do esquema relacional.

Inicialmente, são identificados os atributos que não aparecem no lado direito das dependências funcionais. Esses atributos precisam estar presentes em todas as chaves candidatas.

Em seguida, o algoritmo testa diferentes combinações de atributos, começando pelas menores. Quando uma combinação contém uma chave candidata já identificada, ela é descartada, pois não representa uma nova chave mínima.

Esse procedimento permite encontrar todas as chaves candidatas do esquema.

### 3.5. Verificação das Formas Normais BCNF e 3FN

A ferramenta também realiza a verificação das formas normais Boyce-Codd (BCNF) e Terceira Forma Normal (3FN).

Para isso, analisa as dependências funcionais do esquema e verifica se os atributos do lado esquerdo representam superchaves e se os atributos do lado direito atendem aos critérios exigidos pela 3FN.

Na verificação da 3FN, também são considerados os atributos primos, ou seja, aqueles que fazem parte de pelo menos uma chave candidata.

Caso uma dependência viole alguma das regras, o programa apresenta a dependência responsável e informa o motivo da violação.

### 3.6. Reutilização do Algoritmo de Fecho

O algoritmo de fecho de atributos também é utilizado como base para outras funcionalidades do projeto.

Sua implementação é aproveitada nas seguintes operações:

- Na identificação de chaves candidatas, para verificar se um conjunto determina todos os atributos do esquema.
- Na cobertura mínima, para identificar atributos desnecessários e dependências redundantes.
- Na verificação das formas normais, para determinar se o lado esquerdo de uma dependência representa uma superchave.

Essa reutilização evita a repetição de implementações e mantém o código organizado.

## 4. Compilação

Para compilar o projeto, acesse o diretório raiz onde estão os arquivos do código-fonte e execute:

```bash
make
```

O comando utiliza as configurações definidas no `Makefile` e gera o executável `fdtool`.

Para remover os arquivos objeto (`.o`) e o executável gerado, utilize:

```bash
make clean
```

## 5. Formato do Arquivo de Entrada

A ferramenta utiliza arquivos com extensão `.fds` para representar os esquemas relacionais e suas dependências funcionais.

O arquivo deve conter duas informações principais:

- **U:** representa o universo de atributos do esquema relacional.
- **F:** representa o conjunto de dependências funcionais.

Exemplo de arquivo `.fds`:

```text
U = {A, B, C, D, E}
F = { A->BC, C->D, BD->E }
```

O módulo de leitura interpreta essas informações e organiza os dados em estruturas utilizadas pelos algoritmos.

## 6. Execução da Ferramenta

A ferramenta possui quatro funcionalidades principais, acessadas por meio de comandos no terminal.

### 6.1. Fecho de Atributos (`closure`)

Calcula o fecho de um conjunto de atributos, identificando todos os atributos que podem ser determinados a partir dele.

**Exemplo 1:**

```bash
./fdtool closure --fds exemplos/e1.fds --X A
```

Saída esperada:

```text
ABCDE
```

**Exemplo 2:**

```bash
./fdtool closure --fds exemplos/e1.fds --X BD
```

Saída esperada:

```text
BDE
```

### 6.2. Cobertura Mínima (`mincover`)

Calcula uma cobertura mínima equivalente ao conjunto original de dependências funcionais.

O resultado apresenta uma dependência por linha, com apenas um atributo no lado direito.

**Comando:**

```bash
./fdtool mincover --fds exemplos/e1.fds
```

Saída esperada:

```text
A->B
A->C
C->D
BD->E
```

### 6.3. Chaves Candidatas (`keys`)

Identifica e apresenta todas as chaves candidatas mínimas do esquema relacional.

**Comando:**

```bash
./fdtool keys --fds exemplos/e1.fds
```

Saída esperada:

```text
A
```

### 6.4. Verificação das Formas Normais (`normalform`)

Verifica se o esquema relacional atende às regras das formas normais BCNF e Terceira Forma Normal (3FN).

Caso existam violações, a ferramenta apresenta as dependências funcionais responsáveis e explica o motivo da violação.

**Comando:**

```bash
./fdtool normalform --fds exemplos/e3.fds
```

Saída esperada:

```text
BCNF: VIOLATIONS
3NF: VIOLATIONS
VIOLATION BCNF: B->C (B not superkey)
VIOLATION 3NF: B->C (C not prime, B not superkey)
```

## 7. Utilização de Inteligência Artificial

Durante o desenvolvimento do projeto, foi utilizada uma ferramenta de Inteligência Artificial como recurso de apoio em diferentes etapas da implementação e da documentação.

Sua utilização ocorreu principalmente nas seguintes atividades:

- **Estruturação da documentação:** apoio na organização das seções, revisão da escrita e melhoria da apresentação do arquivo `README.md`, buscando tornar a documentação mais clara e compreensível.

- **Desenvolvimento do módulo de leitura de arquivos:** apoio na organização e implementação do módulo responsável pela leitura, interpretação e manipulação das informações presentes nos arquivos `.fds`, incluindo a identificação do universo de atributos e das dependências funcionais.

- **Representação dos conjuntos de atributos com Bitmap:** apoio na análise e utilização de máscaras de bits (`uint32_t`) para representar os conjuntos de atributos. Essa abordagem permite realizar operações como união, interseção, diferença e verificação de subconjuntos por meio de operações bit a bit, tornando a manipulação dos atributos mais eficiente.

A Inteligência Artificial foi utilizada como ferramenta complementar de apoio, enquanto o foco principal do trabalho permaneceu na implementação dos algoritmos de cálculo do fecho de atributos, cobertura mínima, identificação de chaves candidatas e verificação das formas normais BCNF e 3FN.


## 8. Considerações Finais

O desenvolvimento do `fdtool` permitiu implementar e integrar diferentes operações relacionadas às dependências funcionais em bancos de dados relacionais.

A organização do projeto em módulos independentes facilitou a separação das responsabilidades entre a leitura dos arquivos, a representação dos dados e a execução dos algoritmos.

As funcionalidades desenvolvidas permitem realizar análises importantes de esquemas relacionais, abrangendo o cálculo de fechos, a obtenção de coberturas mínimas, a identificação de chaves candidatas e a verificação das formas normais BCNF e 3FN.

Dessa forma, o projeto demonstra a aplicação prática dos conceitos estudados na disciplina de Banco de Dados, utilizando a linguagem C.
