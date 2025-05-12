#include <stdio.h>
#include <windows.h>
#include <synchapi.h>
#include <errhandlingapi.h>
#include <tlhelp32.h>

// sucht nach dem gewünschten Prozess
BOOL IsProcessRunning(char* processName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (strcmp(entry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return TRUE;
            }
        }
    }

    CloseHandle(snapshot);
    return FALSE;
}

// Kopiert die mp3 Datei zum CircuitPY
int trigger(char* path) {
    char* name = calloc(1024, sizeof(char));
    sprintf(name, "%strigger.mp3", path);
    if (!CopyFile("trigger.mp3", name, FALSE)) {
        DWORD error = GetLastError();
        fprintf(stderr, "Es ist ein Fehler im CircuitPY aufgetreten mit dem Fehlercode: %lu\n", error);

        LPVOID lpMsgBuf;
        FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);

        printf("Fehlermeldung: %s\n", (char *)lpMsgBuf);
        return -1;
    }
    return 0;
}

// Löscht die mp3 Datei vom CircuitPY
int delete_trigger(char* path) {
    char* name = calloc(1024, sizeof(char));
    sprintf(name, "%strigger.mp3", path);
    if (!DeleteFile(name)) {
        DWORD error = GetLastError();
        fprintf(stderr, "Es ist ein Fehler im CircuitPY aufgetreten mit dem Fehlercode: %lu\n", error);

        LPVOID lpMsgBuf;
        FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf, 0, NULL);

        printf("Fehlermeldung: %s\n", (char *)lpMsgBuf);
        return -1;
    }
    return 0;
}

int main(int argc, char** argv) {
    //Wenn weniger als 2 argumente gegeben sind, sollte das Program mit einer Fehlermeldung beendet werden
    if (argc < 2) {
        fprintf(stderr, "Bitte einen Dienstnamen angeben");
        return -1;
    }

    char drive_letter = 'H';
    char* path = malloc(3); //malloc ist nötig für sprintf
    if (path == NULL) {
        fprintf(stderr, "Nicht ausreichend Speicher");
        return -100;
    }

    //Wenn ein 3. argumente gegeben ist, das als Laufwerksbuchstaben verwenden 
    if (argc == 3) {
        //Wenn das 2. Argument mehr als ein Buchstabe ist, wird es kein Laufwerksbuchstabe sein
        if (strlen(argv[2]) > 1) {
            fprintf(stderr, "Bitte nur einen Buchstaben als Laufwerksbuchstaben angeben");
            return -5;
        }
        drive_letter = *argv[2];
    }
    sprintf(path, "%c:\\", drive_letter); 

    char* targetProcessName = argv[1];
    int triggered = 0;
    int err = 0;

    // Schaut alle jede Sekunde ob der unerwünschte Prozess läuft
    while (1){
        if (IsProcessRunning(targetProcessName)) {
            if (triggered == 0) {
                 err = trigger(path);
                triggered = 1;
            }
        } else if (triggered == 1) {
            err = delete_trigger(path);
            triggered = 0;
        }

        if (err != 0) {
            fprintf(stderr, "Es ist ein Fehler aufgetreten!");
            return -10;
        }

        Sleep(1000);
    }

    return 0;
}
