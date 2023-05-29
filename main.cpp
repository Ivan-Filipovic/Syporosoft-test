#include <iostream>
#include <Windows.h>
#include <chrono>
#include <mutex>

volatile bool A, B, C = false;
HANDLE mutex;

void turnOnA();
void turnOnB();
void turnOnC();
void turnOffA();
void turnOffB();
void turnOffC();
void turnOffJustB();
void turnOffJustC();

void turnOnA()
{
    if (!A)
    {
        std::cout << "+A se pali" << std::endl;
        Sleep(2 * 1000);
        A = true;
        std::cout << "++A se upalio" << std::endl;
        turnOnB();
        turnOnC();
    }
}

void turnOffA()
{
    if (A)
    {
        std::cout << "-A se gasi" << std::endl;
        Sleep(2 * 1000);
        A = false;
        std::cout << "--A se ugasio" << std::endl;
        turnOffJustB();
        turnOffJustC();

        turnOnA();
        turnOnB();
        turnOnC();
    }
}

void turnOnB()
{
    if (!B)
    {
        std::cout << "+B se pali" << std::endl;
        Sleep(2 * 1000);
        B = true;
        std::cout << "++B se upalio" << std::endl;
    }
}

void turnOffB()
{
    if (B)
    {
        std::cout << "-B se gasi" << std::endl;
        Sleep(2 * 1000);
        B = false;
        std::cout << "--B se ugasio" << std::endl;
        turnOffJustC();
        turnOnB();
        turnOnC();
    }
}

void turnOffJustB()
{
    if (B)
    {
        std::cout << "-B se gasi" << std::endl;
        Sleep(2 * 1000);
        B = false;
        std::cout << "--B se ugasio" << std::endl;
    }
}

void turnOnC()
{
    if (!C)
    {
        std::cout << "+C se pali" << std::endl;
        Sleep(2 * 1000);
        C = true;
        std::cout << "++C se upalio" << std::endl;
    }
}

void turnOffC()
{
    if (C)
    {
        std::cout << "-C se gasi" << std::endl;
        Sleep(2 * 1000);
        C = false;
        std::cout << "--C se ugasio" << std::endl;
        turnOnC();
    }
}

void turnOffJustC()
{
    if (C)
    {
        std::cout << "-C se gasi" << std::endl;
        Sleep(2 * 1000);
        C = false;
        std::cout << "--C se ugasio" << std::endl;
    }
}

DWORD WINAPI inputScanner(LPVOID lpParam)
{
    std::string input;
    while (1)
    {
        std::getline(std::cin, input);
        switch (input[0])
        {
        case 'A':
            turnOffA();
            break;
        case 'B':
            turnOffB();
            break;
        case 'C':
            turnOffC();
            break;
        default:
            std::cout << "Neispravan unos" << std::endl;
        }
    }
}

int main()
{
    HANDLE scannerHandle = CreateThread(NULL, 0, inputScanner, NULL, 0, NULL);
    if (scannerHandle == NULL)
    {
        std::cerr << "Failed to create thread " << std::endl;
        return 1;
    }
    mutex = CreateMutex(NULL, FALSE, NULL);
    if (mutex == NULL)
    {
        std::cerr << "Failed to create mutex" << std::endl;
        return 1;
    }

    turnOnA();
    turnOnB();
    turnOnC();

    std::cout << "Program je spreman za rad, za gasenje pojedinog elementa upisite njegovo ime u konzolu [A,B,C] i pritisnite enter" << std::endl;
    Sleep(1000);
    std::cout << "Za gasenje programa stisnite kombinaciju tipki CTRL i C" << std::endl;
    Sleep(1000);
    std::cout << "Program krece sa radom za 2 sekunde." << std::endl;
    Sleep(2000);

    while (1)
    {
        WaitForSingleObject(mutex, INFINITE);
        std::cout << "----------" << std::endl;
        std::cout << "A " << ((A == true) ? "upaljeno" : "ugaseno") << std::endl;
        std::cout << "B " << ((B == true) ? "upaljeno" : "ugaseno") << std::endl;
        std::cout << "C " << ((C == true) ? "upaljeno" : "ugaseno") << std::endl;
        std::cout << "----------" << std::endl;
        ReleaseMutex(mutex);
        Sleep(5 * 1000);
    };

    WaitForSingleObject(scannerHandle, INFINITE);

    CloseHandle(scannerHandle);
    CloseHandle(mutex);

    return 0;
}
