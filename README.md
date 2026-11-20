
# fdtool – Ferramenta para Manipulação de Dependências Funcionais

## Trabalho Prático – Banco de Dados

**Desenvolvido pelos alunos:**

- Lilian Miranda Mendas – GRR: 20222549
- Mozart Gustavo da Cruz Dias – GRR: 20213396

---

## 1. Apresentação do Projeto

O **fdtool** é uma ferramenta de linha de comando desenvolvida em linguagem C, seguindo o padrão C11, para realizar operações relacionadas às Dependências Funcionais (DFs) em bancos de dados relacionais.

A ferramenta permite analisar esquemas relacionais compostos por atributos representados pelas letras maiúsculas de A até Z.

Entre suas principais funcionalidades estão:

- Cálculo do fecho de atributos;
- Obtenção da cobertura mínima;
- Identificação de chaves candidatas;
- Verificação das formas normais BCNF e 3FN.

O objetivo do projeto é implementar esses conceitos por meio de algoritmos que auxiliam na análise e na organização de estruturas de bancos de dados relacionais.

## 2. Organização dos Arquivos

O projeto foi dividido em diferentes arquivos, cada um responsável por uma parte específica do funcionamento da ferramenta. Essa organização facilita a compreensão, a manutenção e a reutilização do código.

| Arquivo | Descrição |
|---|---|
| `types.h` / `types.c` | Definem as estruturas básicas do sistema, como conjuntos de atributos, dependências funcionais e esquemas relacionais. Também implementam operações de conversão e manipulação desses conjuntos. |
| `lerarquivo.h` / `lerarquivo.c` | Responsáveis pela leitura e interpretação dos arquivos `.fds`, identificando o universo de atributos e as dependências funcionais. Também realizam o gerenciamento de memória. |
| `closure.h` / `closure.c` | Implementam o algoritmo de cálculo do fecho de atributos (X+), identificando os atributos que podem ser determinados a partir de um conjunto inicial. |
| `mincover.h` / `mincover.c` | Implementam o cálculo da cobertura mínima, realizando a decomposição do lado direito, a remoção de atributos desnecessários e a eliminação de dependências redundantes. |
| `keys.h` / `keys.c` | Responsáveis pela identificação de todas as chaves candidatas mínimas do esquema relacional. |
| `normalform.h` / `normalform.c` | Implementam a verificação das formas normais BCNF e 3FN, identificando possíveis violações das regras de normalização. |
| `main.c` | Contém o ponto de entrada da aplicação, realiza a leitura dos argumentos informados pelo usuário e direciona a execução para a funcionalidade solicitada. |
| `Makefile` | Automatiza o processo de compilação do projeto, utilizando as opções exigidas para o padrão C11. |
| `exemplos/` | Contém arquivos de entrada no formato `.fds`, utilizados para executar testes e validar as funcionalidades da ferramenta. |

## 3. Lógica e Decisões de Implementação

### 3.1. Representação dos Conjuntos de Atributos

Como o projeto trabalha com atributos representados pelas 26 letras maiúsculas do alfabeto (A até Z), foi utilizada uma máscara de bits (`uint32_t`) para armazenar os conjuntos de atributos.

Nessa representação, cada bit corresponde a uma letra. Quando o bit está ativado, significa que o atributo correspondente pertence ao conjunto.

Essa abordagem permite realizar operações como união, interseção, diferença e verificação de subconjuntos de maneira eficiente.

Além disso, a conversão dos conjuntos para texto percorre os atributos em ordem alfabética, garantindo uma saída organizada.

### 3.2. Reutilização do Algoritmo de Fecho

O cálculo do fecho de atributos (X+) é uma das principais operações da ferramenta, pois também é utilizado por outros módulos do projeto.

Sua implementação é aproveitada nas seguintes funcionalidades:

- **Chaves candidatas:** verifica se um conjunto de atributos determina todos os atributos do esquema.
- **Cobertura mínima:** auxilia na identificação de atributos desnecessários e dependências redundantes.
- **Formas normais:** permite verificar se o lado esquerdo de uma dependência funcional representa uma superchave.

Essa reutilização evita a repetição de algoritmos e mantém a implementação organizada.

### 3.3. Busca por Chaves Candidatas

Para encontrar as chaves candidatas, o programa identifica inicialmente os atributos que não aparecem no lado direito das dependências funcionais. Esses atributos são obrigatórios em todas as chaves candidatas.

Depois dessa identificação, o algoritmo testa diferentes combinações de atributos, começando pelas menores.

Quando uma combinação já contém uma chave candidata encontrada anteriormente, ela é descartada, pois não pode ser uma chave mínima.

Esse processo reduz a quantidade de combinações analisadas e permite identificar todas as chaves candidatas mínimas do esquema.

## 4. Compilação

Para compilar o projeto, é necessário acessar o diretório raiz onde estão os arquivos do código-fonte e executar:

```bash
make
```

Esse comando utiliza as configurações definidas no `Makefile` e gera o executável `fdtool`.

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

Os atributos podem ser separados por vírgulas ou espaços. As dependências funcionais podem ser separadas por vírgulas ou ponto e vírgula, conforme o formato aceito pelo analisador de entrada.

## 6. Execução da Ferramenta

A ferramenta possui quatro funcionalidades principais, acessadas por meio de comandos no terminal.

### 6.1. Fecho de Atributos (`closure`)

Calcula o fecho de um conjunto de atributos, identificando todos os atributos que podem ser determinados a partir dele, considerando as dependências funcionais do esquema.

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

Uma chave candidata é um conjunto mínimo de atributos capaz de determinar todos os atributos do esquema.

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

## 7. Considerações Finais

O desenvolvimento do `fdtool` permitiu implementar e integrar diferentes operações relacionadas às dependências funcionais em bancos de dados relacionais.

A divisão do projeto em módulos independentes facilitou a organização do código e a reutilização das implementações. Além disso, a ferramenta reúne funcionalidades importantes para a análise de esquemas relacionais, como cálculo de fechos, obtenção de coberturas mínimas, identificação de chaves candidatas e verificação das formas normais.

Dessa forma, o projeto demonstra a aplicação prática dos conceitos estudados na disciplina de Banco de Dados, utilizando a linguagem C.
