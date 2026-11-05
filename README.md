# Escultor 3D

Aplicação de modelagem tridimensional por voxels desenvolvida em C++ e Qt. O projeto permite criar e editar esculturas 3D utilizando formas geométricas, personalizar cores, visualizar modelos e exportar esculturas para o formato OFF.

## Sobre o projeto

O Escultor 3D utiliza uma matriz tridimensional de voxels (*volumetric pixels*) para representar objetos. Cada voxel pode ser ativado, removido e colorido, possibilitando a construção de modelos tridimensionais a partir de unidades volumétricas.

O projeto foi desenvolvido como atividade acadêmica, aplicando conceitos de programação orientada a objetos, alocação dinâmica de memória, geometria computacional e desenvolvimento de interfaces gráficas.

## Funcionalidades

- Criação de matrizes tridimensionais com dimensões personalizáveis.
- Adição e remoção de voxels individuais.
- Criação e remoção de caixas, esferas e elipsoides.
- Personalização das cores das formas geométricas.
- Interface gráfica interativa desenvolvida com Qt.
- Seleção de ferramentas e configuração de coordenadas e dimensões.
- Carregamento de exemplos de esculturas.
- Interpretação de comandos textuais para construção de modelos.
- Exportação de esculturas para arquivos OFF.
- Compatibilidade com visualizadores de modelos 3D, como o MeshLab.

## Tecnologias utilizadas

- **C++**
- **Qt 6**
- **Qt Creator**
- **CMake**
- **STL (Standard Template Library)**
- **Doxygen**, para geração da documentação do código.
- **OFF (Object File Format)**, para exportação de modelos tridimensionais.

## Ferramentas de modelagem

| Ferramenta | Funcionalidade |
|---|---|
| `PutVoxel` | Adiciona um voxel individual. |
| `CutVoxel` | Remove um voxel individual. |
| `PutBox` | Cria uma caixa preenchida. |
| `CutBox` | Remove voxels de uma região em formato de caixa. |
| `PutSphere` | Cria uma esfera preenchida. |
| `CutSphere` | Remove voxels de uma região esférica. |
| `PutEllipsoid` | Cria um elipsoide preenchido. |
| `CutEllipsoid` | Remove voxels de uma região elipsoidal. |

## Como executar

### Pré-requisitos

- Qt 6 com os componentes necessários para compilação.
- Qt Creator ou outra IDE compatível.
- Compilador C++ compatível com a versão do Qt.
- CMake.

### Executando pelo Qt Creator

1. Clone este repositório:

   ```bash
   git clone https://github.com/natanmorais23/Escultor3D.git
   ```

2. Abra o Qt Creator.
3. Selecione **Open File or Project**.
4. Abra o arquivo `CMakeLists.txt` localizado na raiz do projeto.
5. Configure um kit de compilação compatível com Qt 6.
6. Compile e execute a aplicação.

O projeto gera dois executáveis:

- **Escultor3D**: a interface gráfica.
- **Exemplo**: programa de console que testa todos os métodos do `Sculptor`, todas as classes derivadas de `FiguraGeometrica` e a leitura de arquivos de desenho. Para escolher qual executar, use o seletor de alvo do Qt Creator (ícone de monitor, no canto inferior esquerdo).

## Roteiro de uso da interface

1. **Criar a matriz:** em *Dimensões da matriz*, informe os tamanhos X, Y e Z e clique em **Criar matriz**. A matriz começa vazia.
2. **Escolher a ferramenta:** em *Forma*, selecione Voxel, Caixa, Esfera, Elipsoide ou uma das opções *Apagar*.
3. **Escolher a cor:** clique em **Escolher cor**. As ferramentas de apagar ignoram a cor.
4. **Ajustar os parâmetros:**
   - *Posição X/Y/Z*: ponto onde a forma será aplicada. Na caixa, é o canto inicial; na esfera e no elipsoide, é o centro.
   - *Raio / dimensão X*: raio da esfera, semieixo X do elipsoide ou tamanho da caixa em X.
   - *Dimensão Y* e *Dimensão Z*: semieixos do elipsoide ou tamanho da caixa em Y e Z.
5. **Desenhar com o mouse:** a *Posição Z* define o **plano de desenho**, mostrado no canto superior esquerdo da área de visualização. Clique ou arraste com o **botão esquerdo** sobre a vista: a forma escolhida é aplicada em cada voxel por onde o cursor passa. Para desenhar em outra altura, mude a *Posição Z*.
6. **Aplicar pelos campos:** o botão **Aplicar forma** aplica a ferramenta exatamente nas coordenadas digitadas.
7. **Abrir arquivo de desenho:** carrega um arquivo `.txt` (formato descrito abaixo) e substitui a escultura atual. Há um exemplo em [`exemplos/boneco_de_neve.txt`](exemplos/boneco_de_neve.txt).
8. **Carregar exemplo:** desenha uma Pokébola na matriz atual.
9. **Exportar OFF:** salva a escultura em um arquivo `.off`.

## Formato do arquivo de desenho

Uma instrução por linha. Linhas vazias ou iniciadas por `#` são ignoradas. A primeira instrução precisa ser `dim`. As cores vão de 0 a 1.

```text
dim nx ny nz
putvoxel x y z r g b a
cutvoxel x y z
putbox x0 x1 y0 y1 z0 z1 r g b a
cutbox x0 x1 y0 y1 z0 z1
putsphere xc yc zc raio r g b a
cutsphere xc yc zc raio
putellipsoid xc yc zc rx ry rz r g b a
cutellipsoid xc yc zc rx ry rz
```

## Programa-exemplo

O executável **Exemplo** roda três etapas e imprime `[OK]` ou `[FALHA]` para cada verificação:

1. Usa diretamente todos os métodos do `Sculptor` e gera `exemplo_sculptor.off`.
2. Usa as 8 classes concretas por meio de um vetor de `FiguraGeometrica` (polimorfismo) e gera `exemplo_figuras.off`.
3. Lê `exemplos/boneco_de_neve.txt` (ou o arquivo passado como argumento) e gera `exemplo_arquivo.off`.

Os arquivos `.off` são gravados na pasta de build e podem ser abertos no MeshLab.

> **Observação:** a classe `Sculptor` e o struct `Voxel` mantêm os membros e métodos originais. Foram acrescentados somente os métodos de leitura `getNx()`, `getNy()`, `getNz()` e `getVoxel()`, que a interface gráfica e o programa-exemplo usam para desenhar e conferir a escultura.

## Exportação de modelos 3D

A aplicação permite exportar esculturas para o formato OFF, que representa vértices e faces de objetos tridimensionais.

Para visualizar uma escultura exportada:

1. Crie ou carregue um modelo na aplicação.
2. Clique em **Exportar OFF**.
3. Escolha o local e o nome do arquivo.
4. Abra o arquivo gerado em um programa compatível, como o [MeshLab](https://www.meshlab.net/).

## Documentação com Doxygen

A documentação do código pode ser gerada utilizando o Doxygen.

Após instalar o Doxygen, execute na pasta que contém o arquivo `Doxyfile`:

```bash
doxygen Doxyfile
```

A documentação HTML será gerada em `docs/html/`. Abra o arquivo `docs/html/index.html` para consultar as classes, os métodos e suas descrições.

## Estrutura do projeto

Os principais componentes do sistema são:

- **Sculptor:** gerencia a matriz tridimensional e as operações sobre os voxels.
- **Voxel:** representa uma unidade volumétrica da escultura.
- **FiguraGeometrica:** define a interface abstrata para as operações geométricas.
- **Classes derivadas:** implementam a criação e a remoção de formas.
- **Interpretador:** processa comandos textuais para construir esculturas.
- **SculptorView:** responsável pela visualização da escultura na interface gráfica.
- **MainWindow:** gerencia a janela principal, os controles e as interações do usuário.

## Conceitos aplicados

- Programação orientada a objetos.
- Herança, abstração e polimorfismo.
- Alocação dinâmica de memória.
- Matrizes tridimensionais.
- Estruturas de dados da STL.
- Geometria computacional.
- Desenvolvimento de interfaces gráficas com Qt.
- Interpretação de comandos.
- Exportação de modelos tridimensionais.

## Autores

- **Natan Bastos De Morais**
- **Thiago Vinicius Cardoso Lopes**

## Instituição

Universidade Federal do Rio Grande do Norte (UFRN)

## Licença

Este projeto foi desenvolvido para fins educacionais.
