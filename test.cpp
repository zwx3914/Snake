#include <iostream>
#include "snake.h"

#define BUFSIZE 8192

int main()
{
	system("title ̰����");
	system("mode con cols=90 lines=45");
	/*˫������ʾ��������*/
    //��ȡĬ�ϱ�׼��ʾ���������
    HANDLE hOutput;
    COORD coord={0,0};
    hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
 
    //�����µĻ�����
    HANDLE hOutBuf = CreateConsoleScreenBuffer(
        GENERIC_READ | GENERIC_WRITE, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, 
        NULL, 
        CONSOLE_TEXTMODE_BUFFER, 
        NULL
    );
 
    //���������������Ĺ��
    CONSOLE_CURSOR_INFO cci;
    cci.bVisible=0;
    cci.dwSize=1;
    SetConsoleCursorInfo(hOutput, &cci);
    SetConsoleCursorInfo(hOutBuf, &cci);
	
	//��ʼ�˵�
	startMenu();
	
	//�����µĻ�����Ϊ���ʾ����
    SetConsoleActiveScreenBuffer(hOutBuf);
 
    //˫���崦����ʾ
    DWORD bytes=0;
    char data[BUFSIZE];

	//socket
	struct FoodString
	{
		char x[10];
		char y[10];
	};
	FoodString fs;
	char buff[20];
	
	bool on = true;
	while (true)
	{
		if (playing == false)
		{
			Sleep(3000);
			system("cls");
			memset(Canvas, 0, sizeof(Canvas));
			initialized();
		}

		if (isServer == true)
		{
			if (on)
			{
				server_client_Socket = accept(serverSocket, &server_client_addr, &nsize);
				on = false;
			}
			//server_client_Socket = accept(serverSocket, &server_client_addr, &nsize);
			send(server_client_Socket, &player[0].move, 1, 0);
			recv(server_client_Socket, &player[1].move, 1, 0);

			itoa(f.pos.x, fs.x, 10);
			itoa(f.pos.y, fs.y, 10);
			send(server_client_Socket, (char *)&fs, sizeof(FoodString), 0);
		}
		else if (isClient == true)
		{
			if (on)
			{	
				clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				on = false;
			}
			//clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			connect(clientSocket, (SOCKADDR*) &clinetAddr, sizeof(SOCKADDR));
			send(clientSocket, &player[1].move, 1, 0);
			recv(clientSocket, &player[0].move, 1, 0);

			recv(clientSocket, buff, sizeof(buff), 0);
			memcpy(&fs, buff, sizeof(buff));
			f.pos.x = atoi(fs.x);
			f.pos.y = atoi(fs.y);
			memset(buff, 0, sizeof(buff));
		}

		show();
		updateWithoutInput();
		updateWithInput();
		ReadConsoleOutputCharacterA(hOutput, data, BUFSIZE, coord, &bytes);
        WriteConsoleOutputCharacterA(hOutBuf, data, BUFSIZE, coord, &bytes);
		Sleep(300);

		/*
		if (isServer == true)
		{
			closesocket(server_client_Socket);
		}
		else if (isClient == true)
		{
			closesocket(clientSocket);
		}
		*/
	}
	closesocket(server_client_Socket);
	closesocket(clientSocket);
	system("pause");
	//std::cin.get();
	closesocket(serverSocket);

	WSACleanup();
	
	return 0;
}