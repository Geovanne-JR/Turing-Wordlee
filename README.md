# 🔠 Turing Wordlee | Linguagem C

Este é um jogo de quebra-cabeça de palavras desenvolvido em **Linguagem C** como projeto final da disciplina de Algoritmos e Programação de Computadores (APC) na **Universidade de Brasília (UnB)**.

<div align="center">
  <img src="Screenshots/titulo.png" width="600px">
</div>

## 🎮 Sobre o Projeto
O **Turing Wordlee** desafia o jogador a encontrar palavras ocultas a partir de um conjunto de letras. O projeto destaca-se pelo uso de **ASCII Art**, cores dinâmicas no terminal e um sistema completo de progressão e ranking.

---

## 🖼️ Galeria / Gallery

| <img src="Screenshots/menu_principal.png" width="400"> | <img src="Screenshots/gameplay.png" width="400"> |
| :---: | :---: |
| 📋 **Menu de Opções** | ⌨️ **Interface de Jogo** |
| <img src="Screenshots/instrucoes.png" width="400"> | <img src="Screenshots/configuracoes.png" width="400"> |
| 📖 **Manual de Instruções** | ⚙️ **Painel de Configurações** |

---

## 🏆 Sistema de Pontuação / Scoring System

| Ação / Action | Pontuação / Score |
| :--- | :---: |
| ✅ Acertar uma palavra nova | [cite_start]**+100 pts** [cite: 105] |
| 🔁 Tentar palavra já encontrada | [cite_start]**-10 pts** [cite: 106] |
| ❌ Errar a palavra | [cite_start]**-20 pts** [cite: 106] |
| 🏁 Concluir a fase | [cite_start]**+25 pts** [cite: 107] |

---

## ✨ Destaques Técnicos
* [cite_start]**Persistência de Dados:** Carregamento dinâmico de níveis via `fase.txt` e armazenamento de recordes em arquivo binário `ranking.bin`[cite: 164, 311, 330].
* [cite_start]**Algoritmos de Ordenação:** Implementação do algoritmo **Bubble Sort** para gerar o Hall da Fama (Ranking) em tempo real[cite: 167].
* **Interface Visual:** Uso de sequências de escape ANSI para renderização de cores e molduras "quadradinhas" no terminal.
* [cite_start]**Robustez:** Tratamento de strings com `toupper` para garantir que o jogo não diferencie maiúsculas de minúsculas[cite: 161].

---

## 📂 Estrutura do Repositório
* [cite_start]**`/src`**: Código-fonte principal (`252001154.c`)[cite: 161].
* [cite_start]**`/output`**: Arquivos de dados (`fase.txt`, `ranking.bin`) e o executável[cite: 311, 330].
* **`/Screenshots`**: Registros visuais da interface do jogo.

---

## 🚀 Como Executar
1. [cite_start]Certifique-se de que o arquivo `fase.txt` está na mesma pasta do programa[cite: 80].
2. Compile o código usando GCC:
   ```bash
   gcc src/252001154.c -o turing_wordlee
