# Dungeon Crawler C89

## 👥 Desenvolvedor
* Augusto Pereira Ribeiro Ferreira
## 🎯 Como Jogar
**Objetivo:** Explorar a vila e os três andares da masmorra, coletar chaves para abrir portas, desviar de armadilhas mortais e derrotar o Boss Final.

**Controles:**
* `W` - Move o jogador para cima
* `A` - Move o jogador para a esquerda
* `S` - Move o jogador para baixo
* `D` - Move o jogador para a direita
* `I` - Interage com NPCs ou destranca portas à frente
* `O` - Realiza um ataque de acordo com a arma equipada

## 🗺️ Significado dos Símbolos
* `A`, `<`, `V`, `>` : Jogador (indicando a direção do olhar)
* `*` : Parede (o jogador não pode passar)
* `#` : Espinho (o jogador morre ao passar por cima)
* `k` : Caixa (bloqueia a passagem, mas pode ser destruída com ataque)
* `O` : Botão (executa uma ação ao ser pressionado)
* `D` : Porta fechada (o jogador não pode passar)
* `@` : Chave (abre uma porta fechada ao interagir)
* `=` : Porta aberta (o jogador pode passar)
* `L` : Escada (leva o jogador para a próxima fase)
* `X` : Monstro Tipo 1 (Movimenta-se aleatoriamente)
* `Y` : Monstro Tipo 2 (Persegue o jogador)
* `Z` : Boss Final

## 🤖 Declaração sobre o uso de IA Generativa
A IA generativa (Gemini) foi utilizada neste projeto com o objetivo de auxiliar na identificação e correção de bugs (como o softlock no combate) e no balanceamento matemático das mecânicas de ataque das armas e inteligência dos monstros, visando adequar o código aos critérios da lauda. Nenhuma lógica principal foi gerada do zero pela IA, servindo apenas como ferramenta de revisão e otimização.
