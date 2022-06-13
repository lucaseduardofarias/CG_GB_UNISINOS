![](https://upload.wikimedia.org/wikipedia/pt/9/91/Lp_logo_unisinos.png)

## Trabalho GB Computação Gráfica

**Como fazer funcionar os projetos:**

> Abra o cmd na pasta que deseja clonar o projeta e execute o comando : 
> 
> git clone https://github.com/lucaseduardofarias/CG_GB_UNISINOS.git
> 


> Ao acessar a pasta criada, existe um arquivo com final '.sln' exemplo : TrabalhoGB.sln, o projeto devem ser executado através desse arquivo.


> Cada projeto foi configurado com as seguintes informações.

>Diretórios de Inclusão Adicionais:
>
>$(SolutionDir)External/GLEW/include;$(SolutionDir)External/GLFW/include;$(SolutionDir)External/SOIL2/include;$(SolutionDir)External/GLM;%(AdditionalIncludeDirectories);

>Diretórios de Biblioteca Adicionais:
>
>$(SolutionDir)External/GLEW/lib/Release/x64;$(SolutionDir)External/GLFW/lib-vc2015;$(SolutionDir)External/SOIL2/lib;%(AdditionalLibraryDirectories);

>Dependências Adicionais:
>
opengl32.lib;
glew32.lib;
glfw3.lib;
SOIL.lib;
%(AdditionalDependencies)

Linguagens utilizadas no projeto.

![alt text](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![alt text](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

# Alunos
Lucas Eduardo Farias, Maithe Mikaele Mello

Comando | Descrição
------------ | -------------
 1 | Seleciona o primeiro objeto                            
 2 | Seleciona o segundo objeto                             
 W | translata o objeto +N unidades no eixo Y               
 S | translata o objeto -N unidades no eixo Y               
 A | translata o objeto -N unidades no eixo X               
 D | translata o objeto +N unidades no eixo X
 E | rotaciona N graus para direita em relação ao eixo Y    
 Q | rotaciona N graus para a esquerda em relação ao eixo Y 
 Z | rotaciona N graus para direita em relação ao eixo X    
 C | rotaciona N graus para a esquerda em relação ao eixo X 
Tab | rotaciona N graus para direita em relação ao eixo Z    
Capslock | rotaciona N graus para direita em relação ao eixo Z    
 + | escala o objeto em 10% a mais do seu tamanho atual - NUMPAD    
 - | escala o objeto em 10% a mais do seu tamanho atual - NUMPAD     
 → | move câmera para direita                      
 ← | move câmera para esquerda                     
 ↑ | move câmera para frente                                
 ↓ | move câmera para trás                                  
 Mouse | livre movimentação em todas direção da camera
 
 
 
 
