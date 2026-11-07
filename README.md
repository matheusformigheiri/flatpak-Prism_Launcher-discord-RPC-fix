# flatpak-Prism_Launcher-discord-RPC-fix

PT-BR: Solução leve em C para corrigir o status do Discord no Prism Launcher (Flatpak) via monitoramento nativo da árvore de processos host (prismrun -> java). 

EN-US: Lightweight C solution to fix Discord status for Prism Launcher (Flatpak) by native monitoring of the host process tree (prismrun -> java).

## Instruções de Instalação e Uso / Installation and Setup Instructions

### PT-BR:

### Passo 1: Configurar Permissão do Flatpak
O Prism Launcher roda isolado em um container. Para permitir que ele execute o fix no seu sistema real, abra o terminal e rode o comando:
```bash
sudo flatpak override org.prismlauncher.PrismLauncher --talk-name=org.freedesktop.Flatpak
```

### Passo 2: Baixar ou Compilar o Binário
Você pode compilar o arquivo `main.c` por conta própria ou baixar o executável pronto fornecido neste repositório. O arquivo final deve ser colocado obrigatoriamente no caminho abaixo:
```text
~/.local/bin/prism_rpc-fix
```
*Certifique-se de dar permissão de execução ao arquivo executável rodando `chmod +x ~/.local/bin/prism_rpc-fix` no seu terminal.*

### Passo 3: Configurar o Prism Launcher
1. Abra o Prism Launcher.
2. Na barra superior, clique em **Configurações**.
3. Na barra lateral esquerda, clique em **Minecraft**.
4. Dentro das opções do Minecraft, clique na aba **Comandos personalizados**.
5. No campo **Comando de pré-inicialização**, cole o seguinte comando exatamente como está:
   ```text
   flatpak-spawn --host sh -c "/home/$USER/.local/bin/prism_rpc-fix >/dev/null 2>&1 &"
   ```
6. Clique em **OK** para salvar.

**ATENÇÃO: FECHE COMPLETAMENTE O PRISM LAUNCHER E ABRA-O NOVAMENTE AGORA.** Se você não reiniciar o aplicativo neste momento, as novas permissões do Flatpak não serão carregadas e o jogo não vai iniciar.

### Passo 4: Vincular no Discord
1. Inicie qualquer instância do Minecraft pelo Prism Launcher.
2. Abra o seu aplicativo do Discord nativo (.deb).
3. Vá em **Configurações de Usuário** -> **Atividades Registradas** (Jogos Registrados).
4. Você provavelmente verá a mensagem dizendo que nenhum jogo foi detectado. Clique no link azul **Adicione-o!**.
5. Na lista suspensa, selecione o processo chamado **prism_rpc-fix**.
6. Clique na caixa de texto do nome e mude para **Minecraft: Java Edition** (respeitando maiúsculas e minúsculas para que o Discord puxe a capa oficial do jogo automaticamente).

---

### EN-US:

### Step 1: Configure Flatpak Permissions
Prism Launcher runs inside an isolated container. To allow it to trigger the fix on your host system, open your terminal and run:
```bash
sudo flatpak override org.prismlauncher.PrismLauncher --talk-name=org.freedesktop.Flatpak
```

### Step 2: Download or Compile the Binary
You can either compile `main.c` yourself or download the pre-compiled executable provided in this repository. The final file must be placed exactly at:
```text
~/.local/bin/prism_rpc-fix
```
*Make sure to grant execution permissions to the file by running `chmod +x ~/.local/bin/prism_rpc-fix` in your terminal.*

### Step 3: Configure Prism Launcher
1. Open Prism Launcher.
2. In the top bar, click **Settings**.
3. In the left sidebar, click **Minecraft**.
4. Inside Minecraft options, click on the **Custom commands** tab.
5. In the **Pre-launch command** field, paste the following line exactly as shown:
   ```text
   flatpak-spawn --host sh -c "/home/\$USER/.local/bin/prism_rpc-fix >/dev/null 2>&1 &"
   ```
6. Click **OK** to save.

**ATTENTION: CLOSE PRISM LAUNCHER COMPLETELY AND REOPEN IT NOW.** If you do not restart the application at this step, the new Flatpak permissions will not load, and the game will fail to launch.

### Step 4: Register on Discord
1. Launch any Minecraft instance via Prism Launcher.
2. Open your native Discord client (.deb).
3. Go to **User Settings** -> **Registered Games**.
4. You will likely see a message stating that no game is detected. Click the blue **Add it!** link.
5. From the dropdown list, select the process named **prism_rpc-fix**.
6. Click on the process name text box and rename it to **Minecraft: Java Edition** (match case exactly so Discord fetches the official game cover artwork automatically).

---

## Como o Código Funciona / How It Works

### PT-BR: Funcionamento Técnico e o Problema que Ele Corrige
O Prism Launcher em Flatpak roda isolado em um sandbox (`bwrap`). Ao iniciar o Minecraft, o executável `java` real e seus argumentos ficam presos dentro desse container oculto. Scripts comuns falham porque não conseguem ler esses parâmetros de fora da caixinha do Flatpak. Toda vez que você mudava de instância, o ID da janela mudava levemente e o Discord perdia a associação do jogo, tratando-o como um processo genérico do Java.

Este fix de Rich Presence (RPC) foi construído do zero em C puro para resolver isso sem a necessidade de instalar nenhum mod no jogo. Ele funciona mapeando a árvore de processos do sistema operacional real de fora do Flatpak:
1. O binário inicia em segundo plano no sistema host, disparado pelo launcher.
2. Ele aguarda 5 segundos para que a instância do Minecraft carregue os binários estáveis na memória RAM.
3. Entra em um loop leve que inspeciona o diretório `/proc` a cada 5 segundos combinando `pgrep`, `xargs` e `pstree`.
4. Ele valida se o processo `java` ativo é um descendente direto do gerenciador de instâncias do Flatpak, o `prismrun`.
5. Enquanto essa relação de parentesco (`prismrun` -> `java`) for verdadeira, o programa continua rodando sob o nome único e estático `prism_rpc-fix`. Assim que o jogo é fechado, a relação na árvore de processos é desfeita, o loop quebra e o programa encerra a si mesmo imediatamente, limpando o sistema sem deixar processos fantasmas.

### EN-US: Technical Mechanism and the Problem It Solves
Prism Launcher in Flatpak runs isolated inside a sandbox (`bwrap`). When launching Minecraft, the actual `java` executable and its arguments are locked inside this hidden container. Common scripts fail because they cannot read these parameters from outside the Flatpak environment. Every time you switched instances, the window ID changed slightly, causing Discord to lose track of the game and treat it as a generic Java process.

This Rich Presence (RPC) fix was built from scratch in pure C to solve this without requiring any game mods. It works by mapping the host operating system's process tree from outside Flatpak:
1. The binary starts in the background of the host system, triggered by the launcher.
2. It waits for 5 seconds to let the Minecraft instance load its stable binaries into RAM.
3. It enters a lightweight loop inspecting the `/proc` directory every 5 seconds combining `pgrep`, `xargs`, and `pstree`.
4. It verifies whether the active `java` process is a direct descendant of the Flatpak instance manager, `prismrun`.
5. As long as this parent-child relationship (`prismrun` -> `java`) remains true, the program runs under the fixed, static name `prism_rpc-fix`. As soon as the game is closed, the process tree link breaks, the loop terminates, and the program exits immediately, keeping the system clean without leaving ghost processes behind.
