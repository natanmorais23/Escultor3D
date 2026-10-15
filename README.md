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

A documentação HTML será gerada no diretório configurado no arquivo `Doxyfile`. Abra o arquivo `index.html` gerado para consultar as classes, os métodos e suas descrições.

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
