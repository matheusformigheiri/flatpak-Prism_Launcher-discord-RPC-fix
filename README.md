# flatpak-Prism_Launcher-discord-RPC-fix

**PT-BR:** Solução leve em C para corrigir o status do Discord no Prism Launcher (Flatpak) via monitoramento nativo da árvore de processos host (prismrun -> java).

**EN-US:** Lightweight C solution to fix Discord status for Prism Launcher (Flatpak) by native monitoring of the host process tree (prismrun -> java).

---

## Downloads & Desenvolvimento / Downloads & Development

### PT-BR: Downloads e Modificações
Se você não deseja compilar o código fonte manualmente, pode baixar o binário pronto:
* [Baixar o prism_rpc-fix v1.0.0 (Última Versão)](https://github.com/matheusformigheiri/flatpak-Prism_Launcher-discord-RPC-fix/releases/tag/1.0.0)

Se você é um desenvolvedor, quer entender o código ou prefere compilar manualmente por segurança, acesse nossa pasta de desenvolvimento com o código-fonte e instruções:
* [Pasta de Desenvolvimento e Código-Fonte](./source/)

---

### EN-US: Downloads and Modifications
If you do not want to compile the source code manually, you can download the pre-compiled binary:
* [Download prism_rpc-fix v1.0.0 (Latest Version)](https://github.com/matheusformigheiri/flatpak-Prism_Launcher-discord-RPC-fix/releases/tag/1.0.0)

If you are a developer, want to inspect the code, or prefer to compile manually for security reasons, check out our development folder with source code and instructions:
* [Development Folder and Source Code](./source/)

---

## Instruções de Instalação e Uso / Installation and Setup Instructions

### Passo 1: Configurar Permissão do Flatpak / Step 1: Configure Flatpak Permissions

**PT-BR:** O Prism Launcher roda isolado em um container. Para permitir que ele execute o fix no seu sistema real, abra o terminal e rode o comando:
```bash
sudo flatpak override org.prismlauncher.PrismLauncher --talk-name=org.freedesktop.Flatpak
```

**EN-US:** Prism Launcher runs inside an isolated container. To allow it to trigger the fix on your host system, open your terminal and run:
```bash
sudo flatpak override org.prismlauncher.PrismLauncher --talk-name=org.freedesktop.Flatpak
```

---

### Passo 2: Alocar o Binário / Step 2: Place the Binary

**PT-BR:** O arquivo executável `prism_rpc-fix` (baixado das Releases ou compilado manualmente) deve ser colocado obrigatoriamente no caminho abaixo:
```text
~/.local/bin/prism_rpc-fix
```
Certifique-se de dar permissão de execução ao arquivo executável rodando o comando abaixo no seu terminal:
```bash
chmod +x ~/.local/bin/prism_rpc-fix
```

**EN-US:** The executable file `prism_rpc-fix` (downloaded from Releases or compiled manually) must be placed exactly at the following path:
```text
~/.local/bin/prism_rpc-fix
```
Make sure to grant execution permissions to the executable file by running the following command in your terminal:
```bash
chmod +x ~/.local/bin/prism_rpc-fix
```

---

### Passo 3: Configurar o Prism Launcher / Step 3: Configure Prism Launcher

**PT-BR:**
1. Abra o Prism Launcher.
2. Na barra superior, clique em **Configurações**.
3. Na barra lateral esquerda, clique em **Minecraft**.
4. Dentro das opções do Minecraft, clique na aba **Comandos personalizados**.
5. No campo **Comando de pré-inicialização**, cole o seguinte comando exatamente como está:
   ```text
   flatpak-spawn --host sh -c "/home/$USER/.local/bin/prism_rpc-fix >/dev/null 2>&1 &"
   ```
6. Clique em **OK** para salvar.

**ATENÇÃO: FECHE COMPLETAMENTE O PRISM LAUNCHER E ABRA-O NOVAMENTE AGORA.** Se você não reiniciar o aplicativo neste momento, as novas permissões do Flatpak não serão carregadas e o fix falhará ao iniciar o jogo.

---

**EN-US:**
1. Open Prism Launcher.
2. In the top bar, click **Settings**.
3. In the left sidebar, click **Minecraft**.
4. Inside Minecraft options, click on the **Custom commands** tab.
5. In the **Pre-launch command** field, paste the following line exactly as shown:
   ```text
   flatpak-spawn --host sh -c "/home/\$USER/.local/bin/prism_rpc-fix >/dev/null 2>&1 &"
   ```
6. Click **OK** to save.

**ATTENTION: CLOSE PRISM LAUNCHER COMPLETELY AND REOPEN IT NOW.** If you do not restart the application at this step, the new Flatpak permissions will not load, and the fix will fail to launch alongside the game.

---

### Passo 4: Vincular no Discord / Step 4: Register on Discord

**PT-BR:**
1. Inicie qualquer instância do Minecraft pelo Prism Launcher.
2. Abra o seu aplicativo do Discord nativo (.deb).
3. Vá em **Configurações de Usuário** -> **Atividades Registradas** (Jogos Registrados).
4. Você provavelmente verá a mensagem dizendo que nenhum jogo foi detectado. Clique no link azul **Adicione-o!**.
5. Na lista suspensa, selecione o processo chamado **prism_rpc-fix**.
6. Clique na caixa de texto do nome e mude para **Minecraft: Java Edition** (respeitando maiúsculas e minúsculas para que o Discord puxe a capa oficial do jogo automaticamente).

---

**EN-US:**
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

---

### EN-US: Technical Mechanism and the Problem It Solves
Prism Launcher in Flatpak runs isolated inside a sandbox (`bwrap`). When launching Minecraft, the actual `java` executable and its arguments are locked inside this hidden container. Common scripts fail because they cannot read these parameters from outside the Flatpak environment. Every time you switched instances, the window ID changed slightly, causing Discord to lose track of the game and treat it as a generic Java process.

This Rich Presence (RPC) fix was built from scratch in pure C to solve this without requiring any game mods. It works by mapping the host operating system's process tree from outside Flatpak:
1. The binary starts in the background of the host system, triggered by the launcher.
2. It waits for 5 seconds to let the Minecraft instance load its stable binaries into RAM.
3. It enters a lightweight loop inspecting the `/proc` directory every 5 seconds combining `pgrep`, `xargs`, and `pstree`.
4. It verifies whether the active `java` process is a direct descendant of the Flatpak instance manager, `prismrun`.
5. As long as this parent-child relationship (`prismrun` -> `java`) remains true, the program runs under the fixed, static name `prism_rpc-fix`. As soon as the game is closed, the process tree link breaks, the loop terminates, and the program exits immediately, keeping the system clean without leaving ghost processes behind.


## Perguntas Frequentes & Solução de Problemas / FAQ & Troubleshooting

### PT-BR

Esta seção aborda problemas comuns de indexação, permissões do Flatpak e detecção de status do Discord ao usar o Prism Launcher.

#### O Discord não detecta o processo prism_rpc-fix na lista de jogos.
* **Causa provável:** O Prism Launcher não foi totalmente reiniciado após o comando de permissão, ou o arquivo executável está na pasta errada.
* **Solução:** 
  1. Certifique-se de que fechou o launcher completamente (não apenas minimizou na bandeja do sistema).
  2. Verifique se o arquivo está exatamente em `~/.local/bin/prism_rpc-fix` e se você executou o comando `chmod +x` nele.
  3. Abra um terminal fora do Flatpak e digite `prism_rpc-fix`. Se o sistema disser "comando não encontrado", a sua pasta `.local/bin` não está adicionada ao seu `$PATH` do sistema.

#### Minha atividade no Discord some ou pisca quando mudo de versão do Minecraft.
* **Causa provável:** O comando personalizado foi configurado dentro do perfil de uma única instância.
* **Solução:** Remova o comando das configurações da instância e cole-o nas configurações **Globais** do Prism Launcher (Configurações -> Minecraft -> Comandos personalizados) para abranger o ecossistema inteiro.

#### Erro flatpak-spawn: command not found ou falha ao iniciar o jogo.
* **Causa provável:** Você pode estar tentando rodar o Prism Launcher instalado via repositório nativo (.deb, .rpm, AUR) em vez da versão Flatpak oficial do Flathub.
* **Solução:** Este watchdog foi desenhado especificamente para o ecossistema de sandbox do Flatpak. Se você usa o Prism Launcher nativo, o comando `flatpak-spawn --host` falhará porque o launcher já está rodando diretamente no host.

---

### EN-US

This section targets common indexing issues, Flatpak permission blocks, and Discord status detection bugs when playing Minecraft via Prism Launcher.

#### Discord won't detect the prism_rpc-fix process in the Registered Games list.
* **Likely Cause:** Prism Launcher was not fully restarted after applying the override permission, or the binary is missing/misplaced.
* **Solution:**
  1. Make sure to completely close the launcher (don't just minimize it to the system tray).
  2. Double-check that the file is located precisely at `~/.local/bin/prism_rpc-fix` and that you ran `chmod +x` on it.
  3. Open a system terminal (outside Flatpak) and run `prism_rpc-fix`. If you get a "command not found" error, your user `.local/bin` directory is not part of your system's `$PATH`.

#### Discord status disappears or loses reference when switching Minecraft versions.
* **Likely Cause:** The custom command was applied into a single local instance profile.
* **Solution:** Verify that the pre-launch custom command is added to the **Global Settings** of Prism Launcher (Settings -> Minecraft -> Custom Commands), rather than a specific profile.

#### Error flatpak-spawn: command not found or launch failure.
* **Likely Cause:** You are likely using a native system package (.deb, .rpm, Arch AUR) of Prism Launcher instead of the official Flathub Flatpak package.
* **Solution:** This watchdog script relies entirely on Flatpak's sandbox escape mechanics. If your launcher runs natively on the host, `flatpak-spawn` is not required and will return an error.

---

### Tabela de Erros Rápidos / Quick Error Reference

| Sintoma / Symptom | Causa Raiz / Root Cause | Correção / Fix |
| :--- | :--- | :--- |
| **Discord "Playing Java"** | Discord tracked raw `java` process | Remova Java das atividades e adicione o `prism_rpc-fix` / Remove Java from games and add `prism_rpc-fix` |
| **Game won't launch** | Sandbox permission blocked | Execute o `sudo flatpak override` e reinicie o launcher / Run `sudo flatpak override` and restart launcher |
| **Ghost processes left behind** | Host process tree sync lag | O script checa a árvore a cada 5s; aguarde ou force `pkill -f prism_rpc-fix` / Checks process tree every 5s; wait or run `pkill -f prism_rpc-fix` |
