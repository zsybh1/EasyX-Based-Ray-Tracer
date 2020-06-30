#include <graphics.h>			// 引用图形库头文件
#include <iostream>
#include <vector>
#include <easyxmath.h>
#include <easyxobject.h>
#include <windows.h>
using namespace std;

const int HEIGHT = 480 * 2, WIDTH = 640 * 2;

void draw_triangle_line(const triangle &T, const mat4 &M);
void draw_tricolor_triangle(const tricolor_triangle &T, const mat4 &M);


int main() {

	//===========================
	//== 初始化
	//===========================

	// 窗口设定
	initgraph(WIDTH, HEIGHT, EW_SHOWCONSOLE);
	setorigin(WIDTH / 2, HEIGHT / 2);
	setaspectratio(1, -1);

	// 物体设定
	vector<tricolor_triangle> objectList;
	objectList.emplace_back(pos3(0.0, 0.0, 9.0), pos3(2.0, 2.0, 3.0), pos3(0.0, 2.0, 3.0), BGR(RED), BGR(GREEN), BGR(YELLOW));

	mat4 Mper = perspective_mat4(radians(90.0f), (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
	mat4 Mview = scale_mat4(vec3(WIDTH / 2, HEIGHT / 2, 1.0f));

	float ror = 0.0f;
	//============================
	//== 遍历物体进行渲染
	//============================
	while (1) {
		ror += 1.0f;
		mat4 Mrot = rotate_mat4(radians(ror), vec3(0.0f, 0.0f, 1.0f));
		
		draw_tricolor_triangle(objectList[0], Mview * Mper * Mrot);
		Sleep(20);
	}
	saveimage(L"test.png");
	getchar();

	//==============================
	//== 善后
	//==============================

	closegraph();
	return 0;
}

void draw_triangle_line(const triangle &T, const mat4 &M) {
	pos3 A = (M * pos4(T.A, 1.0f)).xyz();
	pos3 B = (M * pos4(T.B, 1.0f)).xyz();
	pos3 C = (M * pos4(T.C, 1.0f)).xyz();
	line(A.x, A.y, B.x, B.y);
	line(B.x, B.y, C.x, C.y);
	line(C.x, C.y, A.x, A.y);
}

void draw_tricolor_triangle(const tricolor_triangle &T, const mat4 &M) {
	pos3 A = (M * pos4(T.A, 1.0f)).xyz();
	pos3 B = (M * pos4(T.B, 1.0f)).xyz();
	pos3 C = (M * pos4(T.C, 1.0f)).xyz();

	//计算包围三角形的最小轴对齐矩形
	int maxX = max(max(A.x, B.x), C.x);
	int maxY = max(max(A.y, B.y), C.y);
	int minX = min(min(A.x, B.x), C.x);
	int minY = min(min(A.y, B.y), C.y);

	vec3 vec[3] = { vec3(),B - A, C - A };
	float N = vec[1].x * vec[2].y - vec[2].x * vec[1].y;
	for (int i = maxY; i >= minY; --i) {
		for (int j = minX; j <= maxX; ++j) {
			//遍历每个像素，计算像素是否在三角形内
			vec[0] = pos3(j, i, 0) - A;
			float v = (vec[1].x * vec[0].y - vec[0].x * vec[1].y) / N;
			float u = (vec[0].x * vec[2].y - vec[0].y * vec[2].x) / N;
			if (0 <= u && 0 <= v && u + v <= 1) {
				//如果在，则插值上色
				color Color = T.Acolor * (1 - u - v) + T.Bcolor * u + T.Ccolor * v;
				putpixel(j, i, Color.getBGR());
			}
		}
	}
}