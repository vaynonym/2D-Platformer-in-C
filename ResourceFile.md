# RC-Datei zu RES kompilieren
windres appData.rc -O coff -o appDataCompiled.res

# Anpassen des Makefiles
```Makefile
main.exe: main.c hud.c loadGame.c collisionDetection.c collectible.c appDataCompiled.res
	$(CC) $(CFLAGS) -o main.exe main.c hud.c loadGame.c collisionDetection.c collectible.c appDataCompiled.res $(LDFLAGS)
```