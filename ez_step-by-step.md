EASY STEP-BY-STEP GUIDE
=======================

Quick guide to get your Discord status working with Prism Launcher:

1. Terminal command: Run `sudo flatpak override org.prismlauncher.PrismLauncher --talk-name=org.freedesktop.Flatpak`

2. Download: Get the "prism_rpc-fix" file from the GitHub Releases page.

3. Place file: Move it to `~/.local/bin/` and ensure the name is strictly `prism_rpc-fix`.

4. Permissions: Run `chmod +x ~/.local/bin/prism_rpc-fix` in your terminal.

5. Prism setup: In Prism Launcher Settings -> Minecraft -> Custom commands, 
paste `flatpak-spawn --host sh -c "/home/$USER/.local/bin/prism_rpc-fix >/dev/null 2>&1 &"` under Pre-launch command and click OK.

6. Restart: Completely close and reopen Prism Launcher.

7. Discord setup: Launch Minecraft, go to Discord Settings -> Registered Games -> Add it!, select "prism_rpc-fix", and rename it to "Minecraft: Java Edition".

Done! Your status will now update automatically.
