/*	Assigement 2 --> Task 3 , CS-S4
*	Alaa Mahmoud Ebrahim 20190105
*	Josseph Diaa Saied 20190155
*/

#include <Windows.h>
#include <math.h>

int Round(double x){
	return (int)(x + 0.5);
}

void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c){
	SetPixel(hdc, xc + x, yc + y,c);
	SetPixel(hdc, xc + x, yc - y,c);
	SetPixel(hdc, xc - x, yc - y,c);
	SetPixel(hdc, xc - x, yc + y,c);
	SetPixel(hdc, xc + y, yc + x,c);
	SetPixel(hdc, xc + y, yc - x,c);
	SetPixel(hdc, xc - y, yc - x,c);
	SetPixel(hdc, xc - y, yc + x,c);
}

void DrawCircle1(HDC hdc,int xc, int yc, int R, COLORREF c){

	int x = 0;
	double y = R;
	Draw8Points(hdc, xc, yc, 0, R,c);
	while (x < y)
	{
		x++;
		y = sqrt((double)R * R - x * x);
		Draw8Points(hdc, xc, yc, x, Round(y),c);
	}

}


struct Vector {
	double v[2];
	Vector(double x = 0, double y = 0){
		v[0] = x; v[1] = y;
	}
	double& operator[](int i) {
		return v[i];
	}
};

void DrawHermiteCurve(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2, COLORREF c, int r, int xc, int yc){
	double a0 = p1[0], a1 = T1[0],
		a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
		a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];
	double b0 = p1[1], b1 = T1[1],
		b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
		b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];

	for (double t = 0; t <= 1; t += 0.001){
		double t2 = t * t, t3 = t2 * t;
		double x = a0 + a1 * t + a2 * t2 + a3 * t3;
		double y = b0 + b1 * t + b2 * t2 + b3 * t3;
		double distance = sqrt(((x - xc) * (x - xc)) + ((y - yc) * (y - yc)));
		if (distance < r) { //Clipping 
			SetPixel(hdc, Round(x), Round(y), c);
		}
		else {
			SetPixel(hdc, Round(x), Round(y), RGB(0, 0, 225));
		}
		
	}
}
LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	static Vector p[16];
	static Vector c[2];
	static int index = 0;
	static int idx = 0;
	static bool flag = false;
	switch (mcode)
	{
	case WM_LBUTTONDOWN:
		
		if (!flag) {
			c[idx] = Vector(LOWORD(lp), HIWORD(lp));
			if (idx == 1) {
				int r = sqrt(((c[0][0] - c[1][0]) * (c[0][0] - c[1][0])) + ((c[0][1] - c[1][1]) * (c[0][1] - c[1][1])));
				hdc = GetDC(hwnd);
				DrawCircle1(hdc, c[0][0], c[0][1], r, RGB(0, 0, 0));
				idx = 0;
				flag = true;
			}
			else {
				idx++;
			}
		}


		else{
			p[index] = Vector(LOWORD(lp), HIWORD(lp));
			
			if (index == 3) {
				hdc = GetDC(hwnd);
				int r = sqrt(((c[0][0] - c[1][0]) * (c[0][0] - c[1][0])) + ((c[0][1] - c[1][1]) * (c[0][1] - c[1][1])));
				Vector T1(3 * (p[1][0] - p[0][0]), 3 * (p[1][1] - p[0][1]));
				Vector T2(3 * (p[3][0] - p[2][0]), 3 * (p[3][1] - p[2][1]));
				DrawHermiteCurve(hdc, p[0], T1, p[3], T2, RGB(255, 0, 0),r,c[0][0],c[0][1]);	 //Top Right Curve

				p[4][0] = p[0][0] - r*1.4;
				p[5][0] = p[1][0] - r*1.4;
				p[6][0] = p[2][0] - r*1.4;
				p[7][0] = p[3][0] - r*1.4;
				p[4][1] = p[3][1];
				p[5][1] = p[2][1];
				p[6][1] = p[1][1];
				p[7][1] = p[0][1];

				Vector T5(3 * ( p[6][0]- p[7][0] ), 3 * (p[6][1] - p[7][1]));
				Vector T6(3 * (p[4][0] - p[5][0]), 3 * (p[4][1] - p[5][1]));
				DrawHermiteCurve(hdc, p[7], T5, p[4], T6 , RGB(255, 0, 0),r, c[0][0], c[0][1]);	 //Top Left Curve


				p[8][1] = p[0][1] + 50;
				p[9][1] = p[1][1] + 50;
				p[10][1] = p[2][1] + 50;
				p[11][1] = p[3][1] + 50;
				p[8][0] = p[0][0] ;
				p[9][0] = p[1][0] ;
				p[10][0] = p[2][0];
				p[11][0] = p[3][0];
				Vector T3(3 * ((p[9][0]) - (p[8][0])), 3 * ((p[9][1]) - (p[8][1])));
				Vector T4(3 * ((p[11][0]) - (p[10][0])), 3 * ((p[11][1]) - (p[10][1])));
				DrawHermiteCurve(hdc, p[8], T3, p[11], T4, RGB(255, 0, 0),r, c[0][0], c[0][1]);	 //Bottom Right Curve

				p[12][0] = p[4][0];
				p[13][0] = p[5][0];
				p[14][0] = p[6][0];
				p[15][0] = p[7][0];
				p[12][1] = p[4][1]+50;
				p[13][1] = p[5][1]+50;
				p[14][1] = p[6][1]+50;
				p[15][1] = p[7][1]+50;
				Vector T7(3 * (p[14][0] - p[15][0]), 3 * (p[14][1] - p[15][1]));
				Vector T8(3 * (p[12][0] - p[13][0]), 3 * (p[12][1] - p[13][1]));
				DrawHermiteCurve(hdc, p[15], T7, p[12], T8, RGB(255, 0, 0),r, c[0][0], c[0][1]);	 //Bottom Left Curve

				ReleaseDC(hwnd, hdc);
				index = 0;
			}
			else index++;


		}
		
		break;
	case WM_RBUTTONDOWN:
		
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, mcode, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = L"MyClass";
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(L"MyClass", L"My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 1000, 800, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}