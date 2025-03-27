// Para compilar no Codeblocks, acrescentar o parâmetro -mwindows em
// project>build options>linker settings>other linker settings

// Fonte de estudos sobre Win32 API : http://www.winprog.org/tutorial/window_click.html

#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawRectangles(HWND);

int matriz[9][9] =
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	MSG  msg;
	WNDCLASSW wc = {0};

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"FloodFillRCS";
	wc.hInstance     = hInstance;
	wc.hbrBackground = GetSysColorBrush(COLOR_APPWORKSPACE);
	wc.lpfnWndProc   = WndProc;
	wc.hCursor       = LoadCursor(0, IDC_ARROW);

	RegisterClassW(&wc);
	CreateWindowW(wc.lpszClassName, "Flood Fill 8 direções Recursivo",
				  WS_VISIBLE | WS_EX_WINDOWEDGE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
				  100, 100, 600, 700, NULL, NULL, hInstance, NULL);

	//WS_OVERLAPPEDWINDOW |

	while (GetMessage(&msg, NULL, 0, 0))
		{
			DispatchMessage(&msg);
		}

	return (int) msg.wParam;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg,
						  WPARAM wParam, LPARAM lParam)
{
	switch(msg)
    {
        case WM_CREATE:

            CreateWindowW(L"button", L"Limpa",
                WS_VISIBLE | WS_CHILD ,
                300, 610, 80, 25,
                hwnd, (HMENU) 0, NULL, NULL);

            CreateWindowW(L"button", L"Inicia",
                WS_VISIBLE | WS_CHILD ,
                400, 610, 80, 25,
                hwnd, (HMENU) 1, NULL, NULL);

            CreateWindowW(L"button", L"Sai",
                WS_VISIBLE | WS_CHILD ,
                500, 610, 80, 25,
                hwnd, (HMENU) 2, NULL, NULL);
            break;
        case WM_COMMAND:

            if (LOWORD(wParam) == 0)
                {
                Limpa(hwnd);
                }

            if (LOWORD(wParam) == 1)
                {
                fFloodFill_8_Direcoes(hwnd);
                }

            if (LOWORD(wParam) == 2)
                {
                PostQuitMessage(0);
                }

            break;
        case WM_PAINT:
            if (wParam==1)
                PreencheRetangulo(hwnd, lParam);
            else
                DrawRectangles(hwnd);
            break;

        case WM_DESTROY:

            PostQuitMessage(0);
            return 0;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
        }

	return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
void PreencheRetangulo(HWND hwnd, int node[])
	{

	int i, j;
	PAINTSTRUCT ps;
	RECT rect;
	LPCRECT pRect;
	i=node[0];
	j=node[1];
	rect.left=i*64+10;
	rect.top=j*64+10;
	rect.right=i*64+74;
	rect.bottom=j*64+74;
	pRect=&rect;

	InvalidateRect(hwnd,NULL,FALSE);

	HDC hdc = BeginPaint(hwnd, &ps);

	HPEN hBSalmao = CreateSolidBrush(RGB(255, 192, 128));
	SelectObject(hdc, hBSalmao);

	Rectangle(hdc, rect.left+1, rect.top+1, rect.right-1, rect.bottom-1);

	//FillRect(hdc,pRect, hBrushSalmao);
	DeleteObject(hBSalmao);

	EndPaint(hwnd, &ps);
	ReleaseDC(hwnd,hdc);

	InfoNode(hwnd, node);
	}
void InfoNode(HWND hwnd, int node[])
{
    int i, j;
    RECT rect;
    HFONT hFont;
    HDC hdc;
    char * texto[20];
	PAINTSTRUCT ps;
	i=node[0];
	j=node[1];

	sprintf(texto, "Célula: %d,%d",j+1,i+1);

	InvalidateRect(hwnd,NULL,FALSE);

	hdc = BeginPaint(hwnd, &ps);

	//HBRUSH hBrushAzul = CreateSolidBrush(RGB(50, 50, 200));
	//SelectObject(hdc, hBrushAzul);
	SetTextColor(hdc, RGB(50,50,200));
	hFont = CreateFontW(25, 0, 0, 0, FW_MEDIUM, 0, 0, 0, 0,
          0, 0, 0, 0, L"Georgia");

	SetRect(&rect, 100, 610, 70, 655);
	TextOut(hdc, 100,610, texto, 12);
	InvalidateRect(hdc, &rect, FALSE);
	//DrawText(hdc, texto, -1,&rect, DT_NOCLIP);
	DeleteObject(hFont);
	EndPaint(hwnd, &ps);
	ReleaseDC(hwnd,hdc);

}
void DrawRectangles(HWND hwnd)
	{
	int i, j;
	PAINTSTRUCT ps;
	RECT rect;

    InvalidateRect(hwnd,NULL,FALSE);
	HDC hdc = BeginPaint(hwnd, &ps);
	//HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	//HPEN holdPen = SelectObject(hdc, hPen);

	HBRUSH hBrushPreto = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hBrushBranco = CreateSolidBrush(RGB(255, 255, 255));

	HPEN hPenCinza = CreatePen(PS_DOT,2, RGB(244, 244, 244));

	//HBRUSH holdBrush =
	SelectObject(hdc, hBrushBranco);
	SelectObject(hdc, hPenCinza);
	for (i=0; i<9; i++)
    {
        for (j=0; j<9; j++)
        {
            int iCor=matriz[i][j];
            if (iCor==0)
                SelectObject(hdc, hBrushPreto);
            else
                SelectObject(hdc, hBrushBranco);
            rect.left=i*64+10;
            rect.top=j*64+10;
            rect.right=i*64+74;
            rect.bottom=j*64+74;
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        }
    }

	//SelectObject(hdc, holdPen);
	//SelectObject(hdc, holdBrush);

	DeleteObject(hPenCinza);
	DeleteObject(hBrushBranco);
	DeleteObject(hBrushPreto);

	EndPaint(hwnd, &ps);
	ReleaseDC(hwnd,hdc);
	}
void fFloodFill_8_Direcoes(HWND hwnd)
{
	int node[2];
	int targetColor=1;  // branco
	int replacementColor=2;  // salmão
	//Aponta inicialmente para a célula do meio (5,5):
	node[0]=4;
	node[1]=4;
	Flood_Fill_Recursivo_8_Direcoes(node, targetColor, replacementColor, hwnd);
}
void Flood_Fill_Recursivo_8_Direcoes(int node[], int targetColor, int replacementColor, HWND hwnd)
{
	if (node[0]==8 && node[1]==3)
    {
		int jj=9;
	}
	int i;
	int nodeTemp[2];
	if (targetColor==replacementColor) return;
	int iCorNode=matriz[node[0]][node[1]];
	if (iCorNode!=targetColor) return;
	for (i=0; i<50000000; i++) {}; //Temporizador
    matriz[node[0]][node[1]]=replacementColor;
    memcpy(nodeTemp, node, 2* sizeof(int));
    //Envia mensagem para pintura do nó atual
    SendMessage(hwnd, WM_PAINT, 1, (LPARAM) node);

    nodeTemp[0]++;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
    nodeTemp[1]++;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
    nodeTemp[1]-=2;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
    memcpy(nodeTemp, node, 2* sizeof(int));
    nodeTemp[0]--;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
    nodeTemp[1]++;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
    nodeTemp[1]-=2;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
    memcpy(nodeTemp, node, 2* sizeof(int));
    nodeTemp[1]--;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);

    memcpy(nodeTemp, node, 2* sizeof(int));
    nodeTemp[1]++;
    Flood_Fill_Recursivo_8_Direcoes(nodeTemp, targetColor, replacementColor, hwnd);
	}
void Limpa(HWND hwnd)
{
	int matriz1[9][9] =
	{
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0, 1, 1, 1, 0},
        {0, 1, 1, 0, 1, 1, 1, 1, 0},
        {0, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 1, 0, 1, 1, 0},
        {0, 1, 1, 1, 0, 1, 1, 1, 0},
        {0, 1, 1, 1, 0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	memcpy(matriz,matriz1,sizeof(matriz));
	SendMessage(hwnd, WM_PAINT,0,0);
}
