# Development and Compilation Instructions / Instruções de Desenvolvimento e Compilação

## PT-BR: Guia do Desenvolvedor

Esta pasta contém o código-fonte das versões do fix e as instruções necessárias para quem deseja modificar, debugar ou recompilar o binário nativamente.

### Requisitos do Sistema
Para compilar este projeto, você precisa dos seguintes utilitários instalados no seu sistema operacional host (fora do Flatpak):
* `gcc` (GNU Compiler Collection)
* `pstree` (Geralmente incluído no pacote `psmisc` na maioria das distribuições Linux)

No Ubuntu/Debian, você pode instalar os requisitos rodando:
```bash
sudo apt update && sudo apt install build-essential psmisc -y
```

### Como Compilar
Para gerar o binário a partir do código-fonte, navegue até esta pasta no terminal e execute o comando abaixo substituindo `v1.0.0.c` pela versão que deseja compilar:
```bash
gcc v1.0.0.c -o prism_rpc-fix
```

### Como Testar Modificações
1. Mova o novo binário gerado para a pasta local de binários:
   ```bash
   mv prism_rpc-fix ~/.local/bin/prism_rpc-fix
   ```
2. Certifique-se de dar a permissão de execução:
   ```bash
   chmod +x ~/.local/bin/prism_rpc-fix
   ```
3. Abra o Prism Launcher e verifique a saída do terminal ou os logs do sistema caso aplique modificações no tempo de loop ou nos comandos de busca.

---

## EN-US: Developer Guide

This folder contains the source code for the fix versions and the necessary guidelines for anyone who wants to modify, debug, or recompile the binary natively.

### System Requirements
To compile this project, you need the following utilities installed on your host operating system (outside Flatpak):
* `gcc` (GNU Compiler Collection)
* `pstree` (Usually included in the `psmisc` package on most Linux distributions)

On Ubuntu/Debian, you can install the requirements by running:
```bash
sudo apt update && sudo apt install build-essential psmisc -y
```

### How to Compile
To generate the binary from the source code, navigate to this folder in your terminal and run the following command, replacing `v1.0.0.c` with the version you wish to compile:
```bash
gcc v1.0.0.c -o prism_rpc-fix
```

### How to Test Modifications
1. Move the newly generated binary to your local binary directory:
   ```bash
   mv prism_rpc-fix ~/.local/bin/prism_rpc-fix
   ```
2. Make sure to grant execution permissions:
   ```bash
   chmod +x ~/.local/bin/prism_rpc-fix
   ```
3. Launch Prism Launcher and check the terminal output or system logs if you apply changes to the loop timing or search commands.
