Este projeto é uma aplicação interativa desenvolvida em C++/Qt, capaz de exibir e manipular objetos 2D e 3D em um ambiente tridimensional, utilizando uma câmera virtual, projeções ortográfica e perspectiva, transformações geométricas e ordenação de profundidade com Painter’s Algorithm.

Controles do Sistema

Modo Câmera (SHIFT pressionado)
| Tecla     | Ação                                     |
| --------- | ---------------------------------------- |
| SHIFT + W | mover para frente                        |
| SHIFT + S | mover para trás                          |
| SHIFT + A | mover para a esquerda                    |
| SHIFT + D | mover para a direita                     |
| SHIFT + Q | mover para cima                          |
| SHIFT + E | mover para baixo                         |
| SHIFT + I | pitch ↑                                  |
| SHIFT + K | pitch ↓                                  |
| SHIFT + J | yaw ←                                    |
| SHIFT + L | yaw →                                    |
| SHIFT + U | roll ←                                   |
| SHIFT + O | roll →                                   |
| SHIFT + P | alterna projeção Ortográfica/Perspectiva |

Modo Objeto 3D (objeto selecionado)
| Tecla             | Ação                          |
| ----------------- | ----------------------------- |
| ← ↑ ↓ →           | translação X/Y                |
| PageUp / PageDown | mover no eixo Z               |
| F / H             | rotacionar em Y               |
| T / G             | rotacionar em X               |
| R / Y             | rotacionar em Z               |
| = / -             | aumentar/diminuir escala      |
| , (vírgula)       | mover objeto para trás (Z–)   |
| . (ponto)         | mover objeto para frente (Z+) |

Modo Objeto 2D (objeto selecionado)
| Tecla   | Ação                           |
| ------- | ------------------------------ |
| ← ↑ ↓ → | mover                          |
| Z / X   | rotacionar                     |
| = / -   | escalar                        |
| , / .   | mover no eixo Z (profundidade) |

Seleção com o Mouse
- Clique em cima de um objeto 2D → ele é selecionado
- Clique em uma face projetada de um OBJ → o modelo é selecionado
